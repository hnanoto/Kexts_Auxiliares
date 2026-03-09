#include "BluetoothFileExchange.hpp"
#include "../Common/KextLog.hpp"

OSDefineMetaClassAndStructors(BluetoothFileExchange, IOService)

bool BluetoothFileExchange::init(OSDictionary* dictionary) {
  const bool ok = IOService::init(dictionary);
  if (ok) {
    HN_KEXT_LOG("BluetoothFileExchange", "init");
  }
  return ok;
}

void BluetoothFileExchange::free(void) {
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
  registerService();
  return true;
}

void BluetoothFileExchange::stop(IOService* provider) {
  HN_KEXT_LOG("BluetoothFileExchange", "stop");
  IOService::stop(provider);
}
