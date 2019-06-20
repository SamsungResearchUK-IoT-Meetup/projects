"""
MIT License
Copyright (c) 2019 Samsung. n.herriot@samsung.com
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

__version__ = '0.1.0'
__author__ = 'Nicholas Herriot'
__license__ = "MIT"

import pyb, network, utime, time, micropython

class Wifi_manager():
    """
        A pythonic Wifi Manager which will:
        1) Allow you to configure SSID and Password to connect and maintain/check that connection.
        2) Retry and connect if it's been asked to connect and the connection goes down.
        3) Use a WiFi SSID/Password from Flash if it exists.
        4) Encrypt the WiFi SSID/Password on Flash if it does not exist.
        5) Stop a connection and stop the retry/check
        6) Switch to an Access Point mode

        The Wifi manager has internal states for:
        - the current SSID
        - the current password
        - active (i.e. managing a connection )
        - connected
        - last connected local time
    """

    def __init__(self, ssid=None, password=None):
        self.ssid=ssid
        self._password = password
        self._timer = None                              # The timer object which handles periodic retries
        self._wifi = network.WLAN()                     # An instance of the network WLAN manager
        print("WiFi Manager bringing up wlan interface.")
        self.x = 0.1
        self.bar_bound = self.bar

    def bar(self, bar_value):
        self.x *= 1.2
        print("bar value is: {}".format(bar_value))
        print("SSID is: {}".format(self.ssid))
        print("Password is: {}".format(self._password))
        print("X value is: {}".format(self.x))
        if self.ssid is None:
            self.ssid = '********'
        if self._password is None:
            self._password = '********'
        self._wifi.connect(self.ssid, self._password)

    def cb(self, timer):
        # Passing self.bar would cause memory allocation issues.
        #print("Timer counter is: {}".format(timer.counter()))      # This causes a timer exception to be thrown when it's part of the callback.
        print(timer.counter())
        micropython.schedule(self.bar_bound, "hello world")

    def connect(self):
        print("connect 2 started.....")
        self._timer = pyb.Timer(1, freq=0.1)
        self._timer.callback(self.cb)

