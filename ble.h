#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// f5ceb02e-0b48-47de-8f2e-d9b7b3e4e0c7
// 6620c22a-67fc-4751-a148-fcdd1fb44462
// 201f06b1-758b-4a78-ad6b-b0e2d34a18d5
// 646d3a39-085d-43af-8180-813497e094c3
// 5674d1a7-32f7-40b3-92b5-7565190fb399

#define SERVICE_UUID           "f5ceb02e-0b48-47de-8f2e-d9b7b3e4e0c7"
#define CHARACTERISTIC_UUID    "6620c22a-67fc-4751-a148-fcdd1fb44462"

extern void init_ble(void);
extern void set_ble(const char *s);

