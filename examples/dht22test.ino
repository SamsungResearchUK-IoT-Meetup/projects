#include <Arduino.h>
#include <DHT22.h>

#define UPDATE_PERIOD_MS    (3 * 1000)

#define DHT22_GPIO_PIN      14    // ESP8266 

DHT22 dht;

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Started...");
  delay(1000);

   dht.begin(DHT22_GPIO_PIN);
}

void loop()
{
  static unsigned long next_update_millis = 0;

  unsigned long now = millis();
  if (now < next_update_millis)
    return;

  next_update_millis = now + UPDATE_PERIOD_MS;

  float temp, hum;
  int ret = dht.read(&temp, &hum);
  if (ret >= 0) {
    Serial.printf("Temperature = %.1f, Humidity = %.1f\n", temp, hum);
  }
}


