//
// Support for BH1750FVI ambient light sensor
//
//

#include "Arduino.h"
#include <Wire.h>
#include "BH1750.h"

BH1750::BH1750()
  : _addr(0)
{
}

int BH1750::begin(uint8_t addr)
{
  _addr = addr;
  return 0;
}

int BH1750::read(int *pLevel)
{
  if (_addr == 0)
    return BH1750_ERR_NOT_INITIALISED;

  Wire.beginTransmission(_addr);
  Wire.write(BH1750_ONE_TIME_H_RES_MODE);
  Wire.endTransmission();
  Wire.requestFrom(_addr, (uint8_t) 2);
  if (Wire.available() >= 2) {
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    if (pLevel)
      *pLevel = (msb << 8) | lsb;
    return 0;
  } else {
    return BH1750_ERR_NO_DATA;
  }
}


