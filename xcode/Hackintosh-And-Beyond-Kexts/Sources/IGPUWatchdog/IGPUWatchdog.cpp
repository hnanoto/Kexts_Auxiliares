#include "IGPUWatchdog.hpp"
#include "../Common/KextLog.hpp"
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IOWorkLoop.h>

namespace {
static constexpr uint64_t kMatchTimeoutNs = 1000000ULL;  // 1 ms

static const char* kIGPUControllerClasses[] = {
    "IOGraphicsAccelerator2",
    "IntelAccelerator",
    "AMDRadeonX4000_AMDRadeonController",
    "AppleIntelFramebuffer"
};
}  // namespace

OSDefineMetaClassAndStructors(IGPUWatchdog, IOService)

bool IGPUWatchdog::init(OSDictionary* dictionary) {
  const bool ok = IOService::init(dictionary);
  if (ok) {
    workLoop_ = nullptr;
    watchdogTimer_ = nullptr;
    consecutiveUnstable_ = 0;
    recoveryCount_ = 0;
    HN_KEXT_LOG("IGPUWatchdog", "init");
  }
  return ok;
}

void IGPUWatchdog::free(void) {
  teardownWatchdog();
  HN_KEXT_LOG("IGPUWatchdog", "free");
  IOService::free();
}

IOService* IGPUWatchdog::probe(IOService* provider, SInt32* score) {
  HN_KEXT_LOG("IGPUWatchdog", "probe");
  return IOService::probe(provider, score);
}

bool IGPUWatchdog::start(IOService* provider) {
  if (!IOService::start(provider)) {
    return false;
  }

  HN_KEXT_LOG("IGPUWatchdog", "start");
  setProperty("HN.Role", "USBWatchdog");
  setProperty("HN.Mode", "WatchdogRecovery");
  if (!setupWatchdog()) {
    HN_KEXT_LOG("IGPUWatchdog", "watchdog setup failed");
  }
  registerService();
  return true;
}

void IGPUWatchdog::stop(IOService* provider) {
  teardownWatchdog();
  HN_KEXT_LOG("IGPUWatchdog", "stop");
  IOService::stop(provider);
}

void IGPUWatchdog::watchdogAction(OSObject* owner, IOTimerEventSource*) {
  IGPUWatchdog* self = OSDynamicCast(IGPUWatchdog, owner);
  if (self != nullptr) {
    self->runWatchdog();
  }
}

bool IGPUWatchdog::setupWatchdog() {
  workLoop_ = IOWorkLoop::workLoop();
  if (workLoop_ == nullptr) {
    return false;
  }

  watchdogTimer_ = IOTimerEventSource::timerEventSource(
      this, &IGPUWatchdog::watchdogAction);
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
  HN_KEXT_LOG("IGPUWatchdog", "watchdog armed");
  return true;
}

void IGPUWatchdog::teardownWatchdog() {
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

void IGPUWatchdog::scheduleWatchdog() {
  if (watchdogTimer_ != nullptr) {
    watchdogTimer_->setTimeoutMS(kWatchdogIntervalMs);
  }
}

void IGPUWatchdog::runWatchdog() {
  const bool controllerReady = isAnyServiceAvailable(
      kIGPUControllerClasses,
      sizeof(kIGPUControllerClasses) / sizeof(kIGPUControllerClasses[0]),
      kMatchTimeoutNs);

  if (controllerReady) {
    if (consecutiveUnstable_ > 0) {
      IOLog("[Hackintosh-And-Beyond][IGPUWatchdog] recovered after %u "
            "degraded checks\n",
            consecutiveUnstable_);
    }
    consecutiveUnstable_ = 0;
    setProperty("HN.State", "Stable");
  } else {
    ++consecutiveUnstable_;
    setProperty("HN.State", "Degraded");
    IOLog("[Hackintosh-And-Beyond][IGPUWatchdog] degraded check "
          "controller=%d streak=%u\n",
          controllerReady ? 1 : 0, consecutiveUnstable_);
    if (consecutiveUnstable_ >= kRecoveryThreshold) {
      triggerRecovery();
      consecutiveUnstable_ = 0;
    }
  }

  scheduleWatchdog();
}

void IGPUWatchdog::triggerRecovery() {
  ++recoveryCount_;
  IOLog("[Hackintosh-And-Beyond][IGPUWatchdog] triggering recovery cycle %u\n",
        recoveryCount_);
  setProperty("HN.State", "Recovery");

  const char* targetClasses[] = {
      "IOGraphicsAccelerator2",
      "IntelAccelerator"
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
          IOLog("[Hackintosh-And-Beyond][IGPUWatchdog] terminating -> %s\n", service->getName());
          bool term = service->terminate(kIOServiceSynchronous);
          if (term && provider != nullptr) {
            IOLog("[Hackintosh-And-Beyond][IGPUWatchdog] reprobing -> %s\n", provider->getName());
            provider->registerService();
          }
        }
      }
      iterator->release();
    }
  }

  registerService();
}

bool IGPUWatchdog::isAnyServiceAvailable(const char* const* classNames,
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
