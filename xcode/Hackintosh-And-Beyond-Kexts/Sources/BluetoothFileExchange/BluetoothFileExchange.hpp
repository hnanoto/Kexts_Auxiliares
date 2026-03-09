#ifndef BLUETOOTH_FILE_EXCHANGE_HPP
#define BLUETOOTH_FILE_EXCHANGE_HPP

#include <IOKit/IOService.h>

class BluetoothFileExchange : public IOService {
  OSDeclareDefaultStructors(BluetoothFileExchange)

public:
  bool init(OSDictionary* dictionary = nullptr) override;
  void free(void) override;
  IOService* probe(IOService* provider, SInt32* score) override;
  bool start(IOService* provider) override;
  void stop(IOService* provider) override;
};

#endif
