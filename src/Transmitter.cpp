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

#include "Transmitter.h"
#include "opcodes.h"

/**CONSTRUCTOR: TAKES SERIALPORT FOR COMM TO LOCOBUFFER, PIPE TO READ IN COMMANDS TO
   CONVERT TO HEX, BOOLEAN FOR A SIGNAL TO EXIT, AND A DEBUG FLAG FOR DEBUG MODE. */
Transmitter::Transmitter(SerialPort* usb, int debug) {
   _size = 0;
   _usb = usb;
   _slotAvail = 4;

   if(debug)
      _debugfile = new ofstream("../debug/tx_debug.txt");
   else
      _debugfile = NULL;
}

/**DESTRUCTOR: FREE'S THE LEFTOVER MEMORY AND CLOSES THE DEBUG FILE IF OPENED. */
Transmitter::~Transmitter() {
   //delete _buf;

   //close debugfile if open
   if(_debugfile != NULL) {
      _debugfile->close();
      delete _debugfile;
   }
}

/* Takes a properly formatted packet of numbytes and sends it via the   *
 * usb connection given when transmitter created.                       */
int Transmitter::send_packet(int numbytes, byte* packet) {
   //send the packet to usb port.
   if(packet != NULL) {
      _usb->send(packet, numbytes);
      return 1;
   }
   return 0;
}

/* Given a state to set the power to, transmits proper command        */
void Transmitter::send_power(char* state) {
   byte* packet = conv_power(state);
   int sent = send_packet(2, packet);
   if (sent && _debugfile != NULL) {
         print_debug( (char *) "pow", state, NULL);
   }
   if (packet) {delete[] packet;}
}

/* Given a proper address, transmits request for control of train     */
void Transmitter::send_request(char* addr) {

   byte fre[3] = {0xB5, 0, 0x03};
   byte* freeSl = NULL;
   byte* rqst = conv_request(addr);
   char *slot1 = (char*)std::to_string(_slotAvail).c_str();
   char *slot2 = (char*)std::to_string(_slotAvail).c_str();
   byte* move = conv_move_slot(slot1,slot2);

   fre[1] = _hex.decFFhex(_slotAvail);
   freeSl = _hex.create_packet(fre, 3);

   int freeSent = send_packet(4, freeSl);
   int rqstSent = send_packet(4, rqst);
   int moveSent = send_packet(4, move);
   if ((freeSent || rqstSent || moveSent) && _debugfile != NULL) {
      print_debug( (char *) "rqs", addr, NULL);
      print_debug( (char *) "mov", slot1, slot2);
   }
   if (rqst)   {delete[] rqst;}
   if (freeSl) {delete[] freeSl;}
   if (move)   {delete[] move;}

   _slotAvail++;
}

/* Transmits request to move slot 1 to slot 2.                        */
void Transmitter::send_move(char* slot1, char* slot2) {
   byte* packet = conv_move_slot(slot1, slot2);
   int sent = send_packet(4, packet);
   if (sent && _debugfile != NULL) {
      print_debug( (char *) "mov", slot1, slot2);
   }
   if (packet) {delete[] packet;}
}

/* Transmits request for turnout to be set to state                   */
void Transmitter::send_turnout(char* turnout, char* state) {
   byte* packet = conv_turnout(turnout, state);
   int sent = send_packet(8, packet);
   if (sent && _debugfile != NULL) {
         print_debug( (char *) "tot", turnout, state);
   }
   if (packet) {delete[] packet;}
}

/* Transmits request for train in slot to be set to speed             */
void Transmitter::send_speed(char* slot, char* speed) {
   byte* packet = conv_speed(slot, speed);
   int sent = send_packet(4, packet);
   if (sent && _debugfile != NULL) {
         print_debug( (char *) "spd", slot, speed);
   }
   if (packet) {delete[] packet;}
}

/* Transmits request for train in slot to stop                        */
void Transmitter::send_stop(char* slot) {
   byte* packet = conv_stop(slot);
   int sent = send_packet(4, packet);
   if (sent && _debugfile != NULL) {
         print_debug( (char *) "stp", slot, NULL);
   }
   if (packet) {delete[] packet;}
}

/* Transmits request for train in slot to go in dir                   */
void Transmitter::send_direction(char* slot, char* dir) {
   byte* packet = conv_direction(slot, dir);
   int sent = send_packet(4, packet);
   if (sent && _debugfile != NULL) {
         print_debug( (char *) "dir", slot, dir);
   }
   if (packet) {delete[] packet;}
}

/* Transmits request for train in slot to set state of sount to state */
void Transmitter::send_sound(char* slot, char* state) {
   byte* packet = conv_sound(slot, state);
   int sent = send_packet(4, packet);
   if (sent && _debugfile != NULL) {
         print_debug( (char *) "snd", slot, state);
   }
   if (packet) {delete[] packet;}
}

/* Byte Translation Code Follows */
/** RETURN GLOBAL POWER STATE REQUEST HEX */
byte* Transmitter::conv_power(char* onoroff) {
   byte pow[1] = {0};
   if(!strncmp(onoroff, "ON", 2)) {
      pow[0] = 0x83;

      byte fre[3] = {0xB5, 0, 0x03};
      byte* free = NULL;
      byte* rqst = NULL;
      byte* move = NULL;

      //free all slots up to MAX_SLOTS and then requests and uses them
      // for addresses 411=slot1, 420=slot2, 1204=slot3
      for(int i=1; i<MAX_SLOTS+1; i++) {
         fre[1] = _hex.decFFhex(i);
         free = _hex.create_packet(fre, 3);

         if(i == 1) {
            rqst = conv_request((char*)"411\0");
            move = conv_move_slot((char*)"1\0", (char*)"1\0");
         }
         else if(i == 2) {
            rqst = conv_request((char*)"420\0");
            move = conv_move_slot((char*)"2\0", (char*)"2\0");
         }
         else if(i == 3) {
            rqst = conv_request((char*)"1204\0");
            move = conv_move_slot((char*)"3\0", (char*)"3\0");
         }

          //send out commands to free slots, request, and use them.
          _usb->send(free, 4);
          _usb->send(rqst, 4);
          _usb->send(move, 4);
          delete[] rqst;
          delete[] move;
          delete[] free;
      }
   } else if(!strncmp(onoroff, "OFF", 3)) {
      pow[0] = 0x82;
   } else {
      return NULL;
   }

   return _hex.create_packet(pow, 1);
}

/** RETURN ADDRESS REQUEST HEX */
byte* Transmitter::conv_request(char* addr) {
   int adr = atoi(addr);
   byte req[3] = {0xBF, 0, 0};

   //convert address to a hi and lo sub address.
   req[1] = _hex.decFFhex_hi(adr);
   req[2] = _hex.decFFhex_lo(adr);
   return _hex.create_packet(req, 3);
}

/** RETURN MOVE SLOT FROM SLOT1 TO SLOT2 HEX */
byte* Transmitter::conv_move_slot(char* slot1, char* slot2) {
   int sl1 = atoi(slot1);
   int sl2 = atoi(slot2);
   byte mov[3] = {0xBA, 0, 0};

   //convert slot#'s to hex.
   mov[1] = _hex.decFFhex(sl1);
   mov[2] = _hex.decFFhex(sl2);
   return _hex.create_packet(mov, 3);
}

/** RETURN SPEED STATE CHANGE TO LOCO HEX */
/** ACCEPTS SPEED FROM 0(STOP) TO 126(FULL SPEED)*/
byte* Transmitter::conv_speed(char* slot, char* speed) {
   int slt = atoi(slot);
   int spd = atoi(speed);
   byte sped[3] = {0xA0, 0, 0};

   if(spd < 0 || spd > 126) //speed range 0 to 126.
      return NULL;
   if(spd > 0 && spd < MIN_SPEED) //spd 1 is of emergency stop, set to 2.
      spd = MIN_SPEED;

   //convert slot and speed to hex.
   sped[1] = _hex.decFFhex(slt);
   sped[2] = _hex.decFFhex(spd);
   return _hex.create_packet(sped, 3);
}

/** RETURN EMERGENCY STOP REQUEST TO LOCO HEX */
/** OVERIDES THE LOCK FUNCTION OF THE SLOTS */
byte* Transmitter::conv_stop(char* slot) {
   int slt = atoi(slot);

   if(slt <= 0) {
      byte p[3] = {0xA0, 0x00, 0x01};
      byte* packet = NULL;

      //sets all slots to emergency stop
      for(int i=0; i<MAX_SLOTS; i++) {
         p[1] = _hex.decFFhex(i+1);
         packet = _hex.create_packet(p, 3);
         _usb->send(packet, 4);
         delete[] packet;
      }
   }
   else { //set given slot to emergency stop
      byte sped[3] = {0xA0, 0, 0x01};

      //convert slot and speed to hex.
      sped[1] = _hex.decFFhex(slt);
      return _hex.create_packet(sped, 3);
   }

   return NULL;
}

/** RETURN DIR STATE CHANGE TO LOCO HEX */
byte* Transmitter::conv_direction(char* slot, char* dir) {
   char d = dir[0];
   int slt = atoi(slot);
   byte dirt[3] = {0xA1, 0, 0};

   dirt[1] = _hex.decFFhex(slt);

   if(d == 'F')
      dirt[2] = OP_LOCO_FWD;
   else if(d == 'R')
      dirt[2] = OP_LOCO_REV;
   else
      return NULL;

   return _hex.create_packet(dirt, 3);
}

/** RETURN TURNOUT CHANGE STATE REQUEST HEX */
byte* Transmitter::conv_turnout(char* turnout, char* torc) {
   char toc = torc[0];
   int tout = atoi(turnout)-1; //map# vs. actual# varies by 1.

   //check if for available turnout number.
   if(tout < 0 || tout > MAX_TURNOUTS-1)
      return NULL;

   byte t1[3] = {0xB0, 0, 0};
   byte t2[3] = {0xB0, 0, 0};

   if(toc == 'T') {
      t1[2] = 0x10;
      t2[2] = 0x00;
   }
   else if(toc == 'C') {
      t1[2] = 0x30;
      t2[2] = 0x20;
   }
   else
      return NULL;

   //convert turnout# to hex.
   t1[1] = t2[1] = _hex.decFFhex(tout);
   byte* to1 = _hex.create_packet(t1, 3);
   byte* to2 = _hex.create_packet(t2, 3);

   //create a dual packet to send and return;
   byte* msg = new byte[8];
   for(int i=0; i<4; i++) {
      msg[i] = to1[i];
      msg[i+4] = to2[i];
   }

   //free unused packets.
   delete[] to1;
   delete[] to2;

   return msg;
}

/** RETURN LOCO CHANGE NOISE STATE REQUEST HEX */
byte* Transmitter::conv_sound(char* slot, char* onoroff) {
   int slt = atof(slot);
   byte snd[3] = {0xA2, 0, 0};

   if(!strncmp(onoroff, "ON", 2))
      snd[2] = OP_SND_ON;
   else if(!strncmp(onoroff, "OFF", 3))
      snd[2] = OP_SND_OFF;
   else
      return NULL;

   //convert slot# to hex.
   snd[1] = _hex.decFFhex(slt);
   return _hex.create_packet(snd, 3);
}

/* Debug Code Follows */
/** IF DEBUG FLAG SET DURING CONSTRUCTION WILL PRINT OUT EVERY COMMAND
    SENT THROUGH TRANSMITTER IN FORMAT [CMD|ARG1|ARG2] TO DEBUG FILE.*/
void Transmitter::print_debug(char* cmd, char* arg1, char* arg2) {
   *_debugfile<<"["<<cmd<<"|"<<arg1;

   if(arg2 == NULL)
      *_debugfile<<"|"<<arg2;

   *_debugfile<<"]"<<endl;
}