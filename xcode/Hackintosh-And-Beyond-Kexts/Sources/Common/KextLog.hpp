#ifndef HN_KEXT_LOG_HPP
#define HN_KEXT_LOG_HPP

#include <IOKit/IOLib.h>

#define HN_KEXT_LOG(component, message) \
  IOLog("[Hackintosh-And-Beyond][%s] %s\n", component, message)

#endif
