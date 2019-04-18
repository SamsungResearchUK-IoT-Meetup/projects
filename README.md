# Arduino ESP8266 driver for BH1750 light sensor

To use:

1. Create an instance of the ```BH1750``` class
2. In ```setup()```:
  * initialise the ```Wire``` library (for I2C)
  * call the ```begin()``` method of the ```BH1750``` instance with the I2C address of the sensor
3. In ```loop()```, call the ```read()``` method with the address of an ```int``` variable as argument.  A return
   value of zero indicates a successful read, with the light level in the variable passed as argument.  A negative
   return value indicates failure.

Example:
```
#define BH1750_ADDR   0x23  // Default I2C address
#define SDA_PIN       5     // I2C data pin - GPIO 5 (ESP8266 "D1")
#define SCL_PIN       4     // I2C clock pin - GPIO 4 (ESP8266 "D2")

BH1750 bh1750;

void setup()
{
  Wire.begin(SDA_PIN, SCL_PIN);
  bh1750.begin(BH1750_ADDR);
}

void loop()
{
  int level;
  int ret = bh1750.read(&level);
  if (ret >= 0) {
    // Light level in 'level'
  } else {
    // Error number in 'ret'
  }
}
```

NOTE:
* the above example uses a global instance of the ```BH1750``` class - you can also use ```new BH1750()``` if required.


