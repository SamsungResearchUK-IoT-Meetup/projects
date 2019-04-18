
#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_BMP280.h>
#include <DHT22.h>
#include "sensors.h"


BH1750 *BH1750p           = nullptr;
Adafruit_BMP280 *BMP280p  = nullptr;
DHT22 *DHT22p             = nullptr;

//
// DHT22 driver interface routines
//

void init_DHT22()
{
  DHT22p = new DHT22();
  DHT22p->begin(DHT22_GPIO_PIN);        // ensure pin is set to INPUT in quiescent state
}

int read_DHT22(float* temp_p, float* hum_p)
{
  int ret = 0;

  if (DHT22p) {
    float temp, hum;
    ret = DHT22p->read(&temp, &hum);
    if (temp_p)
      *temp_p = temp;
    if (hum_p)
      *hum_p = hum;
  }
  return ret;
}

//
// BH1750 driver interface routines
//

void init_BH1750()
{
  BH1750p = new BH1750();                               
  BH1750p->begin(BH1750_ADDR);
}

int read_BH1750(int& lux)
{
  int ret = 0;

  if (BH1750p) {
    ret = BH1750p->read(&lux);
  } else {
    ret = BH1750_ERR_NOT_INITIALISED;
  }
  return ret;
}

//
// BMP280 driver interface routines
//

void init_BMP280()
{
  BMP280p = new Adafruit_BMP280();
  if (!BMP280p->begin(BMP280_ADDRESS_ALT)) {
    Serial.println("Error initialising BMP280");
    delete BMP280p;
    BMP280p = nullptr;
  } else {
    BMP280p->setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  }
}

int read_BMP280(float* temp_p, float* pres_p, float* alt_p)
{
  if (BMP280p) {
    if (temp_p)
      *temp_p = BMP280p->readTemperature();
    if (pres_p)
      *pres_p = BMP280p->readPressure();
    if (alt_p)
      *alt_p = BMP280p->readAltitude(MEAN_SEA_LEVEL_PRESSURE);
    return 0;
  } else {
    return -1;
  }
}



