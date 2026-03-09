#ifndef BLUETOOTH_FILE_EXCHANGE_HPP
#define BLUETOOTH_FILE_EXCHANGE_HPP

#include <IOKit/IOService.h>

class IOTimerEventSource;
class IOWorkLoop;

class BluetoothFileExchange : public IOService {
  OSDeclareDefaultStructors(BluetoothFileExchange)

public:
  bool init(OSDictionary* dictionary = nullptr) override;
  void free(void) override;
  IOService* probe(IOService* provider, SInt32* score) override;
  bool start(IOService* provider) override;
  void stop(IOService* provider) override;

private:
  static constexpr UInt32 kWatchdogIntervalMs = 15000;
  static constexpr UInt32 kRecoveryThreshold = 3;

  IOWorkLoop* workLoop_;
  IOTimerEventSource* watchdogTimer_;
  UInt32 consecutiveUnstable_;
  UInt32 recoveryCount_;

  static void watchdogAction(OSObject* owner, IOTimerEventSource* sender);

  bool setupWatchdog();
  void teardownWatchdog();
  void scheduleWatchdog();
  void runWatchdog();
  void triggerRecovery();

  static bool isAnyServiceAvailable(const char* const* classNames,
                                    UInt32 count,
                                    uint64_t timeoutNs);
};

#endif
