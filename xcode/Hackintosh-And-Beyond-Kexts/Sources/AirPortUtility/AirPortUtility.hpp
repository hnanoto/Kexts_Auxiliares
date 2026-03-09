#ifndef AIRPORT_UTILITY_HPP
#define AIRPORT_UTILITY_HPP

#include <IOKit/IOService.h>

class AirPortUtility : public IOService {
  OSDeclareDefaultStructors(AirPortUtility)

public:
  bool init(OSDictionary* dictionary = nullptr) override;
  void free(void) override;
  IOService* probe(IOService* provider, SInt32* score) override;
  bool start(IOService* provider) override;
  void stop(IOService* provider) override;
};

#endif
