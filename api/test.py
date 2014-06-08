#! /usr/bin/env python2.7
#
# This is to verify that the Conductor can handle a veritable
# crapload of input.

#   This file is part of RailRoad.
#
#   RailRoad is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, version 3 of the License.
#
#   RailRoad is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with RailRoad.  If not, see <http://www.gnu.org/licenses/>.

import sys
import socket
import time

TPORT = 54320
THOST = "localhost"
try:
    # Create socket associated with train control software and connect.
    trainsock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    trainsock.connect((THOST,TPORT))
    while True:
        trainsock.send("[tot|4|t]")
        time.sleep(0.5)
        trainsock.send("[tot|4|c]")
        time.sleep(0.5)
        trainsock.send("[tot|5|t]")
        time.sleep(0.5)
        trainsock.send("[tot|6|c]")
        time.sleep(0.5)
        trainsock.send("[tot|22|t]")
        time.sleep(0.5)
        trainsock.send("[tot|14|c]")
        time.sleep(0.5)
        trainsock.send("[tot|13|t]")
        time.sleep(0.5)
        trainsock.send("[tot|15|c]")
        time.sleep(0.5)
        trainsock.send("[tot|17|t]")
        time.sleep(0.5)
        trainsock.send("[tot|1|c]")
        time.sleep(0.5)
        trainsock.send("[tot|2|t]")
        time.sleep(0.5)
        trainsock.send("[tot|7|c]")
        time.sleep(0.5)
        trainsock.send("[spd|3|25]")
        time.sleep(0.5)
        trainsock.send("[spd|1|25]")
        time.sleep(0.5)
        trainsock.send("[spd|3|105]")
        time.sleep(0.5)
        trainsock.send("[spd|1|105]")
        time.sleep(0.5)
        trainsock.send("[snd|3|off]")
        time.sleep(0.5)
        trainsock.send("[snd|3|on]")
        time.sleep(0.5)
        trainsock.send("[snd|1|off]")
        time.sleep(0.5)
        trainsock.send("[snd|1|on]")
        time.sleep(0.5)
except:
    print "All done."