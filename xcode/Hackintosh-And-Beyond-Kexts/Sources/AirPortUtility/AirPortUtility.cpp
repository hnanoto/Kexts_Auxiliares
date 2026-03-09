#include "AirPortUtility.hpp"
#include "../Common/KextLog.hpp"

OSDefineMetaClassAndStructors(AirPortUtility, IOService)

bool AirPortUtility::init(OSDictionary* dictionary) {
  const bool ok = IOService::init(dictionary);
  if (ok) {
    HN_KEXT_LOG("AirPortUtility", "init");
  }
  return ok;
}

void AirPortUtility::free(void) {
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
  registerService();
  return true;
}

void AirPortUtility::stop(IOService* provider) {
  HN_KEXT_LOG("AirPortUtility", "stop");
  IOService::stop(provider);
}
