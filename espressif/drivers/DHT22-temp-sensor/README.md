# Arduino ESP8266 driver for DHT22 temperature and humidity sensor

A simple driver for the DHT22.

To use:
1. Create an instance of the ```DHT22``` class
2. In ```setup()```, call the ```begin()``` class method with the GPIO pin number to which the DHT22 is attached as argument
3. In ```loop()```, call the ```read()``` class method with two ```float``` arguments, one each for temperature and humidity.  A return value of 0 indicates a successful measurement, and a negative value indicates an error.

Example:
```
#define DHT22_GPIO_PIN      14      // (esp8266 "D5")

DHT22 dht;

void setup()
{
    dht.begin(DHT22_GPIO_PIN);
}

void loop()
{
    float temp, humidity;
    if (dht.read(&temp, &humidity) == 0) {
        // Success
    } else {
        // Error
    }
    ...
}
```

NOTE:
* the above example uses a global instance of the ```DHT22``` class - you can also use ```new DHT22()``` if required.
* this driver was developed for the DHT22 and not the DHT11.


