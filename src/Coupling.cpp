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

#include "Coupling.h"
#include <signal.h>

// A coupling constructor with no arguments should do nothing when run is called.
Coupling::Coupling() {
   _keepAlive = 0;
}

// Creates a coupling object. This object is used to translate messages sent via sockets
// into messages sent via the MailCar.
Coupling::Coupling(std::queue<std::string>* queue,
		   std::mutex* mutex,
		   std::condition_variable * condVar,
		   int socket) {
   _mail = queue;
   _mtx = mutex;
   _cv = condVar;
   _keepAlive = 1;
   _sockFD = socket;
}

// Coupling needs to run as its own thread, therefore it has its own run method.
// Essentially reads from a socket, pushes it onto the queue given to it
// making sure to lock the mutex when it does so. Also notifies a reader if necessary.
void Coupling::run(){
   char buffer[1024];	// buffer for reading from socket
   int numRead = 0;	   // keeping track of number of chars read
   std::unique_lock<std::mutex>	lck (*_mtx, std::defer_lock); // lock obj for mutex

   /* Values for watching for messages. */
   fd_set rfds;
   struct timeval tv;
   int retval;

   while (_keepAlive) {
      /* Zero out rfds, add the FD to the set. */
      FD_ZERO(&rfds);
      FD_SET(_sockFD, &rfds);
      /* Set wait time for select statement (5 seconds). */
      tv.tv_sec  = 5;
      tv.tv_usec = 0;
      retval = select(_sockFD+1, &rfds, NULL, NULL, &tv);
      if (retval < 0) {
         perror("MLC: Select");
         _keepAlive = 0;
      } else if (retval > 0) {
         numRead = read(_sockFD, buffer, 1024);
         if (numRead < 0) {
            continue;           // error from read
         } else if (!numRead) { // EOF received
            break;
         }
         buffer[numRead] = 0;
         std::string msgStr(buffer); // string object for pushing onto stack
         lck.lock();	                // lock mutex
         _mail->push(msgStr);        // push then notify
         _cv->notify_one();
         lck.unlock();               // need explicit unlock here
      }
   }
   return;
}

// Tells the coupling it should exit its run loop.
void Coupling::exit() {
   _keepAlive = 0;
   return;
}

// Function to pass to thread creation.
void Coupling::start_thread(void* t) {
   ((Coupling*) t)->run();
   return;
}
