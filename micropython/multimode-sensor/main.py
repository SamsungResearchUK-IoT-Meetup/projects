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


from wifi_connect import *                             # Import the WiFi Manager module to help us monitor and stay connected to poor wifi
from routes import *                                   # Import all the routes that are being used in our web pages and have functions controlling each route
from web.microWebSrv import MicroWebSrv                # Import the WiFi microweb server object to allow us to run a mini web server on the board

# ============================================================================
# ===( WiFi Manager Process )=================================================
# ============================================================================

mywifi = Wifi_manager()                                 # Create our WiFi manager object
mywifi.connect('srbackup', 'Tr3x1949')                  # TODO pull the password and SSID from an encrypted file on FLASH

# ============================================================================
# ===( Web Server Process )===================================================
# ============================================================================

# routeHandlers = [
#	( "/test",	"GET",	_httpHandlerTestGet ),
#	( "/test",	"POST",	_httpHandlerTestPost )
# ]

srv = MicroWebSrv(webPath='www/')
srv. MaxWebSocketRecvLen     = 256
srv.WebSocketThreaded		= False
srv.AcceptWebSocketCallback = _acceptWebSocketCallback
srv.Start()

# ----------------------------------------------------------------------------



