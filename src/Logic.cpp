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

#include <string>
#include <iostream>
#include "Logic.h"

Logic::Logic(MailCar *output, MailCar *input, Track* track, atomic<bool>* exit, int debug) {
  _inCar  = input;
  _outCar = output;
  _track  = track;
  _exit   = exit;
  _debug  = debug;
}

Logic::~Logic() {
  // TODO add anything necessary
}

void Logic::start_thread(void *t) {
  ((Logic*)t)->LogicLoop();
}

void Logic::LogicLoop() {
  // TODO add logic here
  while (!(*_exit)) {
    // String created and destroyed each time due to fix bug.
    std::string conductString;
    if (!_inCar->waitForMail(&conductString)) {
      cout << conductString << endl;
    }
  }
}