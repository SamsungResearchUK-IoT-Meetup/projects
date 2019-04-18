//
// Implementation of an ESP8266 Webthing sensor station that reports
// readings from the following sensors:
//
// - DHT22      - temperature/humidity (GPIO)
// - BH1750     - light level (I2C)
// - BMP280     - temperature/pressure (I2C)
// - Photocell  - light level (Analog)
//

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>

#include <Wire.h>

#include <BH1750.h>
#include <Adafruit_BMP280.h>
#include <DHT22.h>
#include <SSD1306Wire.h> // legacy include: `#include "SSD1306.h"`

#include "i2c_scan.h"
#include "webthing.h"
#include "sensors.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include "wifi_credentials.h"

#define ENABLE_BLE

#ifdef ENABLE_BLE
#include "ble.h"
#endif

//
// wifi_credentials.h should contain:
//
// const struct wifi_credentials {
//     const char * const ssid;
//     const char * const pass;
// } wifi_credentials[] = {
//     { "<ssid1>", "<pass1>" },
//     { "<ssid2>", "<pass2> "}
//     etc
// };
//

#define ARRAYSIZE(x)  (sizeof(x) / sizeof(x[0]))

#define UPDATE_PERIOD_MS    (2 * 1000)      // update period of sensors
#define OLED_BRIGHTNESS     10              // Initial OLED display brightness

#define I2C_MAX_DEVICES     5

//
// Declaration of sensor driver classes
//

extern BH1750 *BH1750p;
extern Adafruit_BMP280 *BMP280p;
extern DHT22 *DHT22p;
extern WebThingAdapter* adapter;

WiFiMulti wifiMulti;
bool wifi_active = false;
String localIP;
String SSID;
const char *ssid = NULL;

//
// Definition of structures used for I2C scanning
//

i2c_addr_t i2c_devices[I2C_MAX_DEVICES];
int i2c_devices_found = 0;

const i2c_addr_t required_devices[] = {
#ifdef ENABLE_BH1750
  BH1750_ADDR,              // 0x23
#endif
#ifdef ENABLE_BMP280
  BMP280_ADDRESS_ALT,       // 0x76
#endif
#ifdef ENABLE_SSD1306
  SSD1306_ADDR               // 0x3c
#endif
};
const int required_devices_count = ARRAYSIZE(required_devices);

#ifdef ENABLE_SSD1306
SSD1306Wire ssd1306(0x3c, SDA_PIN, SCL_PIN);

//
// SSD1306 Routines
//

void init_ssd1306()
{
  ssd1306.init();
  ssd1306.setFont(ArialMT_Plain_10);
}

#endif

void init_led()
{
#ifdef ESP8266
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif
}

//
// WiFi Initialisation
//

void init_wifi()
{
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);

  for (int i = 0 ; i < ARRAYSIZE(wifi_credentials) ; ++i) {
    wifiMulti.addAP(wifi_credentials[i].ssid, wifi_credentials[i].pass);
  }

  int count = 45 * 2;
  // Wait for connection
  while (count > 0 && wifiMulti.run() != WL_CONNECTED) {
    --count;
    delay(500);
    Serial.print(".");
  }

  if (count > 0) {
    wifi_active = true;
    SSID = WiFi.SSID();
    ssid = SSID.c_str();
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    localIP = WiFi.localIP().toString();
    Serial.printf("IP address: %s\n", localIP.c_str());
  } else {
    Serial.println("Not connected");
  }
}

//
// Main setup() routine
//

void setup()
{
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Started...");
  delay(1000);

  init_led();                         // turn on LED while initialising to WiFi
  init_wifi();
#ifdef ESP8266
  digitalWrite(LED_BUILTIN, HIGH);    // turn off LED once WiFi is established
#endif
  if (wifi_active)
    init_webthing(OLED_BRIGHTNESS);

#ifdef ENABLE_DHT22
  // Initialise DHT22 Temperature/Humidity sensor
  init_DHT22();
#endif
#ifdef ENABLE_SSD1306
  // Initialise SSD1306 OLED display
  init_ssd1306();                     
#endif
#ifdef ENABLE_BLE
  init_ble();
#endif
}

//
// Scan I2C bus for device addresses specified in i2c_devices[].
// Return true if all devices were found, else false.
//

bool find_devices()
{
  Serial.println("Scanning I2C bus...");
  i2c_devices_found = i2c_scan(i2c_devices, I2C_MAX_DEVICES);
  int devices_matched = 0;
  if (i2c_devices_found == 0) {
    // No I2C devices found
    Serial.println("No devices found");
  } else {
    // A number of I2C devices found
    Serial.printf("%d devices found: ", i2c_devices_found);
    for (int i = 0 ; i < i2c_devices_found ; ++i)
      Serial.printf("0x%x ", i2c_devices[i]);
    Serial.println("");

    // Check how many of the devices found match those specified in i2c_devices[]
    for (int r = 0 ; r < required_devices_count ; ++r) {
      i2c_addr_t required_device = required_devices[r];
      for (int i = 0 ; i < i2c_devices_found ; ++i) {
        if (i2c_devices[i] == required_device) {
          devices_matched++;
          break;
        }
      }

    }
  }

  if (devices_matched == required_devices_count) { // note required_devices_count can also be zero
    // All devices found
    Serial.println("All devices found");
    return true;
  } else {
    // No devices found
    return false;
  }
}

//
// Update SSD1306 display with sensor values and, if changed, new OLED brightness setting
//

#ifdef ESP32
void update_ssd1306(float tempC, float pres, float temp, float hum, int lux, int adc, int touch, int new_brightness)
#else
void update_ssd1306(float tempC, float pres, float temp, float hum, int lux, int adc, int new_brightness)
#endif
{
  static int current_brightness = -1;

#ifdef ENABLE_SSD1306
  ssd1306.clear();

  if (new_brightness != current_brightness) {
    ssd1306.setBrightness(new_brightness);
    current_brightness = new_brightness;
  }
  ssd1306.setTextAlignment(TEXT_ALIGN_LEFT);
  ssd1306.setFont(ArialMT_Plain_10);
  char buf[150];

  if (wifi_active) {
    snprintf(buf, sizeof(buf), "%s %s", localIP.c_str(), ssid);
    ssd1306.drawString(0, 0, buf);
  }

  snprintf(buf, sizeof(buf), "Temp: %.2f, P: %.2f", tempC, pres);
  ssd1306.drawString(0, 12, buf);

  snprintf(buf, sizeof(buf), "Temp: %.1f, Hum: %.1f%%", temp, hum);
  ssd1306.drawString(0, 24, buf);

  snprintf(buf, sizeof(buf), "Analog: %d, Lux: %d", adc, lux);
  ssd1306.drawString(0, 36, buf);

#ifdef ESP32
  snprintf(buf, sizeof(buf), "Touch: %d, Heap: %d", touch, ESP.getFreeHeap());
#else
  snprintf(buf, sizeof(buf), "Heap: %d", ESP.getFreeHeap());
#endif
  ssd1306.drawString(0, 48, buf);

  ssd1306.display();
#endif
}

#ifdef ENABLE_BLE
void update_ble(float tempC, float pres, float temp, float hum, int lux, int adc, int touch)
{
  char buf[256];

  //if (wifi_active) {
  //  snprintf(buf, sizeof(buf), "%s %s", localIP.c_str(), ssid);
  //}

  snprintf(buf, sizeof(buf),
            "Temp %.2f, Pres %.2f, Temp %.1f, Hum %.1f, Analog %d, Lux %d, Touch %d, Heap %d",
            tempC, pres, temp, hum, adc, lux, touch, ESP.getFreeHeap());
  set_ble(buf);
}
#endif

//
// Check if a sensor update is due.
// Return true if so, else false.
//

bool next_update_due()
{
  static unsigned long next_update_millis = 0;
  unsigned long now = millis();
  if (now < next_update_millis)
    return false;
  next_update_millis = now + UPDATE_PERIOD_MS;
  return true;
}

//
// Called from loop() to scan for I2C sensor devices.
// Returns true if all sensor devices have been found, else false.
//

bool i2c_init_complete()
{
  static int max_i2c_scan_count = 10;
  static bool all_devices_found = false;

  if (!all_devices_found && max_i2c_scan_count-- > 0) {
    all_devices_found = find_devices();
    if (!all_devices_found) {
      // If we didn't find all I2C sensor devices, delay and try again later.
      // Repeat this for max_i2c_scan_count times.
      delay(3000);
      return false;
    } else {
      // We have found all I2C sensor devices, so initialise them.
#ifdef ENABLE_BH1750
      init_BH1750();
#endif
#ifdef ENABLE_BMP280
      init_BMP280();
#endif
    }
  }
  return true;
}

//
// Main loop() routine
//

void loop()
{

  // Scan for I2C sensor devices
  if (!i2c_init_complete())
    return;

  // Call Webthing update routine
  if (wifi_active)
    adapter->update();

  // Return if we are not due for a sensor update
  if (!next_update_due())
    return;

  if (wifi_active)
    Serial.printf("IP = %s (%s)\n", localIP.c_str(), ssid);

  // Read BMP280 Temperature/Pressure sensor
  int ret;
#ifdef ENABLE_BMP280
  float tempC = 0, pres = 0;
  ret = read_BMP280(&tempC, &pres, nullptr);
  if (ret >= 0) {
    Serial.printf("Temp = %.2f C, Pres = %.2f Pa\n", tempC, pres);
  }
#endif

// Read DHT22 Temperature/Humidity sensor
#ifdef ENABLE_DHT22
  float temp = 0, hum = 0;
  ret = read_DHT22(&temp, &hum);
  if (ret >= 0) {
    Serial.printf("Temp = %.1f, Hum = %.1f\n", temp, hum);
  }
#endif

  // DHT22 can take a while as it's GPIO poll driven, so yield() just in case.
  yield();

  // Read BH1750 light sensor
#ifdef ENABLE_BH1750
  int lux = -1;
  ret = read_BH1750(lux);
#ifndef ENABLE_ANALOG
  if (ret >= 0)
    Serial.printf("Lux = %d\n", lux);
#endif
#endif

  int adc = -1;

  // Read analog input - connected to simple photocell sensor
#ifdef ENABLE_ANALOG
  adc = MAX_ANALOG_VALUE - analogRead(ANALOG_CHANNEL);
#ifdef ENABLE_BH1750
  Serial.printf("Analog = %d, Lux = %d\n", adc, lux);
#else
  Serial.printf("Analog = %d\n", adc);
#endif
#endif

  // Update Webthing properties with sensor readings
  if (wifi_active)
    update_webthing(lux, tempC, temp, hum, adc);

  // Update OLED display with sensor readings and (if required), brightness
  int brightness = get_oled_brightness();
#ifdef ESP32
  int touch = touchRead(TOUCH_CHANNEL);
  Serial.printf("Touch %d, Free heap %d\n", touch, ESP.getFreeHeap());
  update_ssd1306(tempC, pres, temp, hum, lux, adc, touch, brightness);
#ifdef ENABLE_BLE
  update_ble(tempC, pres, temp, hum, lux, adc, touch);
#endif
#else
  update_ssd1306(tempC, pres, temp, hum, lux, adc, brightness);
  Serial.printf("Free heap %d\n", ESP.getFreeHeap());
#endif

  Serial.println("");
  delay(2000);
}


