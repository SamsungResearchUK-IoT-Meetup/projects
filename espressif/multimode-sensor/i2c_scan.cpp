// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
//
// February 18, 2019, nedw
//    Return devices found in an array passed in as argument

//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//


#include "i2c_scan.h"
#include <Wire.h>

//
// Scan I2C bus and return a list of device addresses found in out variable "devices", which
// has maximum length defined by in variable "devices_size".
//

int i2c_scan(i2c_addr_t *devices, int devices_size)
{
  byte error, address;
  int nDevices;

  if (devices && devices_size <= 0)
    return -1;
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0) {
      if (devices) {
        devices[nDevices++] = address;
        if (nDevices >= devices_size)
          return nDevices;
      }
    }
    else
    if (error == 4) {
      Serial.printf("i2c_scan: unknown error at address 0x%x", address);
    }    
  }
  return nDevices;
}

