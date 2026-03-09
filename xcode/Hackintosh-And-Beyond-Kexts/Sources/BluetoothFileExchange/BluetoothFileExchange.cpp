#include "BluetoothFileExchange.hpp"
#include "../Common/KextLog.hpp"
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IOWorkLoop.h>

namespace {
static constexpr uint64_t kMatchTimeoutNs = 1000000ULL;  // 1 ms

static const char* kBluetoothControllerClasses[] = {
    "IOBluetoothHCIController",
    "IOBluetoothHostController",
    "IOBluetoothFamily",
};

static const char* kBluetoothTransportClasses[] = {
    "IOBluetoothHostControllerUSBTransport",
    "IOBluetoothHostControllerUARTTransport",
    "IOBluetoothHCIController",
};
}  // namespace

OSDefineMetaClassAndStructors(BluetoothFileExchange, IOService)

bool BluetoothFileExchange::init(OSDictionary* dictionary) {
  const bool ok = IOService::init(dictionary);
  if (ok) {
    workLoop_ = nullptr;
    watchdogTimer_ = nullptr;
    consecutiveUnstable_ = 0;
    recoveryCount_ = 0;
    HN_KEXT_LOG("BluetoothFileExchange", "init");
  }
  return ok;
}

void BluetoothFileExchange::free(void) {
  teardownWatchdog();
  HN_KEXT_LOG("BluetoothFileExchange", "free");
  IOService::free();
}

IOService* BluetoothFileExchange::probe(IOService* provider, SInt32* score) {
  HN_KEXT_LOG("BluetoothFileExchange", "probe");
  return IOService::probe(provider, score);
}

bool BluetoothFileExchange::start(IOService* provider) {
  if (!IOService::start(provider)) {
    return false;
  }

  HN_KEXT_LOG("BluetoothFileExchange", "start");
  setProperty("HN.Role", "BluetoothStabilizer");
  setProperty("HN.Mode", "WatchdogRecovery");
  if (!setupWatchdog()) {
    HN_KEXT_LOG("BluetoothFileExchange", "watchdog setup failed");
  }
  registerService();
  return true;
}

void BluetoothFileExchange::stop(IOService* provider) {
  teardownWatchdog();
  HN_KEXT_LOG("BluetoothFileExchange", "stop");
  IOService::stop(provider);
}

void BluetoothFileExchange::watchdogAction(OSObject* owner, IOTimerEventSource*) {
  BluetoothFileExchange* self = OSDynamicCast(BluetoothFileExchange, owner);
  if (self != nullptr) {
    self->runWatchdog();
  }
}

bool BluetoothFileExchange::setupWatchdog() {
  workLoop_ = IOWorkLoop::workLoop();
  if (workLoop_ == nullptr) {
    return false;
  }

  watchdogTimer_ = IOTimerEventSource::timerEventSource(
      this, &BluetoothFileExchange::watchdogAction);
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
  HN_KEXT_LOG("BluetoothFileExchange", "watchdog armed");
  return true;
}

void BluetoothFileExchange::teardownWatchdog() {
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

void BluetoothFileExchange::scheduleWatchdog() {
  if (watchdogTimer_ != nullptr) {
    watchdogTimer_->setTimeoutMS(kWatchdogIntervalMs);
  }
}

void BluetoothFileExchange::runWatchdog() {
  const bool controllerReady = isAnyServiceAvailable(
      kBluetoothControllerClasses,
      sizeof(kBluetoothControllerClasses) / sizeof(kBluetoothControllerClasses[0]),
      kMatchTimeoutNs);
  const bool transportReady = isAnyServiceAvailable(
      kBluetoothTransportClasses,
      sizeof(kBluetoothTransportClasses) / sizeof(kBluetoothTransportClasses[0]),
      kMatchTimeoutNs);

  if (controllerReady && transportReady) {
    if (consecutiveUnstable_ > 0) {
      IOLog("[Hackintosh-And-Beyond][BluetoothFileExchange] recovered after %u "
            "degraded checks\n",
            consecutiveUnstable_);
    }
    consecutiveUnstable_ = 0;
    setProperty("HN.State", "Stable");
  } else {
    ++consecutiveUnstable_;
    setProperty("HN.State", "Degraded");
    IOLog("[Hackintosh-And-Beyond][BluetoothFileExchange] degraded check "
          "controller=%d transport=%d streak=%u\n",
          controllerReady ? 1 : 0, transportReady ? 1 : 0, consecutiveUnstable_);
    if (consecutiveUnstable_ >= kRecoveryThreshold) {
      triggerRecovery();
      consecutiveUnstable_ = 0;
    }
  }

  scheduleWatchdog();
}

void BluetoothFileExchange::triggerRecovery() {
  ++recoveryCount_;
  IOLog("[Hackintosh-And-Beyond][BluetoothFileExchange] triggering recovery "
        "cycle %u\n",
        recoveryCount_);
  setProperty("HN.State", "Recovery");
  registerService();
}

bool BluetoothFileExchange::isAnyServiceAvailable(const char* const* classNames,
                                                  UInt32 count,
                                                  uint64_t timeoutNs) {
  for (UInt32 i = 0; i < count; ++i) {
    OSDictionary* matching = IOService::serviceMatching(classNames[i]);
    if (matching == nullptr) {
      continue;
    }

    IOService* service = IOService::waitForMatchingService(matching, timeoutNs);
    matching->release();
    if (service != nullptr) {
      service->release();
      return true;
    }
  }

  return false;
}
