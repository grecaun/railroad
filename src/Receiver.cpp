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


   RECIEVES HEX CODE TRANSMITTED THROUGH LOCO-BUFFER-USB VIA THE SERIAL
   PORT. THEN CONVERTS THE HEX INTO A READABLE PROTOCOL BEFORE SENDING
   IT MESSAGE ON THROUGH THE PIPE. ALSO FILTERS THROUGH INCOMING NOISE
   AND THROWS OUT GARBAGE MESSAGES.*/

#include <sys/time.h>
#include "Receiver.h"

unsigned long long current_timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);                            // get current time
    unsigned long long ms = (tv.tv_usec / 1000) + (tv.tv_sec*1000); // caculate milliseconds
    return ms;
}

/**CONSTRUCTOR: TAKES SERIALPORT FOR COMM FROM LOCOBUFFER, PIPE TO WRITE OUT COMMANDS
   CONVERTED FROM HEX, BOOLEAN FOR A SIGNAL TO EXIT, AND A DEBUG FLAG FOR DEBUG MODE.*/
Receiver::Receiver(SerialPort* usb, MailCar *output, int numSens, atomic<bool>* exit, int debug)
   :_outLine(output), _usb(usb),_exit(exit) {
   if(debug) {
      _debugfile = new ofstream("../debug/rx_debug.txt");
   } else {
      _debugfile = NULL;
   }
   _sensTimers = new unsigned long long[numSens+1];
}


/**DESTRUCTOR: CLOSE THE COMM PIPE AND IF IN DEBUG MODE CLOSE THE FILE
   AND DELETE THE OFSTREAM.*/
Receiver::~Receiver() {
  if (_sensTimers != NULL) {
    delete[] _sensTimers;
  }
   if(_debugfile != NULL) {
      _debugfile->close();
      delete _debugfile;
   }
}


/** STATIC METHOD TO BEGIN THE THREAD FROM MAIN. */
void Receiver::start_thread(void* r) {
   // run the transmitter loop from main concurrently with rest of program.
   ((Receiver*) r)->recieve_loop();
}


/** MAIN LOOP OF RECEIVER. CONTINUOUSLY READS FROM USB SERIAL PORT. ONCE
    DATA IS FOUND, SHOVES ON BUFFER THEN GOES THROUGH ONE BYTE AT A TIME
    AND FINDS MESSAGES. AFTER BEING FOUND CREATES READABLE MESSAGES IN
    PROTOCOL AND SENDS THEM THROUGH PIPE TO SPECIFIED PROCESS. */
void Receiver::recieve_loop() {
	int    msglen;
	byte   *opcode, *message;
	string command;
	int    portFD = _usb->get_port();

	/* Values for watching for messages from Train. */
	fd_set rfds;
	struct timeval tv;
	int retval;

	while(!(*_exit)) {
    /* Zero out rfds, add the FD for the port to the set. */
    FD_ZERO(&rfds);
    FD_SET(portFD, &rfds);
    /* Set wait time for select statement (5 seconds). */
    tv.tv_sec  = 5;
    tv.tv_usec = 0;
		retval = select(portFD+1, &rfds, NULL, NULL, &tv);
		if (retval < 0) {
			perror("RCV: Select");
      *_exit = true;
		} else if (retval > 0) {
		 	opcode = _usb->recieve(1);
	  	//opcode recieved.
	  	if(opcode != NULL) {
	     	// compute expected length of complete message
	     	msglen = compute_msglen(0, opcode[0]);

	     	// N length message. need next byte to check length
	     	if(msglen == 0) {
        	// recieve second size opcode.
         	byte* lencode = _usb->recieve(1);

         	if(lencode != NULL) {
          	//update msglen with actual.
            msglen = compute_msglen(1, lencode[0]);
          } else {
       		  delete[] opcode;
       		  continue;
         	}
	     	} else if (msglen < 0) {  // unable to compute message length
          delete[] opcode;
          continue;
        }

	     	// message is not in usb buffer, remove and continue.
	     	if((message = _usb->recieve(msglen)) == NULL) {
	        	delete[] opcode;
	        	continue;
	     	}

	     	// removes useless commands from usb buffer.
	     	if(is_useless(opcode[0])) {
	        	delete[] opcode;
	        	delete[] message;
	        	continue;
	     	}

	     	// break down opcode,message into command to be sent via MailCar.
	     	command = breakdown_command(opcode[0], message);
        if (command.size() > 0) {
          if (_debugfile) {
            cout<<"RCV: Message to CND: "<<command<<endl;
          }
          send(command);
        }

	     	delete[] message;
	     	delete[] opcode;
		 	}
		}
	}
  cout<<"RCV: Done receiving. Quitting." << endl;
}


/** SEND A CHAR ARRAY THROUGH THE MAILCAR GIVEN TO CONSTRUCTOR. */
int Receiver::send(string command) {
  // send mail through mailcar with priority of 3.
  if(_outLine->sendMail(3, command) != 0) {
    cout<<"RCV: Sending "<<command<<"."<<endl;
    return -1;
  }
  return 0;
}


/** GIVEN THE OPCODE OF A COMMAND RETURNS WHETHER THAT COMMAND
   IS USELESS OR NOT TO PARSE. */
int Receiver::is_useless(byte opcode) {
   int i;
   byte usefull[NUM_USEFULL] = OP_USEFULL;

   //checks to see if opcode is in list of usefull.
   for(i=0; i<NUM_USEFULL; i++) {
      if(opcode == usefull[i]) {
        return 0;
      }
   }

   //if not return useless.
   return 1;
}


/** COMPUTES THE EXPECTED LENGTH OF THE MESSAGE. IF OPTYPE IS 0, OPCODE
    IS THE FIRST OPCODE BYTE, IF OPTYPE IS 1, THEN OPCODE IS THE SECONDE
    OPCODE BYTE (N BYTE MESSAGE). RETURNS 0 IF NEEDS THE SECOND OPCODE
    BYTE TO COMPUTE MESSAGE. RETURN -1 IF ERROR AND OPCODE INVALID,
    OTHERWISE RETURNS EXPECTED MESSAGE SIZEE OTHERWISE.*/
int Receiver::compute_msglen(int optype, byte opcode) {
    // if 1st opcode byte
    if(optype == 0) {
      // gather opcode message size bits
      byte numcode = opcode & 0xE0;
      if (numcode == 0x80) {         // command size 2
        return 1;
      } else if (numcode == 0xA0) {  // command size 4
        return 3;
      } else if (numcode == 0xC0) {  // command size 6
        return 5;
      } else if (numcode == 0xE0) {  // command size n
        return 0;
      }
      return -1;
    } else if(optype == 1) {  // second opcode byte
      // gather n byte from second opcode byte
      return ((int) opcode & 0x7F) - 2;
    }
    return -1;
}


/** CREATES THE Receiver TO CONDUCTOR MESSAGE OUT OF THE GIVEN
   OPCODE AND MESSAGE. */
string Receiver::breakdown_command(byte opcode, byte* message) {
   string output;
   switch ((int)opcode) {
      case OP_POW:
        break;
        return conv_pow(message);
      case OP_SENSOR:
        return conv_sensor(message);
      case OP_TURNOUT:
         return conv_turnout(message);
      case OP_LOCO_SPEED:
         return conv_loco_speed(message);
      case OP_LOCO_FUNC_1:
         return conv_loco_func1(message);
      case OP_LOCO_FUNC_2:
         return conv_loco_func2(message);
      default:
         return output;
   }
   return output;
}


/** CONVERTS THE BYTES INTO POWER LOCOPROTO MESSAGE */
string Receiver::conv_pow(byte* message) {
   stringstream ss;

   if(message[3] == 0x12) {
      ss<<"[IPOW|ON]\0";
   } else if(message[3] == 0x1D) {
      ss<<"[IPOW|OFF]\0";
   } else {
      string s;
      return s;
   }
   return ss.str();
}


/** CONVERTS THE BYTES INTO SENSOR LOCOPROTO MESSAGE */
string Receiver::conv_sensor(byte* message) {
   stringstream ss;

   //convert byte one into decimal
   int dec = (hextodec(message[0]) * 2) + 1;

   if ((message[1] & 0x20) == 0x20) {
      dec++;
   }

   if (_sensTimers[dec] + TIMEBTWNTICK > current_timestamp() ) {
    return "";
   }
   ss<<"[ISEN|"<<dec;

   if((message[1] & 0x10) == 0x10) {
      return "";
      //ss<<"|HI]";
   } else {
      _sensTimers[dec] = current_timestamp();
      ss<<"|LO]";
   }

   return ss.str();
}


/** CONVERTS THE BYTES INTO TURNOUT LOCOPROTO MESSAGE */
string Receiver::conv_turnout(byte* message) {
   stringstream ss;

   int dec = hextodec(message[0])+1;

   ss<<"[ITOT|"<<dec;

   if((message[1] & 0x10) == 0x10) {
      ss<<"|C]";
   } else {
      ss<<"|T]";
   }

   return ss.str();
}


/** CONVERTS THE BYTES INTO LOCO SPEED LOCOPROTO MESSAGE */
string Receiver::conv_loco_speed(byte* message) {
   stringstream ss;

   int slot = hextodec(message[0]);
   int speed = hextodec(message[1]);//no compile error

   if (speed == 1) {
    speed--;
   }

   ss<<"[ISPD|"<<slot<<"|"<<speed<<"]";

   return ss.str();
}


/** CONVERTS THE BYTES INTO LOCO FUNCTION LOCOPROTO MESSAGE */
string Receiver::conv_loco_func1(byte* message) {
   stringstream ss;

   int slot = hextodec(message[0]);//no compile error

   ss<<"[IDIR|"<<slot<<"|"<<((message[1]==OP_LOCO_FWD)?"F]":"R]");

   return ss.str();
}


/** CONVERTS THE BYTES INTO LOCO FUNCTION LOCOPROTO MESSAGE */
string Receiver::conv_loco_func2(byte* message) {
   stringstream ss;

   int slot = hextodec(message[0]);//no compile error

   ss<<"[ISND|"<<slot<<"|"<<((message[1]==OP_SND_ON)?"ON]":"OFF]");

   return ss.str();
}


int Receiver::hextodec(byte hex) {
  return (int) hex;
}


/** IF DEBUG FLAG SET DURING CONSTRUCTION WILL PRINT OUT EVERY COMMAND
    SENT THROUGH TRANSMITTER IN FORMAT [CMD|ARG1|ARG2] TO DEBUG FILE.*/
void Receiver::print_debug(char* cmd, char* arg1, char* arg2) {
   *_debugfile<<"["<<cmd<<"|"<<arg1;

   if(arg2 == NULL) {
      *_debugfile<<"|"<<arg2;
   }

   *_debugfile<<"]"<<endl;
}
