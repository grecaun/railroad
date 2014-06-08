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

#ifndef __MAILCAR_H__
#define __MAILCAR_H__

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <thread>
#include "Coupling.h"

class MailCar {
  public:
   int sendMail(std::string  message);
   int sendMail(char* message);
   int sendMail(int prior, std::string message);
   int sendMail(int prior, char* message);
   int checkMail(std::string* message);
   int waitForMail(std::string* message);
   void stopDelivery();
   MailCar();
   MailCar(int socket);
   ~MailCar();

  private:
   std::mutex              _mtx;
   std::queue<std::string> _mail;
   std::queue<std::string> _expressMail;
   std::condition_variable _cv;
   int                     _exit;

   int _sockFD;
   Coupling* _coupling;
   std::thread* _couplingThread;
};

#endif
