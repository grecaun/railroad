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

#ifndef __CONDUCTOR_H__
#define __CONDUCTOR_H__

#include "Transmitter.h"
#include "Track.h"
#include "VBuffer.h"
#include "MailCar.h"

#define C_POWER         0
#define C_STOP          1
#define C_REQUEST       2
#define C_TOKEN         3
#define C_SPEED         4
#define C_DIRECTION     5
#define C_SOUND         6
#define C_TURNOUT       7
#define C_LOCK_TRAIN    8
#define C_LOCK_TURNOUT  9
#define C_EXIT         10
#define R_POWER        11
#define R_SENSOR       12
#define R_TURNOUT      13
#define R_SPEED        14
#define R_DIRECTION    15
#define R_SOUND        16
#define NUM_C          17
#define CMDS "POW STP RQS TKN SPD DIR SND TOT LSL LTO EXT"
#define RCVS "IPOW ISEN ITOT ISPD IDIR ISND"

class Conductor{
  public:
   static void start_thread(void* t);
   Conductor(SerialPort* usb, MailCar *input, MailCar *output, Track* track, int APISock, atomic<bool>* exit, int debug);
   ~Conductor();
   void conduct_loop();

  private:
   MailCar*      _inLine;    // Means by which Conductor receives commands.
   MailCar*      _logicLine; // line for sending messages to logic
   int 			     _outLine;
   atomic<bool>* _exit;
   Transmitter*  _trans;
   Track*        _track;
   VBuffer       _recvdLine;
   int           _debug;

   void parseRecvd();
};

#endif // __CONDUCTOR_H__
