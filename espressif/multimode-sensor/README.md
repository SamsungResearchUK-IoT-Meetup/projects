# ESP8266/ESP32 Sensor Station Project
## 1. Introduction
This project implements a sensor station built from the following components:

* ESP8266 or ESP32
* DHT22 Temperature/Humidity Sensor (GPIO)
* BH1750 Light Level Sensor (I2C)
* BMP280 Temperature/Atmospheric Pressure Sensor (I2C)
* Simple Photocell Light Level Sensor (ADC)
* SSD1306 OLED Display (I2C)

The sensor readings are taken every two seconds and are made available to the user in the following ways:

* On the OLED display
* As a set of Mozilla WebThing properties over HTTP
* As a Bluetooth LE Characteristic (ESP32)
* Via the USB serial port (for debugging)

## 2. Getting the Project Running

To get the project running in your environment, you need to make two changes:
1. Set up WiFi Credentials
2. Set up GPIO/I2C/ADC pins

_WiFi Credentials_

The WiFi credentials need to be stored in a file called `wifi_credentials.h` with the following template:

```
const struct wifi_credentials {
    const char * const ssid;
    const char * const pass;
} wifi_credentials[] = {
    { "<SSID1>", "<password1>" },
    { "<SSID2>", "<password2>" }
    ...etc...
}
```
You needn't include multiple SSIDs - simply have one line per SSID you are interested in connecting to.

_GPIO/I2C/ADC pins_

The pin configuration for these is defined in `sensors.h` for both ESP8266 and ESP32.  The following is an extract from the file showing the default settings that you may have to change:

```
#ifdef ESP32
#define ANALOG_CHANNEL      A0
#define MAX_ANALOG_VALUE    4096
#define DHT22_GPIO_PIN      32
#define TOUCH_CHANNEL       T7
#else 
#define ANALOG_CHANNEL      0
#define MAX_ANALOG_VALUE    1024
#define DHT22_GPIO_PIN      14
#endif
//
// I2C definitions
//

#ifdef ESP32
#define SDA_PIN             21               // I2C data pin
#define SCL_PIN             22               // I2C clock pin
#else
#define SDA_PIN             5               // I2C data pin
#define SCL_PIN             4               // I2C clock pin
#endif
```
The `TOUCH_CHANNEL` setting is an experimental pin whose value is also presented alongside that of the other sensor readings.  In the project as it stands, the pin is just connected to a jumper lead for experiments with touch input.

## 3. Dependencies
The project requires a number of additional Arduino libraries.  These should be cloned from GitHub in the `libraries/` directory.  The following is a list of  dependencies required:

| Dependency | GitHub Repo |
| - | - |
DHT22 driver | `git@github.com:nedw/esp8266-DHT22.git`
BH1750 driver | `git@github.com:nedw/esp8266-BH1750.git`
SSD1306 OED driver | `git@github.com:ThingPulse/esp8266-oled-ssd1306.git`
BMP280 driver | `https://github.com/adafruit/Adafruit_BMP280_Library.git`
Mozilla WebThings | `https://github.com/nedw/webthing-arduino.git`

In addition, the  Mozilla WebThings library has the following dependencies, which will also need to be added:

| Dependency | GitHub Repo |
| - | - |
Arduino JSON | `https://github.com/bblanchon/ArduinoJson.git`
ESP Async Webserver | `https://github.com/me-no-dev/ESPAsyncWebServer`
ESP Async TCP | `https://github.com/me-no-dev/ESPAsyncTCP`
Async TCP (for ESP32) | `https://github.com/me-no-dev/AsyncTCP`


Finally, have fun!

