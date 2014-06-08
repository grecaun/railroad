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

#ifndef __TEST_SERIALPORT_H__
#define __TEST_SERIALPORT_H__

#include "../src/SerialPort.h"
#include <unistd.h>

class Test_SerialPort : public SerialPort {
	public:
		Test_SerialPort() {
            pipe2(fds, O_NONBLOCK);
		}
        ~Test_SerialPort() {
            close(fds[0]);
            close(fds[1]);
        }
        int ConfigureRecv() {
            _port_fd = fds[0];
            return fds[1];
        }
        int ConfigureTrans() {
            _port_fd = fds[1];
            return fds[0];
        }
    private:
        int fds[2];
};

#endif