## PIR Sensor

The SR-501 PIR sensor documentation can be found [here](https://www.mpja.com/download/31227sc.pdf). The pin configuration is:

### Pin Configuration

| Pin Number    | Pin Name               |Description        |
|---------------|------------------------|-------------------|
| 1             | Vcc                    | Input voltage is +5V for typical applications. Can range from 4.5V- 12V |
| 2             | High/Low Ouput (Dout)  | Digital pulse high (3.3V) when triggered (motion detected) digital low(0V) when idle(no motion detected |
| 3             | Ground                 | Connected to ground of circuit |

![PIR Image2](https://github.com/SamsungResearchUK-IoT-Meetup/projects/blob/master/micropython/assets/HC-SR501-PINOUT.jpg)


### Repeatable (H) Mode
In Repeatable(H) mode the output pin Dout will go high (3.3V) when a person is detected within range and goes low after a particular time (time is set by “Off time control” potentiometer). In this mode the output pin will go high irrespective of whether the person is still present inside the range or has left the area. The sensitivity can be set using the “sensitivity control” potentiometer

### Non Repeatable (L) Mode
In “I” mode the output pin Dout will go high (3.3V) when  a person is detected within range and will stay high as long as he/she stays within the limit of the Sensors range. Once the person has left the area the pin will go low after the particular time which can be set using the potentiometer. The sensitivity can be set using the “sensitivity control” potentiometer

### PIR Setup
The PIR sensor is defaulted to 'Repeatable' mode. Meaning it will go 'high' when it detects movement and keeps it high for a wait period. To setup the sensor we reduce the wait period to it's minimum amount - 4 seconds - by turning the time delay adjust pot to the minimum amount. This gives a good signal pulse to be captured by the PIR driver and processed. 

### Testing The Driver
The driver for the sensor is [here](https://github.com/SamsungResearchUK-IoT-Meetup/projects/blob/master/micropython/test-programs/pir-sensor/sr_501_sensor.py). The sr_501_sensor.py file is really a wrapper over the micropython interrupt handlers. It allows you to create an object to manage your connection, get basic information from your PIR sensor and provide the object a callback function to be called when signals are detected from the PIR sensor.

#### Start PIR Sensor
A basic example with a sensor having pin 2 connected to X1 on the micropython board, pin 1 to VCC and pin 3 to ground (GND). You can easily create a PIR object and test with the following code:

```python
/> rshell
/> repl
>>> import pyb
>>> import time
>>> import sr_501_sensor
>>> from sr_501_sensor import PIR
>>> p1 = PIR(pir_pin_id='X1')        # Creates a PIR object
>>> p1.start()                       # Starts the PIR sensor to detect objects.
(True, None)
>>> PIR callback called with line number:  0
*** Test callback function ***
    Line value is:    0 
    Program Time is:  611147966
PIR callback called with line number:  0
```
#### Getting Statistics
To get statistics from the PIR sensor you can call the methods **pir_stats()** and **pir_total()** like this:

```python
>>> p1.pir_stats()
{'Last Event': 611150071, 'Trigger Events': 16, 'Start Time': 611145275}
>>>  
>>> p1.pir_total()
16
>>>
```
The **pir_total** method returns the total number of times the sensor has detected objects since the controller has been started. 

The **pir_stats** method returns three things:
1) the last time it detected an object, 
2) the start time of the PIR object **i.e. since the object was instantiated**; and 
3) how many times it has detected objects.

#### Detecting Current Activity
The PIR sensor has two methods to find out it's current state. There are two states to worry about here, the first tells you if the PIR sensor has been activated to detect objects and is called **is_active**.

The second method tells you if it is currently detecting something and is called **is_currently_active**. Things to note, **is_currently_active** is really useful as a debug tool. It allows you to find out if the pin is 'high' when issued. Examples of using the methods can be used like this:

```python
>>> p1.is_active()
True
>>> 
>>> p1.is_currently_active()
0
>>> p1.is_currently_active()
1
>>>
```

#### PIR Object Attributes
The PIR public Object Attributes can tell you the start time **start_time** of the PIR object, and the current PIR pin **pir_pin**. Examples of using the methods can be used like this:
```python
>>> p1.start_time
611145275
>>> p1.pir_pin
'X1'
>>>
```

#### Stopping the PIR
The PIR object can be stopped. In other words it will no longer report when objects are being detected via the interrupt handler using the **stop()** method. This can be run simple like:
```python
>>> p1.stop()
(True, 'OK')
>>>
```


