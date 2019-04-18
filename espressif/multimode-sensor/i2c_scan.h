#include <Arduino.h>

using i2c_addr_t = byte;

int i2c_scan(i2c_addr_t *devices, int devices_size);