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

