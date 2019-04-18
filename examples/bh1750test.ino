#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>

#define UPDATE_PERIOD_MS    (2 * 1000)

#define BH1750_ADDR   0x23  // Default I2C address
#define SDA_PIN       5     // I2C data pin - GPIO 5 (ESP8266 "D1")
#define SCL_PIN       4     // I2C clock pin - GPIO 4 (ESP8266 "D2")

BH1750 bh1750;

void setup()
{
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Started...");
  delay(1000);
  bh1750.begin(BH1750_ADDR);
}

void loop()
{
  static unsigned long next_update_millis = 0;

  unsigned long now = millis();
  if (now < next_update_millis)
    return;

  next_update_millis = now + UPDATE_PERIOD_MS;

  int lx;
  int ret = bh1750.read(&lx);
  if (ret >= 0)
    Serial.printf("Light level = %d lx\n", lx);
  else
    Serial.printf("Error %d reading BH1750 light level\n", ret);
}


