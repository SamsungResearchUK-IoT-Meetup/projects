# Tempearture and Humidity Sensor (HDC2080)

The HDC 2080 sensor documentation can be found [here](https://pybd.io/hw/tile_sensa.html). There is no PIN configuration since this device is connected via [I2C bus](https://i2c.info/) a 2 wire bus protocol.
The driver for the HDC 2080 is adapted from the micropython code, it includes some helper functions, hardening and statistics.

![hdc 2080 tile](https://github.com/SamsungResearchUK-IoT-Meetup/projects/blob/master/micropython/assets/TILE_SENSA_ds1.jpg)

## HDC Setup
The HDC sensor contains a temperature sensor and a humidity sensor combined. It has address number 64 on the bus.
The sensor is mounted via the tile header onto something like the [DIP 28](https://pybd.io/hw/wbus_dip28.html) or the [DIP 68](https://pybd.io/hw/wbus_dip68.html) i.e. it's modular and pluggable.

## Testing The Driver
The driver for the sensor is [here](https://github.com/SamsungResearchUK-IoT-Meetup/projects/blob/master/micropython/test-programs/tiles/hdc2080_sensor.py). The hdc2080_sensor.py file is really an enhancement to the basic micropython example code adding a few helper methods, some hardening and a few statistics. It allows you to create an object to get basic information from your Humidity and Temperature sensor. It provides methods for temperature, humidity, testing the sensor is ready and holding the max and min temperature values of values that are fetched from the board.

### Creating HDC2080 Sensor
Once you have copied the file onto the board, you can create a HDC object:

```python
/> rshell
/> repl
>>> import machine
>>> from hdc2080_sensor import HDC_Sensor
>>> i2c = machine.ID2C('X')
>>> tempHumidity = HDC_Sensor(i2c)
```
### Getting Statistics
To get statistics from the temperature and humidity sensor use methods **max_temperature()** and **min_temperature()** like this:

```python
>>> tempHumidity.max_temperature()
0
>>> tempHumidity.min_temperature()
0
```

### Detecting Current Temperature
The temperature and humidity sensor has two methods for detecting current temperature and humidity which are:
* temperature()
* humidity()
The respond with the temperature in degrees centigrade and humidity in percentage concentration of water vapour present in the air.
Examples are:

```python
>>> tempHumidity.temperature()
23.456
>>> tempHumidity.humidity()
41.431
```

### HDC2080 Object Attributes
The HDC2080 public Object Attributes can tell you the start time **start_time** of the HDC2080 object and used like this:
```python
>>> tempHumidity.start_time
611145275
```

### Checking HDC2080 Is Ready
The HDC2080 can be send a message to see if it's ready. Like a 'ping' in network terminanology. In other words it will return a 'True' response on the I2C bus if it's ready to recieve commands. To do this:

```python
>>> tempHumidity.is_ready()
True
```
### Errors From HDC2080
If the HDC2080 is not responding to messages, there is an electrical fault between the microcontroller and the tile sensor, or the firmware is not configured to use the I2C bus you will see an error generated on the repl like this:

```python
>>> tempHumidity.is_ready()
The I2C bus is not responding to the I2C device address of: 64
Error value: [Errno 19] ENODEV
False
>>>
>>> tempHumidity.temperature()
The I2C bus is not responding to the I2C device address of: 64
Error value: [Errno 19] ENODEV
0
```

Notice that the sensor object is still handling API calls. In this case the temperature sensor reading responds with '0'. This is not the actual temperature or the temperature value on the tile. It's a default software value that is returned by the HDC Sensor object.


# Light Level Sensor (OPT3001)

The OPT 3001 sensor documentation can be found [here](https://pybd.io/hw/tile_sensa.html). There is no PIN configuration since this device is connected via [I2C bus](https://i2c.info/) a 2 wire bus protocol.
The driver for the OPT 3001 is adapted from the micropython code, it includes some helper functions, hardening and statistics.

You can find an image at the top of the page.

## OPT Setup
The OPT sensor contains a light level sensor. It has address number 69 on the bus. The sensor is mounted via the tile header onto something like the [DIP 28](https://pybd.io/hw/wbus_dip28.html) or the [DIP 68](https://pybd.io/hw/wbus_dip68.html) i.e. it's modular and pluggable.

## Testing The Driver
The driver for the sensor is [here](https://github.com/SamsungResearchUK-IoT-Meetup/projects/blob/master/micropython/test-programs/tiles/opt3001_sensor.py). The opt3001_sensor.py file is really an enhancement to the basic micropython example code adding a few helper methods, some hardening and a few statistics. It allows you to create an object to get basic information from your light level sensor. It provides methods for lux level, testing the sensor is ready and holding the max and min lux values of values that are fetched from the board.

### Creating OPT3001 Sensor
Once you have copied the file onto the board, you can create a OPT object:

```python
/> rshell
/> repl
>>> import machine
>>> from opt3001_sensor import OPT_Sensor
>>> i2c = machine.ID2C('X')
>>> opt = OPT_Sensor(i2c)
```
### Getting Statistics
To get statistics from the temperature and humidity sensor use methods **max_lux()** and **min_lux()** like this:

```python
>>> opt.max_lux()
4318.719
>>> opt.min_lux()
4.72
```

### Detecting Current Light Levels
The light level sensor has a method for detecting current light levels which are:
* lux()
It responds with the light lux level.
Examples are:

```python
>>> opt.lux()
148.16
>>> opt.lux()
4318.719
>>> opt.lux()
5470.72
```

### OPT3001 Object Attributes
The OPT3001 public Object Attributes can tell you the start time **start_time** of the HDC2080 object and used like this:
```python
>>> opt.start_time
611145275
```

### Checking OPT3001 Is Ready
The OPT3001 can be send a message to see if it's ready. Like a 'ping' in network terminanology. In other words it will return a 'True' response on the I2C bus if it's ready to recieve commands. To do this:

```python
>>> opt.is_ready()
True
```
### Errors From OPT3001
If the OPT3001 is not responding to messages, there is an electrical fault between the microcontroller and the tile sensor, or the firmware is not configured to use the I2C bus you will see an error generated on the repl like this:

```python
>>> opt.is_ready()
The I2C bus is not responding to the I2C device address of: 64
Error value: [Errno 19] ENODEV
False
>>>
>>> opt.lu()
The I2C bus is not responding to the I2C device address of: 64
Error value: [Errno 19] ENODEV
0
```

Notice that the sensor object is still handling API calls. In this case the light sensor reading responds with '0'. This is not the actual lux or the lux value on the tile. It's a default software value that is returned by the OPT Sensor object.


# Test Program for MicroPython Pyboard D

This test program is written for the new micropython [Pyboard D](http://pybd.io/).
The main difference between this and over development board is that this is made to be modular.
So that the board could be used *as is* for a product containing the appropirate 40 pin BUS connection.

You can get started right away by writing your Python code in 'main.py' as normal.

## For a serial prompt
 - Windows: you need to go to 'Device manager', right click on the unknown device,
   then update the driver software, using the 'pybcdc.inf' file found on this drive.
   Then use a terminal program like Hyperterminal or putty.
 - Mac OS X: use the command: screen /dev/tty.usbmodem*
 - Linux: use the command: screen /dev/ttyACM0

Please visit http://micropython.org/help/ for further help.

## Example Porgram

The test program contains a 'tile' object definition which allows the user to instantiate a tile
object. The tile object will by default configure and read values from the sensor tiles made for
the pyboard 'D' which are described [here](https://pybd.io/hw/tile_sensa.html). The tiles are made to fit
onto the [68 pin](https://pybd.io/hw/wbus_dip68.html) or 28 pin DIP board.

For using the sensor on the big sensor tile. Import the driver in the repl.

```
>>> import tile_one
>>> s=tile_one.TileOne()
>>> s.demo(5) # e.g. for 5 measurements
20393133 0 TI07d0 23.3 C 25.0 % TI3001 14.32 lux
20394133 1 TI07d0 23.3 C 25.0 % TI3001 14.32 lux
20395133 2 TI07d0 23.3 C 25.0 % TI3001 14.24 lux
20396133 3 TI07d0 23.3 C 25.1 % TI3001 0.66 lux
20397133 4 TI07d0 23.3 C 25.0 % TI3001 0.18 lux
20398133 5 TI07d0 23.3 C 25.1 % TI3001 1.37 lux
>>>
```

