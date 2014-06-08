/*
   This file is part of RailRoad.

   RailRoad is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3 of the License.

   RailRoad is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with RailRoad.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <atomic>
#include "stdns.h"

typedef unsigned char byte;

class SerialPort {
  public:
   SerialPort(int connect);
   ~SerialPort();
   int send(byte* data, int size);
   int get_port(){return _port_fd;}
   byte* recieve(int size);

  protected:
    SerialPort();
    int _connection;
    int _port_fd;
    int create_port();
    void configure_port();
};

#endif
