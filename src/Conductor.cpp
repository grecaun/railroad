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

#include <sstream>
#include "Conductor.h"
#include "Transmitter.h"

/**CONSTRUCTOR: TAKES SERIALPORT FOR COMM TO LOCOBUFFER, PIPE TO READ IN COMMANDS TO
   CONVERT TO HEX, BOOLEAN FOR A SIGNAL TO EXIT, AND A DEBUG FLAG FOR DEBUG MODE. */
Conductor::Conductor(SerialPort* usb, MailCar *input, MailCar *output, Track* track, int APISock, atomic<bool>* exit, int debug) {
   _inLine    = input;
   _logicLine = output;
   _outLine   = APISock;
   _exit      = exit;

   _debug = debug;
   // setup transmitter with serialport.
   _trans = new Transmitter(usb, debug);
   _track = track;
}


/**DESTRUCTOR: FREE'S THE LEFTOVER MEMORY AND CLOSES THE DEBUG FILE IF OPENED. */
Conductor::~Conductor() {
   delete _trans;
}


/** STATIC METHOD TO BEGIN THE THREAD FROM MAIN. */
void Conductor::start_thread(void* c) {
   //run the Conductor loop from main concurrently with rest of program.
   ((Conductor*) c)->conduct_loop();
}


/** MAIN LOOP OF THE Conductor. CONTINOUSLY LISTENS ON _pipefd
    FOR ANY HUMAN PROTOCOL MESSAGE. IF TRAFFIC CONCAT TO BUFFER
    THEN TRANSFORM INTO HEX LOCONET MESSAGE AND SEND. */
void Conductor::conduct_loop() {
   while(!(*_exit)) {
      // String created and destroyed each time due to fix bug.
      std::string receveidStr;
      if (!_inLine->waitForMail(&receveidStr)) {
         _recvdLine.add((char *)receveidStr.c_str());
         while (_recvdLine.size() > 0) {
            parseRecvd();
         }
      }
   }
}

/** Parses _recvdLine. Gets first proper LOCOPROTO message in _recvdLine *
  * and then does whatever is appropriate for said message               */
void Conductor::parseRecvd(){
   char         *cmd, *arg1, *arg2;  // various arguments to be parsed
   int          ix, cmdNum;          // index for loops, number of the command
   string       key;                 // used exclusively for getline
   stringstream cStream(CMDS);       // turns CMDS into a stream for getline
   stringstream rStream(RCVS);       // turns RVCS into a stream for getline

   // Set default values for parsed values, then get the cmd
   cmd = arg1 = arg2 = NULL;
   cmd = _recvdLine.get_frto('[', (char *) "|]", 4);
   if (cmd == NULL) {                // no value found
      _recvdLine.clear_thru('[');    // used frto, so either there was a [
      _recvdLine.clear_upto('[');    // and no 3 char cmd, or there wasn't
      return;
   }
   _recvdLine.clear_thru('[');       // Clear the first [ from buffer
   // Set default cmdNum for error checking.
   cmdNum = -1;
   if (_debug) {
      cout << "CND: Message received from API: " << cmd << endl;
   }
   // getline takes the first bit separated by the third argument of the
   // stream and returns it in key, so we're going until the stream has
   // nothing left, incrementing ix as we go
   for (ix = 0; getline(cStream, key, ' '); ix++) {
      if (!(strncmp(key.c_str(), cmd, 3))) {
         cmdNum = ix;
         break;
      }
   }
   for (ix = 0; getline(rStream, key, ' '); ix++) {
      if (!(strncmp(key.c_str(), cmd, 4))) {
         cmdNum = ix+11;
         break;
      }
   }
   if (cmd) {
      delete[] cmd;
      cmd = NULL;
   }
   if (cmdNum == -1) {               // unable to find known command
      _recvdLine.clear_upto('[');
      delete[] cmd;
      return;
   }
   if (cmdNum < 4) {                 // <4 indicates POW, STP, RQS, TKN (single arg)
      if (_recvdLine.clear_thru('|') == 0) {
         _recvdLine.clear_upto('['); // if clear_thru returns 0, was unable to
         delete[] cmd;               // find a |, which is necessary for these
         return;
      }                              // only one argument, get it
      arg1 = _recvdLine.get_to((char *) "]", 4);
      if (arg1 == NULL) {
         _recvdLine.clear_upto('[');
         delete[] cmd;
         delete[] arg1;
         return;
      }
   } else if (cmdNum != C_EXIT) {         // >4 <10 indicates SPD, DIR, SND, TOT, LSL, LTO (double arg)
      if (_recvdLine.clear_thru('|') == 0) {
         _recvdLine.clear_upto('['); // see above
         delete[] cmd;
         return;
      }
      arg1 = _recvdLine.get_to((char *) "|", 4);
      if (_recvdLine.clear_thru('|') == 0) {
         _recvdLine.clear_upto('[');
         delete[] cmd;
         if (arg1) {delete[] arg1;}
         return;
      }
      arg2 = _recvdLine.get_to((char *) "]", 4);
      if (arg1 == NULL || arg2 == NULL) {
         _recvdLine.clear_upto('[');
         delete[] cmd;
         if (arg1) {delete[] arg1;}
         if (arg2) {delete[] arg2;}
         return;
      }
   }

   // all arguments parsed, clear to next argument
   _recvdLine.clear_upto('[');

   int state;
   switch (cmdNum) {
      case C_POWER:
         _trans->send_power(arg1);
         break;
      case C_STOP:
         _trans->send_stop(arg1);
         break;
      case C_REQUEST:
         _trans->send_request(arg1);
         break;
      case C_TOKEN:
         break;
      case C_SPEED:
         _trans->send_speed(arg1, arg2);
         break;
      case C_DIRECTION:
         if (!_track->is_train_locked(atoi(arg1))) {
            _trans->send_direction(arg1, arg2);
         }
         break;
      case C_SOUND:
         _trans->send_sound(arg1, arg2);
         break;
      case C_TURNOUT:
         if (!_track->is_turnout_locked(atoi(arg1))) {
            _trans->send_turnout(arg1, arg2);
         }
         break;
      case C_LOCK_TRAIN:
         state = strncmp(arg2, "U", 1) ? 0 : 1;
         _track->lock_train(atoi(arg1), state);
         break;
      case C_LOCK_TURNOUT:
         state = strncmp(arg2, "U", 1) ? 0 : 1;
         _track->lock_turnout(atoi(arg1), state);
         break;
      case C_EXIT:
         *_exit = true;
         _inLine->stopDelivery();
         _logicLine->stopDelivery();
         cout<<"CND: Finished conducting. Terminating."<<endl;
         write(_outLine, "[TERM]", 6);
         break;
      case R_POWER:
         // TODO no power message gets sent from receiver.
         // Whether or not it needs to be dealt with is not relevant.
         // This here incase someone wants to deal with it later.
         break;
      case R_TURNOUT:
         state = strncmp(arg2,"C",1) ? 1 : -1;
         _track->move_turnout(atoi(arg1),state);
         break;
      case R_SPEED:
         _track->chspd_train(atoi(arg1),atoi(arg2));
         break;
      case R_SENSOR:
         _logicLine->sendMail(std::string("[ISEN|") + arg1 + std::string("]"));
         break;
      case R_SOUND:
         // TODO sound messages don't matter atm.
         // Perhaps there would be something useful later on.
         break;
      case R_DIRECTION:
         state = strncmp(arg2, "F", 1) ? 1 : -1;
         _track->chdir_train(atoi(arg1),state);
         break;
   }
   if (arg1) {
      delete[] arg1;
      arg1 = NULL;
   }
   if (arg2) {
      delete[] arg2;
      arg2 = NULL;
   }
}
