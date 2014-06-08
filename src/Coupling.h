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

#ifndef __COUPLING_H__
#define __COUPLING_H__

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <unistd.h>

class Coupling {
  public:
   void run();
   static void start_thread(void* t);
   void exit();
   Coupling();
   Coupling(std::queue<std::string>* queue,
	    std::mutex* mutex,
	    std::condition_variable * condVar,
	    int socket);

  private:
   int _sockFD;
   int _keepAlive;

   std::queue<std::string>* _mail;
   std::mutex* _mtx;
   std::condition_variable* _cv;
};

#endif
