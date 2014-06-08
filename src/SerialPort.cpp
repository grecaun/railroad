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

#include "SerialPort.h"

/** CONSTRUCTOR FOR TESTING */
SerialPort::SerialPort() {
   _connection = 1;
}

/** CONSTRUCTOR */
SerialPort::SerialPort(int connect) {
   _connection = (!connect);

   if(_connection) {
      _port_fd = create_port();
      configure_port();
   }
}


/** DESTRUCTOR */
SerialPort::~SerialPort() {
   if(_connection)
      close(_port_fd);
}


/** SENDS THE PACKET OF HEX data OF SIZE size TO THE _port_fd */
int SerialPort::send(byte* data, int size) {
   int ret = 0;

   // attempt to write to usb port
   if(_connection && (ret = write(_port_fd, data, size)) <= 0) {
      cout<<"CND: "<<"COULD NOT SEND DATA!"<<endl;
      exit(EXIT_FAILURE);
   }

   // print out packet if verbose.
   if(!_connection) {
      cout<<"CND: "<<"TX: ";
      for(int i = 0; i < size; i++)
         cout<<"<"<<hex<<(unsigned int)data[i]<<">";
      cout<<endl;
   }
   return ret;
}


/** RECIEVES A PACKET OF HEX DATA of size FROM THE _port_fd */
byte* SerialPort::recieve(int size) {
   byte *data = new byte[size];
   int ret = 0;

   //attempt to read from usb port
   if(_connection && (ret = read(_port_fd, data, size)) < 0) {
      perror("RCV: Read");
      exit(EXIT_FAILURE);
   }

   if(ret == 0) {
      delete[] data;
      return NULL;
   }

   return data;
}


/** CREATES THE PORT USING DEV ttyUSB0 */
int SerialPort::create_port() {
   int port;

   cout<<"CND: "<<"opening port...";
   if((port = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY)) <= 0) {
      cout<<"CND: "<<"fail."<<endl;
      exit(EXIT_FAILURE);
   }

   cout<<"done."<<endl;
   return port;
}


/** CONFIGURES THE PORT USING TERMIOS */
void SerialPort::configure_port() {
   cout<<"CND: "<<"configuring port...";

   struct termios config;

   // RECIEVE TERMIOS STRUCT FROM _PORT_FD_if
   if(tcgetattr(_port_fd, &config) < 0) {
      cout<<"fail."<<endl;
      exit(EXIT_FAILURE);
   }

   cout<<"Port configuration retrieved...";

   // SET INPUT AND OUTPUT BAUD
   if(cfsetispeed(&config, B57600) < 0 || cfsetospeed(&config, B57600) < 0) {
      cout<<"fail."<<endl;
      exit(EXIT_FAILURE);
   }

   cout<<"Baud rate set...";

   // NEEDED FOR FLOW CONTROL RTS/CTS HARDWARE ON
   config.c_lflag &= ~(_BSD_SOURCE);

   // NO PARITY, 1BIT STOPBIT, 8-BIT TRANSMISSION, DISABLE HARDWARE FLOW CONTROL
   // ENABLE RECEIVING, DON'T CHANGE OWNER OF PORT
   // PARENB  = Enable parity bit
   // CSTOPB  = 2 stop bits (1 otherwise)
   // CSIZE   = Bit mask for data bits
   // CRTSCTS = Enable hardware flow control (not supported on all platforms)
   // CS8     = 8 data bits
   // CREAD   = Enable receiver
   // CLOCAL  = Local line - do not change "owner" of port
   config.c_cflag &= ~(PARENB | CSTOPB | CSIZE | CRTSCTS);
   config.c_cflag |= CS8 | CREAD | CLOCAL;

   cout<<"Applying configuration...";

   // APPLY CONFIGURATIONS TO _PORT_FD
   if(tcsetattr(_port_fd, TCSAFLUSH, &config) < 0) {
      cout<<"fail."<<endl;
      exit(EXIT_FAILURE);
   }

   // CONFIGURATION COMPLETED SUCCESFULLY
   cout<<"done."<<endl;
}
