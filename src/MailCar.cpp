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

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <thread>
#include <iostream>
#include "MailCar.h"
#include "Coupling.h"
#include "constants.h"

// Creates a MailCar object.
// No arguments given indicates we don't want the socket functionality.
MailCar::MailCar() {
   _sockFD = 0;
   _exit = 0;
}

// Creates a MailCar object that will listen to a socket.
// Must provide a WORKING socket.
MailCar::MailCar(int socket) {
   _sockFD = socket;
   _exit = 0;
   if (_sockFD) {
      _coupling = new Coupling(&_mail, &_mtx, &_cv, _sockFD);
      _couplingThread = new std::thread(Coupling::start_thread, _coupling);
   }
}

// Deconstructs the MailCar
// If the socket given was 0, or there was no socket given, does nothing extraordinary.
MailCar::~MailCar() {
   if (_sockFD) {
      close(_sockFD);
      _coupling->exit();
      _couplingThread->join();
      delete _couplingThread;
      delete _coupling;
   }
}

// Takes a string object (the message to be sent) and returns
// an integer specifying success (0) or failure(!0).
int MailCar::sendMail(std::string message) {
   if (!message.compare("")) {
      return ERR_NOMSG;
   }
   std::unique_lock<std::mutex> lck (_mtx);  // Lock object. Locks mutex on creation, unlocks on destruction.
   _mail.push(message);	                     // push message onto queue
   _cv.notify_one();		                     // notify one consumer that there is a product ready
   return RET_SUCCESS;
}

// Takes a char * (message to be sent) and returns an integer
// specifying success (0) or failure (!0).
int MailCar::sendMail(char * message) {
   if (message == nullptr) {
      return ERR_NOMSG;
   }
   std::string msgStr(message);	            // string object for pushing onto stack
   std::unique_lock<std::mutex> lck (_mtx);  // lock mutex
   _mail.push(msgStr);		                  // push then notify
   _cv.notify_one();
   return RET_SUCCESS;
}

// Takes a string object and a value representing priority and returns
// an integer specifying success (0) or failure (!0)
int MailCar::sendMail (int prior, std::string message) {
   if (!message.compare("")) {
      return ERR_NOMSG;
   }
   std::unique_lock<std::mutex> lck (_mtx);  // Lock object. Locks mutex on creation, unlocks on destruction.
   if (prior == 0) {                         // zero indicates no priority given
      _mail.push(message);                   // push message onto low priority queue
   } else {
      _expressMail.push(message);
   }
   _cv.notify_one();                         // notify one consumer that there is a product ready
   return RET_SUCCESS;
}

// Takes a char * (message to be sent) and a value representing priority
// and returns an integer specifying success (0) or failure (!0)
int MailCar::sendMail (int prior, char * message) {
   if (message == nullptr) {
      return ERR_NOMSG;
   }
   std::string msgStr(message);               // string object for pushing onto stack
   std::unique_lock<std::mutex> lck (_mtx);   // lock mutex
   if (prior == 0) {                          // zero indicates no priority given
      _mail.push(msgStr);                     // push then notify
   } else {
      _expressMail.push(msgStr);
   }
   _cv.notify_one();
   return RET_SUCCESS;
}

// Takes a pointer to a string object where the message is to be placed
// if found.  Checks for message, returns 0 if found, otherwise !0.
int MailCar::checkMail(std::string *message) {
   std::unique_lock<std::mutex> lck (_mtx);         // lock mutex
   if ((_mail.size() + _expressMail.size()) > 0) {  // check for items in queue
      if (_expressMail.size() > 0) {
         if (message != nullptr) {
            message->assign(_expressMail.front());	  // assign value to output
            _expressMail.pop();                      // and pop the item from queue
            return RET_SUCCESS;
         }
         return RET_YGMAIL;
      } else {
         if (message != nullptr) {
            message->assign(_mail.front());          // assign value to output
            _mail.pop();                             // and pop the item from queue
            return RET_SUCCESS;
         }
         return RET_YGMAIL;
      }
      return ERR_MAILNF;
   }
   return RET_NOMAIL;
}

// Takes a pointer to a string object where the message is to be placed.
// This is a blocking function call.  Will return RET_SUCCESS when found. A non zero
// return value indicates error.
int MailCar::waitForMail(std::string *message) {
   std::unique_lock<std::mutex> lck (_mtx);            // lock mutex
   while (((_mail.size() + _expressMail.size()) < 1) && !(_exit)) {  // while there is less than 1 item in queue wait
      _cv.wait(lck);
   }
   if (!_exit) {
      if (_expressMail.size() > 0) {
         if (message != nullptr) {
            message->assign(_expressMail.front());        // something is there now, we've got the mutex
            _expressMail.pop();                           // so assign value, then pop from queue
            return RET_SUCCESS;
         } else {
            return RET_YGMAIL;
         }
      } else {
         if (message != nullptr) {
            message->assign(_mail.front());               // lower value target
            _mail.pop();
            return RET_SUCCESS;
         } else {
            return RET_YGMAIL;
         }
      }
   }
   return ERR_MAILNF;
}

// Ensures that we're able to unblock a waitForMail call.
void MailCar::stopDelivery() {
   _exit = 1;
   _cv.notify_all();
}