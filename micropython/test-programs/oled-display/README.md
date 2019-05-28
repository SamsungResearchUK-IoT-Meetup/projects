## OLED Display

The OLED Display driver documentation can be found [here](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf). The OLED Display driver is a monochrome 128x64 Dot Matrix display. The version used in the lab is controlled over an I2C protocol Pin. Fortunately there is already a driver written for this display which is [here](https://github.com/micropython/micropython/blob/master/drivers/display/ssd1306.py). This driver file is copied locally to our project directory and used to control the display.

The pin configuration is:

### Pin Configuration

| Pin Number    | Pin Name               |Description        |
|---------------|------------------------|-------------------|
| 1             | Ground                 | Connected to ground of circuit |
| 2             | Vcc                    | Vcc between 3.3V to 5V |
| 3             | SCL                    | I2C Clock line    |
| 4             | SDA                    | I2C Data line     |

![PIR Image2](https://github.com/SamsungResearchUK-IoT-Meetup/projects/blob/master/micropython/assets/oled.png)

The image above is slightly different to the lab version in that Ground (GND) and Power (Vcc) are reversed. To use the OLED display we connected Pins:
* Pin 1 to GND (Pin 1 or 2 on the [WBUS-DIP68](https://pybd.io/hw/wbus_dip68.html))
* Pin 2 to VIN (Pin 5 on WBUS-DIP68)
* Pin 3 (SCL) to X9 (Pin 48 on WBUS-DIP68). You can see Pins used on the microcontroller for I2C [here](https://pybd.io/hw/pybd_sfxw.html).
* Pin 3 (SDC) to X10 (Pin 50 on WBUS-DIP68). You can see Pins used on the microcontroller for I2C [here](https://pybd.io/hw/pybd_sfxw.html).

## Possible Pin Configuration
The OLED Display uses the I2C bus on the microcontroller. To find out what possible Pin's can be used for the I2C bus you can [check here](https://store.micropython.org/media/products/PYBD_SF2_W4F2_ds2.jpg). Possible Pin's are X9, X10 and Y9, Y10 for SCL (clock) and SDA (data) lines respectively. i.e. there are 2 I2C bus controllers that are exposed on the board.
As mentioned above, this was tested using Pin X9 and X10.

### Testing The Driver And OLED Display
The driver for the OLED is already written and part of the [micropython github repo here](https://github.com/micropython/micropython/blob/master/drivers/display/ssd1306.py). The driver is also copied locally onto our [test-programs](https://github.com/SamsungResearchUK-IoT-Meetup/projects/tree/master/micropython/test-programs) directory and our [dirvers](https://github.com/SamsungResearchUK-IoT-Meetup/projects/tree/master/micropython/multimode-sensor/drivers) directory.

#### Setup Driver
To setup the driver to test, simple copy it to the microcontroller

```python
/> rshell
/>  cp ssd1306.py /flash
```

#### Driver Description And Methods/Functions
The driver sub classes the **frambuf** class in micropython. It provides functions to:
* initialize the display for a set size and width;
* power off;
* power on;
* set contrast;
* invert display;
* show a set frame;
* control as an I2C or SPI bus; and
* write text to be displayed at a particular 'x' and 'y' position. 

Lets look at trying a simple **hello world** to the screen.

##### Creating The SSD1306 Object #####
Assuming you have copied the file onto your flash drive on the microcontroller you first have to import the driver, then create the object using the correct screen size and I2C address to instantiate the object.

```python
>>> import ssd1306
>>> from machine import Pin
>>> from machine import I2C
>>> from ssd1306 import SSD1306_I2C
>>> i2c = I2C(scl=Pin('X9'), sda=machine.Pin('X10'))       # 2) Create i2c object
>>> oled = SSD1306_I2C(128, 64, i2c, 0x3c)                 # 3) Create OLED object
>>> i2c.scan()                                             # 4) Check the device is on the bus
[60]
```
1) We first **import** the classes **Pin**, **I2C**, and **SSD1306_I2C**. Note that we are using the I2C OLED, so we are using this class which sub classes SSD1306.
2) We then create an i2c object which binds SCL to Pin X9, and SDA onto Pin X10. This attaches the protocol to the Pin's that we have physically attached the OLED to.
3) We create our OLED Object and tell it the horizontal, vertical size, the i2c bus object, and the address the OLED has. In our case, the OLED has address 60 in decimal - hence 3C in hex.
4) We can check that the OLED device is responding to that address by scanning the I2C bus. It responds as **60** which is correct

#### Printing To The OLED ####
To print to the OLED it's a 3 step process. 
1) Clear the screen buffer of any images and text
2) Place your text onto the screen. At this point it will not show anything.
3) Telling the screen to show the text.
It's done this way due to the delay in sending the information to the screen over the bus. Better do this first for the screen to place into it's frame buffer. Then send the command to make that frame buffer active.

```python
>>> oled.fill(0)                      # 1) clear the screen buffer
>>> oled.text("Hello World", 0, 0)    # 2) Place the text into the screen buffer at 'x' & 'y' co-ordinate 0
>>> oled.show()                       # 3) Tell the OLED to display this screen buffer
```

#### Printing Multiple Lines ####
To print multiple lines you need to position each line approx 10 pixels down from the previous. In this case the top of the screen is '0' and as you move down 'y' increases in size. Hence printing 3 lines to the screen looks like this:

```python
>>> oled.fill(0)
>>> oled.text("Hello World", 0, 0)
>>> oled.text("Hello World 2", 0, 10)
>>> oled.text("Hello World 3", 0, 20)
>>> oled.show()
```

<img src="https://github.com/SamsungResearchUK-IoT-Meetup/projects/blob/master/micropython/assets/oled.jpg" alt="OLED with text displayed" width="400">

#### Inverting The Display ####
To invert the display simply pass 'True' to the invert function. Or 'False' if it's already inverted.

```python
>>> oled.invert(True)
```

### Running Test Script ###
There is a test script which will print text to the screen then delete and show through various positions. To run the script first copy to the microcontroller and run.

```python
/> rshell
/> cp test_oled.py /flash
/> repl
>>>> import test_oled
*** Starting test to OLED display ****
Printing: "Hello World"
Printing: "Hello World" on 2nd Line
Printing: "Hello World" on 3nd Line
Printing: "Hello World" on 4th Line
Printing: "Hello World" on 5th Line
Printing: "Hello World" on 6th Line
Printing: "Hello World" on 7th Line
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Printing: "Hello World" moving right
Now inverting Display
```



