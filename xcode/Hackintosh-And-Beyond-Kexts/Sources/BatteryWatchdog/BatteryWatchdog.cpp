#include "BatteryWatchdog.hpp"
#include "../Common/KextLog.hpp"
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IOWorkLoop.h>

namespace {
static constexpr uint64_t kMatchTimeoutNs = 1000000ULL;  // 1 ms

static const char* kBatteryControllerClasses[] = {
    "IOPMPowerSource",
    "SMCBatteryManager",
    "AppleSmartBatteryManager",
};
}  // namespace

OSDefineMetaClassAndStructors(BatteryWatchdog, IOService)

bool BatteryWatchdog::init(OSDictionary* dictionary) {
  const bool ok = IOService::init(dictionary);
  if (ok) {
    workLoop_ = nullptr;
    watchdogTimer_ = nullptr;
    consecutiveUnstable_ = 0;
    recoveryCount_ = 0;
    HN_KEXT_LOG("BatteryWatchdog", "init");
  }
  return ok;
}

void BatteryWatchdog::free(void) {
  teardownWatchdog();
  HN_KEXT_LOG("BatteryWatchdog", "free");
  IOService::free();
}

IOService* BatteryWatchdog::probe(IOService* provider, SInt32* score) {
  HN_KEXT_LOG("BatteryWatchdog", "probe");
  return IOService::probe(provider, score);
}

bool BatteryWatchdog::start(IOService* provider) {
  if (!IOService::start(provider)) {
    return false;
  }

  HN_KEXT_LOG("BatteryWatchdog", "start");
  setProperty("HN.Role", "BatteryWatchdog");
  setProperty("HN.Mode", "WatchdogRecovery");
  if (!setupWatchdog()) {
    HN_KEXT_LOG("BatteryWatchdog", "watchdog setup failed");
  }
  registerService();
  return true;
}

void BatteryWatchdog::stop(IOService* provider) {
  teardownWatchdog();
  HN_KEXT_LOG("BatteryWatchdog", "stop");
  IOService::stop(provider);
}

void BatteryWatchdog::watchdogAction(OSObject* owner, IOTimerEventSource*) {
  BatteryWatchdog* self = OSDynamicCast(BatteryWatchdog, owner);
  if (self != nullptr) {
    self->runWatchdog();
  }
}

bool BatteryWatchdog::setupWatchdog() {
  workLoop_ = IOWorkLoop::workLoop();
  if (workLoop_ == nullptr) {
    return false;
  }

  watchdogTimer_ = IOTimerEventSource::timerEventSource(
      this, &BatteryWatchdog::watchdogAction);
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
  HN_KEXT_LOG("BatteryWatchdog", "watchdog armed");
  return true;
}

void BatteryWatchdog::teardownWatchdog() {
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

void BatteryWatchdog::scheduleWatchdog() {
  if (watchdogTimer_ != nullptr) {
    watchdogTimer_->setTimeoutMS(kWatchdogIntervalMs);
  }
}

void BatteryWatchdog::runWatchdog() {
  const bool controllerReady = isAnyServiceAvailable(
      kBatteryControllerClasses,
      sizeof(kBatteryControllerClasses) / sizeof(kBatteryControllerClasses[0]),
      kMatchTimeoutNs);

  if (controllerReady) {
    if (consecutiveUnstable_ > 0) {
      IOLog("[Hackintosh-And-Beyond][BatteryWatchdog] recovered after %u "
            "degraded checks\n",
            consecutiveUnstable_);
    }
    consecutiveUnstable_ = 0;
    setProperty("HN.State", "Stable");
  } else {
    ++consecutiveUnstable_;
    setProperty("HN.State", "Degraded");
    IOLog("[Hackintosh-And-Beyond][BatteryWatchdog] degraded check "
          "controller=%d streak=%u\n",
          controllerReady ? 1 : 0, consecutiveUnstable_);
    if (consecutiveUnstable_ >= kRecoveryThreshold) {
      triggerRecovery();
      consecutiveUnstable_ = 0;
    }
  }

  scheduleWatchdog();
}

void BatteryWatchdog::triggerRecovery() {
  ++recoveryCount_;
  IOLog("[Hackintosh-And-Beyond][BatteryWatchdog] triggering recovery cycle %u\n",
        recoveryCount_);
  setProperty("HN.State", "Recovery");

  const char* targetClasses[] = {
      "SMCBatteryManager",
      "AppleSmartBatteryManager"
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
          IOLog("[Hackintosh-And-Beyond][BatteryWatchdog] terminating -> %s\n", service->getName());
          bool term = service->terminate(kIOServiceSynchronous);
          if (term && provider != nullptr) {
            IOLog("[Hackintosh-And-Beyond][BatteryWatchdog] reprobing -> %s\n", provider->getName());
            provider->registerService();
          }
        }
      }
      iterator->release();
    }
  }

  registerService();
}

bool BatteryWatchdog::isAnyServiceAvailable(const char* const* classNames,
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
