from micropython import const
import pyb
from pyb import Pin
import machine, uerrno

HDC_ADDR = const(64)            # Humidity and temperature sensor I2C address: http://www.ti.com/lit/ds/symlink/hdc2080.pdf
OPT_ADDR = const(69)            # Ambient light sensor I2C address: http://www.ti.com/lit/ds/symlink/opt3001.pdf


class TileOne:

    """An example class which allows the configuration of 'tiles' on the mycropython pyboard-D product line.

    Creation of the TileOne class takes a 'bus' or 'slot' value of 'X' or 'Y'.
    If none is given it defaults to 'X'. This means that the bus used will be
    either from the 'Y' defined pin's or 'X' defined pins of the micropython board.
    You can find out more about pin configuration here:

    https://store.micropython.org/media/products/PYBD_SF2_W4F2_ds2.jpg

    Attributes:
        slot (str: optional): Slot or bus value which can be 'X' or 'Y'. Defaults to 'X'

    """

    def __init__(self, slot='X'):

        try:
            if slot == 'X':
                self.tim = 2
                self.chan = 1
            elif slot == 'Y':
                self.tim = 1
                self.chan = 1
            else:
                raise ValueError('either "X" or "Y" slot')
            i2c = machine.I2C(slot)
            self.i2c = i2c
            i2c.writeto_mem(OPT_ADDR, 0x1, b'\xcc\x10')  # Write to register 1. This sets measurement in continuous mode at 800ms cycle.
            i2c.writeto_mem(HDC_ADDR, 0x0e, b'\x75')  # 5Hz, heater off, int act low, comp mode
            i2c.writeto_mem(HDC_ADDR, 0x0f, b'\x01')  # start
        except ValueError:
            print('Slot value should be either "X" or "Y".')
        except OSError as exc:
            if exc.args[0] == uerrno.EIO:
                print("There was an IO error reading and writing to the sensors")
            elif exc.args[0] == uerrno.ENXIO:
                print("There is no device on the I2D address specified")
            elif exc.args[0] == uerrno.ECOMM:
                print("There was a communications error in sending to the I2C sensors")
            else:
                print('There was an operating system error while writing to I2C devices on initialisation')

        self.buz_en = pyb.Pin(slot+'1', mode=pyb.Pin.OUT_PP, value=0)
        self.buz = Pin(slot+'6')

        leds = []
        for i in range(2, 5):
            leds.append(pyb.Pin(slot+str(i), mode=Pin.OUT_OD, value=1))
        self.leds = leds

        self.opt_int = pyb.Pin(slot+'12', mode=Pin.OUT_OD, pull=Pin.PULL_UP, value=1)
        self.hdc_int = pyb.Pin(slot+'8', mode=Pin.OUT_OD, pull=Pin.PULL_UP, value=1)

    def opt(self):
        man = self.i2c.readfrom_mem(OPT_ADDR, 0x7e, 2)
        dev = self.i2c.readfrom_mem(OPT_ADDR, 0x7f, 2)
        res = self.i2c.readfrom_mem(OPT_ADDR, 0x0, 2)
        res = (res[0] << 8) + res[1]
        exp = res >> 12
        res &= 0xfff
        res <<= exp
        return '%c%c%04x %.2f lux' % (man[0], man[1], (dev[0] << 8) + dev[1], res/100)
    
    def hdc(self):
        man = self.i2c.readfrom_mem(HDC_ADDR, 0xfc, 2)
        dev = self.i2c.readfrom_mem(HDC_ADDR, 0xfe, 2)
        temp = self.i2c.readfrom_mem(HDC_ADDR, 0x00, 2)
        hum = self.i2c.readfrom_mem(HDC_ADDR, 0x02, 2)
        temp = temp[0] + (temp[1] << 8)
        temp = temp*165/0x10000 - 40
        hum = hum[0] + (hum[1] << 8)
        hum = hum/0x10000*100
        return '%c%c%04x %.1f C %.1f %%' % (man[1], man[0], (dev[1] << 8) + dev[0], temp, hum)

    def heater(on=None):
        cfg = bytearray(2)
        if on is None:
            self.i2c.readfrom_mem_into(HDC_ADDR, 0x0e, cfg)
            return (cfg[0] & 8) > 0
        else:
            self.i2c.readfrom_mem_into(HDC_ADDR, 0x0e, cfg)
            if on:
                cfg[0] |= 8
            else:
                cfg[0] &= 0xf7
            self.i2c.writeto_mem(HDC_ADDR, 0x0e, cfg[:1])   # 5Hz, heater off, int act low, comp mode
            return on

    def beep(self, freq=4000, dur=100, v=100):
        # TODO wrap value checking in a try or another function then call up the timer code to make
        # the buzzer sound
        if freq < 100:
            self.buz_en(0)
            return
        else:
            self.buz_en(1)
        if v < 1:
            v = 1
        elif v > 10:
            v = 10
        t = pyb.Timer(self.tim, freq=freq, mode=pyb.Timer.CENTER)
        t.channel(self.chan, pyb.Timer.PWM, pin=self.buz, pulse_width=(t.period() + 1) // (200//v))
        pyb.delay(dur)
        t.channel(self.chan, pyb.Timer.PWM, pin=self.buz, pulse_width=0)

    def led(self, color):
        color ^= 7                                  # Exclusive OR Bitwise operation
        self.leds[0](color & 1)
        self.leds[1](color & 2)
        self.leds[2](color & 4)


    def demo(self, n=0, dt=1000, rgb=0):
        next = pyb.mills()                         # Before we start take a snapshot of the current time on the board

        for x in range(n):                         # Setup our loop to go through 'n' iterations.
            hdc = self.hdc()                       # Read a value from our humidity and temperature sensor
            lux = self.opt()                       # Read a value from our light level sensor
            self.beep(dur=150)                     # Beep our sounder
            self.beep(0)                           # Turn off the sounder
            self.led((rgb+x) & 7)                  # Change colour based on taking the modulus of 'x mod 7'
            print(next, rgb+x, hdc, lux)
            time_current = pyb.millis()            # Get current time from the board. Used to work out elapsed time
            pyb.delay(next - time_current)         # Make sure the delay time is delay - time to do all the functions above.
                                                   # First time round the loop this will be negative. TODO check for negative times.
            next += dt                             # Add our delay increment to the start time.



