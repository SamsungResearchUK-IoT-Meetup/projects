#include "webthing.h"

WebThingAdapter* adapter;

//
// Define Webthing sensor device and properties
//

const char* sensorTypes[] = {"MultiLevelSensor", nullptr};
ThingDevice sensor("sensor", "Sensor Platform", sensorTypes);

ThingProperty temp1("temp1", "Temperature 1", NUMBER, "LevelProperty");
ThingProperty temp2("temp2", "Temperature 2", NUMBER, "LevelProperty");
ThingProperty light1("light1", "The level of light 1", NUMBER, "LevelProperty");
ThingProperty light2("light2", "The level of light 2", NUMBER, "LevelProperty");
ThingProperty humidity("humidity", "The level of humidity", NUMBER, "LevelProperty");
ThingProperty oled_brightness("oled_brightness", "OLED Brightness", NUMBER, "LevelProperty");

//
// Initialise Webthing adapter and sensor device.
//

void init_webthing(int brightness)
{
  adapter = new WebThingAdapter("sensors", WiFi.localIP());

  sensor.addProperty(&temp1);
  sensor.addProperty(&temp2);
  sensor.addProperty(&light1);
  sensor.addProperty(&light2);
  sensor.addProperty(&humidity);

  ThingPropertyValue value;
  value.number = brightness;
  oled_brightness.setValue(value);
  sensor.addProperty(&oled_brightness);

  adapter->addDevice(&sensor);

  adapter->begin();
  Serial.println("Webthing adapter started");
}

//
// Update Webthing properties from sensor readings
//

void update_webthing(int light, float tempC, float temp, float hum, int adc)
{
  ThingPropertyValue value;
  
  value.number = light;
  light1.setValue(value);

  value.number = tempC;
  temp1.setValue(value);

  value.number = temp;
  temp2.setValue(value);

  value.number = hum;
  humidity.setValue(value);

  value.number = adc;
  light2.setValue(value);
}

//
// Retrieve OLED brightness property
//

int get_oled_brightness()
{
  ThingPropertyValue value = oled_brightness.getValue();
  return value.number;
}

