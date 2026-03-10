#include "AirPortUtility.hpp"
#include "../Common/KextLog.hpp"
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IOWorkLoop.h>

namespace {
static constexpr uint64_t kMatchTimeoutNs = 1000000ULL;  // 1 ms

static const char* kWifiClasses[] = {
    "IO80211Interface",
    "IO80211Controller",
    "AirPort_BrcmNIC",
    "itlwm",
};

static const char* kNetworkClasses[] = {
    "IONetworkInterface",
    "IOEthernetInterface",
    "IO80211Interface",
};
}  // namespace

OSDefineMetaClassAndStructors(AirPortUtility, IOService)

bool AirPortUtility::init(OSDictionary* dictionary) {
  const bool ok = IOService::init(dictionary);
  if (ok) {
    workLoop_ = nullptr;
    watchdogTimer_ = nullptr;
    consecutiveUnstable_ = 0;
    recoveryCount_ = 0;
    HN_KEXT_LOG("AirPortUtility", "init");
  }
  return ok;
}

void AirPortUtility::free(void) {
  teardownWatchdog();
  HN_KEXT_LOG("AirPortUtility", "free");
  IOService::free();
}

IOService* AirPortUtility::probe(IOService* provider, SInt32* score) {
  HN_KEXT_LOG("AirPortUtility", "probe");
  return IOService::probe(provider, score);
}

bool AirPortUtility::start(IOService* provider) {
  if (!IOService::start(provider)) {
    return false;
  }

  HN_KEXT_LOG("AirPortUtility", "start");
  setProperty("HN.Role", "WiFiNetworkStabilizer");
  setProperty("HN.Mode", "WatchdogRecovery");
  if (!setupWatchdog()) {
    HN_KEXT_LOG("AirPortUtility", "watchdog setup failed");
  }
  registerService();
  return true;
}

void AirPortUtility::stop(IOService* provider) {
  teardownWatchdog();
  HN_KEXT_LOG("AirPortUtility", "stop");
  IOService::stop(provider);
}

void AirPortUtility::watchdogAction(OSObject* owner, IOTimerEventSource*) {
  AirPortUtility* self = OSDynamicCast(AirPortUtility, owner);
  if (self != nullptr) {
    self->runWatchdog();
  }
}

bool AirPortUtility::setupWatchdog() {
  workLoop_ = IOWorkLoop::workLoop();
  if (workLoop_ == nullptr) {
    return false;
  }

  watchdogTimer_ =
      IOTimerEventSource::timerEventSource(this, &AirPortUtility::watchdogAction);
  if (watchdogTimer_ == nullptr) {
    workLoop_->release();
    workLoop_ = nullptr;
    return false;
  }

  const IOReturn status = workLoop_->addEventSource(watchdogTimer_);
  if (status != kIOReturnSuccess) {
    watchdogTimer_->release();
    watchdogTimer_ = nullptr;
    workLoop_->release();
    workLoop_ = nullptr;
    return false;
  }

  scheduleWatchdog();
  HN_KEXT_LOG("AirPortUtility", "watchdog armed");
  return true;
}

void AirPortUtility::teardownWatchdog() {
  if (watchdogTimer_ != nullptr) {
    watchdogTimer_->cancelTimeout();
    if (workLoop_ != nullptr) {
      workLoop_->removeEventSource(watchdogTimer_);
    }
    watchdogTimer_->release();
    watchdogTimer_ = nullptr;
  }

  if (workLoop_ != nullptr) {
    workLoop_->release();
    workLoop_ = nullptr;
  }
}

void AirPortUtility::scheduleWatchdog() {
  if (watchdogTimer_ != nullptr) {
    watchdogTimer_->setTimeoutMS(kWatchdogIntervalMs);
  }
}

void AirPortUtility::runWatchdog() {
  const bool wifiReady = isAnyServiceAvailable(
      kWifiClasses, sizeof(kWifiClasses) / sizeof(kWifiClasses[0]), kMatchTimeoutNs);
  const bool networkReady = isAnyServiceAvailable(
      kNetworkClasses, sizeof(kNetworkClasses) / sizeof(kNetworkClasses[0]),
      kMatchTimeoutNs);

  if (wifiReady && networkReady) {
    if (consecutiveUnstable_ > 0) {
      IOLog("[Hackintosh-And-Beyond][AirPortUtility] recovered after %u degraded "
            "checks\n",
            consecutiveUnstable_);
    }
    consecutiveUnstable_ = 0;
    setProperty("HN.State", "Stable");
  } else {
    ++consecutiveUnstable_;
    setProperty("HN.State", "Degraded");
    IOLog("[Hackintosh-And-Beyond][AirPortUtility] degraded check wifi=%d "
          "network=%d streak=%u\n",
          wifiReady ? 1 : 0, networkReady ? 1 : 0, consecutiveUnstable_);
    if (consecutiveUnstable_ >= kRecoveryThreshold) {
      triggerRecovery();
      consecutiveUnstable_ = 0;
    }
  }

  scheduleWatchdog();
}

void AirPortUtility::triggerRecovery() {
  ++recoveryCount_;
  IOLog("[Hackintosh-And-Beyond][AirPortUtility] triggering recovery cycle %u\n",
        recoveryCount_);
  setProperty("HN.State", "Recovery");

  const char* targetClasses[] = {
      "IO80211Controller",
      "AirPort_BrcmNIC",
      "itlwm"
  };

  for (UInt32 i = 0; i < sizeof(targetClasses) / sizeof(targetClasses[0]); ++i) {
    OSDictionary* matching = IOService::serviceMatching(targetClasses[i]);
    if (matching == nullptr) continue;

    OSIterator* iterator = IOService::getMatchingServices(matching); // consumes dictionary
    if (iterator != nullptr) {
      OSObject* obj = nullptr;
      while ((obj = iterator->getNextObject()) != nullptr) {
        IOService* service = OSDynamicCast(IOService, obj);
        if (service != nullptr) {
          IOService* provider = service->getProvider();
          IOLog("[Hackintosh-And-Beyond][AirPortUtility] terminating -> %s\n", service->getName());
          bool term = service->terminate(kIOServiceSynchronous);
          if (term && provider != nullptr) {
            IOLog("[Hackintosh-And-Beyond][AirPortUtility] reprobing -> %s\n", provider->getName());
            provider->registerService();
          }
        }
      }
      iterator->release();
    }
  }

  registerService();
}

bool AirPortUtility::isAnyServiceAvailable(const char* const* classNames,
                                           UInt32 count,
                                           uint64_t timeoutNs) {
  for (UInt32 i = 0; i < count; ++i) {
    OSDictionary* matching = IOService::serviceMatching(classNames[i]);
    if (matching == nullptr) {
      continue;
    }

    IOService* service = IOService::waitForMatchingService(matching, timeoutNs);
    // matching is consumed by waitForMatchingService, do NOT release it
    if (service != nullptr) {
      service->release();
      return true;
    }
  }

  return false;
}
