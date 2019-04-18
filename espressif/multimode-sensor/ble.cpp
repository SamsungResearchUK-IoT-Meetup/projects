#include "ble.h"

BLEServer *server = NULL;
BLEService *service = NULL;
BLECharacteristic *characteristic = NULL;

void init_ble()
{
  Serial.println("Initialising BLE");

  BLEDevice::init("ESP32 sensor");
  server = BLEDevice::createServer();
  service = server->createService(SERVICE_UUID);

  characteristic = service->createCharacteristic(
      CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);

  service->start();
  server->getAdvertising()->start();
  Serial.println("BLE initialised");
}

void set_ble(const char *s)
{
    characteristic->setValue(s);
}

