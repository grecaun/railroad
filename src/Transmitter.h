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

#ifndef __TRANSMITTER_H__
#define __TRANSMITTER_H__

#include <sstream>
#include <string.h>
#include "HexDec.h"
#include "SerialPort.h"
#include "stdns.h"

#define EOS '\0'
#define MAX_SLOTS 3
#define MAX_TURNOUTS 23
#define MIN_SPEED 25

class Transmitter {
  public:
   Transmitter(SerialPort* usb, int debug);
   ~Transmitter();
   void send_power    (char* state);
   void send_request  (char* addr);
   void send_stop     (char* slot);
   void send_move     (char* slot1,   char* slot2);
   void send_speed    (char* slot,    char* speed);
   void send_direction(char* slot,    char* dir);
   void send_sound    (char* slot,    char* state);
   void send_turnout  (char* turnout, char* state);

  private:
   int         _size;
   int         _slotAvail;
   HexDec      _hex;
   SerialPort* _usb;
   ofstream*   _debugfile;

   void print_debug(char* cmd,    char* arg1, char* arg2);
   int  send_packet(int numbytes, byte* packet);

   // conversions to hex
   byte* conv_power    (char* onoroff);
   byte* conv_request  (char* addr);
   byte* conv_move_slot(char* slot1,   char* slot2);
   byte* conv_speed    (char* slot,    char* speed);
   byte* conv_stop     (char* slot);
   byte* conv_direction(char* slot,    char* dir);
   byte* conv_turnout  (char* turnout, char* torc);
   byte* conv_sound    (char* slot,    char* onoroff);
};

#endif
