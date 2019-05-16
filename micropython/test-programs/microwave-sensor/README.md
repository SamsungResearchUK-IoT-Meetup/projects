

## Microwave Radar Sensor
The RCWL-0516 Microwave Radar sensor documentation can be found [here](https://ita.ovh/files/rcwl-0516.pdf). There is also a GitHub repo with more information [here](https://github.com/jdesbonnet/RCWL-0516). The sensor works using Doppler effect with an output frequency around 3GHz. Again for detailed information on the electrical workings of the sensor please refer to the link above written by [Jdesbonnet](https://github.com/jdesbonnet).



The pin configuration is:

### Pin Configuration
| Pin   | Function |
| ---   | --- |
| 3V3   | 3.3V regulated output. Max 100mA (?)                  |
| GND   | Ground                                                |
| OUT   | Digital pulse high (3.3V) when triggered (motion detected). 0V normally. |
| VIN   | 4 - 28V supply voltage                                |
| CDS   | LDR 10-20k RL, U_LDR > 0.7V = On                      |

Here we use Pin 'GND', 'OUT' and 'VIN'. Where the ground (GND) is connected to the ground terminal of the micropython board, 'VIN' is connected to Pin 'VIN' ([Pin 5 on WBUS-DIP68](https://pybd.io/hw/wbus_dip68.html)) and 'OUT' is configured to the Pin used in your code to detect movement.

![RCWL-0516 board](https://github.com/jdesbonnet/RCWL-0516/blob/master/images/RCWL-0516-board.jpg)


### Microwave Radar Sensor Setup
The Microwave Radar sensor is defaulted to detect and hold the 'OUT' Pin high for 4 seconds after detecting movement. Meaning it will go 'high' when it detects movement and keeps it high for this wait period. 

### Testing The Driver
The driver for the sensor is [here](https://github.com/SamsungResearchUK-IoT-Meetup/projects/blob/master/micropython/test-programs/microwave-sensor/rcwl_0516_sensor.py). The rcwl_0516_sensor.py file is really a wrapper over the micropython interrupt handlers. It allows you to create an object to manage your connection, get basic information from your Microwave Radar sensor and provide the object a callback function to be called when signals are detected from the Microwave Radar sensor.

#### Start Microwave Radar Sensor
A basic example with a sensor having pin 3 connected to X1, pin 4 to VCC and pin 2 to ground (GND) on the [Micropython Board](https://pybd.io/hw/wbus_dip68.html ). You can easily create a Microwave Radar sensor object and test with the following code:

```python
/> rshell
/> repl
>>> import pyb
>>> import time
>>> import rcwl_0516_sensor
>>> from  rcwl_0516_sensor import MicrowaveRadar
>>> mr1 = MicrowaveRadar(mr_pin_id='X2')        # Creates a MicrowaveRadar object
>>> mr1.start()                                 # Starts the MicrowaveRadar sensor to detect objects.
(True, None)
>>> MicrowaveRadar callback called with line number:  1
*** Test callback function ***
    Line value is:    1 
    Program Time is:  611147966
MicrowaveRadar callback called with line number:  1
```
#### Getting Statistics
To get statistics from the MicrowaveRadar sensor you can call the methods **mr_stats()** and **mr_total()** like this:

```python
>>> mr1.mr_stats()
{'Last Event': 611150071, 'Trigger Events': 16, 'Start Time': 611145275}
>>>  
>>> mr1.mr_total()
16
>>>
```
The **mr_total** method returns the total number of times the sensor has detected objects since the controller has been started. 

The **mr_stats** method returns three things:
1) the last time it detected an object, 
2) the start time of the MicrowaveRadar object **i.e. since the object was instantiated**; and 
3) how many times it has detected objects.

#### Detecting Current Activity
The MicrowaveRadar sensor has two methods to find out it's current state. There are two states to worry about here, the first tells you if the MicrowaveRadar sensor has been activated to detect objects and is called **is_active**.

The second method tells you if it is currently detecting something and is called **is_currently_active**. Things to note, **is_currently_active** is really useful as a debug tool. It allows you to find out if the pin is 'high' when issued. Examples of using the methods can be used like this:

```python
>>> mr1.is_active()
True
>>> 
>>> mr1.is_currently_active()
0
>>> mr1.is_currently_active()
1
>>>
```

#### MicrowaveRadar Object Attributes
The MicrowaveRadar public Object Attributes can tell you the start time **start_time** of the MicrowaveRadar object, and the current MicrowaveRadar pin **mr_pin**. Examples of using the methods can be used like this:
```python
>>> mr1.start_time
611145275
>>> mr1.mr_pin
'X2'
>>>
```

#### Stopping the MicrowaveRadar
The MicrowaveRadar object can be stopped. In other words it will no longer report when objects are being detected via the interrupt handler using the **stop()** method. This can be run simple like:
```python
>>> mr1.stop()
(True, 'OK')
>>>
```

#### Finding Info
The MicrowaveRadar object has a name attribute which can be queried. It provides the description of the currently instantiated object and can be used like this:

```python
>>> 
>>> mr1.name
'Microwave Radar Object for RCWL-0516 Sensor'
>>>
```
