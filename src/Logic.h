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

#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "MailCar.h"
#include "Track.h"

class Logic {
  public:
    static void start_thread(void* t);
    Logic(MailCar *output, MailCar *input, Track* track, atomic<bool>* exit, int debug);
    ~Logic();
    void LogicLoop();

  private:
    MailCar*      _inCar;
    MailCar*      _outCar;
    Track*        _track;
    atomic<bool>* _exit;
    int           _debug;
};

#endif  // __LOGIC_H__