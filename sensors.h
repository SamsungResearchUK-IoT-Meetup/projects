
#define ENABLE_BMP280
#define ENABLE_BH1750
#define ENABLE_SSD1306
#define ENABLE_DHT22
#define ENABLE_ANALOG

#define MEAN_SEA_LEVEL_PRESSURE 1013.25

//
// GPIO/Analog definitions
//

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

#define SSD1306_ADDR        0x3c            // Default SSD1306 OLED display address

//
// Sensor initialisation and reading routines. 
//

extern void init_DHT22();
extern int read_DHT22(float* temp_p, float* hum_p);

extern void init_BH1750();
extern int read_BH1750(int& lux_p);

extern void init_BMP280();
extern int read_BMP280(float* temp_p, float* pres_p, float* alt_p);
