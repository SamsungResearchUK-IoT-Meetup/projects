import ssd1306, pyb
from micropython import const
from machine import Pin, I2C
from ssd1306 import SSD1306_I2C

# register definitions
OLED_I2C_ADDRESS    = const(0x3c)
OLED_WIDTH          = const(128)
OLED_HEIGHT         = const(64)
# Create objects
i2c = I2C(scl=Pin('X9'), sda=Pin('X10'))
oled = SSD1306_I2C(OLED_WIDTH, OLED_HEIGHT, i2c, OLED_I2C_ADDRESS)

# Moving text down the screen
print('*** Starting test to OLED display ****')

print('Printing: "Hello World"')
oled.fill(0)
oled.text("Hello World", 0, 0)
oled.show()
pyb.delay(2000)

print('Printing: "Hello World" on 2nd Line')
oled.fill(0)
oled.text("Hello World", 0, 10)
oled.show()
pyb.delay(2000)

print('Printing: "Hello World" on 3nd Line')
oled.fill(0)
oled.text("Hello World", 0, 20)
oled.show()
pyb.delay(2000)

print('Printing: "Hello World" on 4th Line')
oled.fill(0)
oled.text("Hello World", 0, 30)
oled.show()
pyb.delay(2000)

print('Printing: "Hello World" on 5th Line')
oled.fill(0)
oled.text("Hello World", 0, 40)
oled.show()
pyb.delay(2000)

print('Printing: "Hello World" on 6th Line')
oled.fill(0)
oled.text("Hello World", 0, 50)
oled.show()
pyb.delay(2000)

print('Printing: "Hello World" on 7th Line')
oled.fill(0)
oled.text("Hello World", 0, 60)
oled.show()
pyb.delay(2000)

# Moving text across the screen

print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 0, 0)
oled.show()
pyb.delay(500)


print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 5, 0)
oled.show()
pyb.delay(500)


print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 10, 0)
oled.show()
pyb.delay(500)


print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 15, 0)
oled.show()
pyb.delay(500)


print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 20, 0)
oled.show()
pyb.delay(500)


print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 25, 0)
oled.show()
pyb.delay(500)


print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 30, 0)
oled.show()
pyb.delay(500)


print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 35, 0)
oled.show()
pyb.delay(500)

print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 40, 0)
oled.show()
pyb.delay(500)

print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 45, 0)
oled.show()
pyb.delay(500)

print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 50, 0)
oled.show()
pyb.delay(500)

print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 55, 0)
oled.show()
pyb.delay(500)

print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 60, 0)
oled.show()
pyb.delay(500)

print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 65, 0)
oled.show()
pyb.delay(500)

print('Printing: "Hello World" moving right')
oled.fill(0)
oled.text("Hello World", 70, 0)
oled.show()
pyb.delay(500)


# Inverting Dsiplay

print("Now inverting Display")
oled.invert(True)