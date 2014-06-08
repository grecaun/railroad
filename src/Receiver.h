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

#ifndef __RECEIVER_H__
#define __RECEIVER_H__

#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "SerialPort.h"
#include "HexDec.h"
#include "MailCar.h"
#include "stdns.h"
#include "opcodes.h"

#define TIMEBTWNTICK 1500

extern HexDec hexdec;

class Receiver {
  public:
   static void start_thread(void* r);
   Receiver(SerialPort* usb, MailCar *output, int numSens, std::atomic<bool>* exit, int debug);
   ~Receiver();
   void recieve_loop();

  private:
   MailCar*            _outLine;
   SerialPort*         _usb;
   std::atomic<bool>*  _exit;
   std::ofstream*      _debugfile;
   unsigned long long* _sensTimers;

   int    send             (string command);
   int    is_useless       (byte opcode);
   int    compute_opnum    (byte opcode);
   string breakdown_command(byte opcode, byte* message);
   int    compute_msglen   (int optype,  byte opcode);
   void   print_debug      (char* cmd,   char* arg1, char* arg2);

   //conversions from hex to locoproto
   int    hextodec(byte hex);
   string conv_pow(byte* message);
   string conv_sensor(byte* message);
   string conv_turnout(byte* message);
   string conv_loco_speed(byte* message);
   string conv_loco_func1(byte* message);
   string conv_loco_func2(byte* message);
};

#endif
