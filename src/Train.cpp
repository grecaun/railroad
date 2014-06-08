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

#include "Train.h"

Train::Train(int slot, int num) {
   _slot      = slot;
   _number    = num;
   _speed     = 0;
   _direction = 1;
}

Train::~Train() {
   /**TODO ADD*/
}

int Train::isLocked() {
    return 0;
}

int Train::lock() {
    return 0;
}

int Train::unlock() {
    return 0;
}

bool Train::isAssoc(int sens) {
    return false;
}

Section* Train::getAssoc(int sens) {
    return nullptr;
}

int Train::reserve(int numahead, int sens) {
    return 0;
}

int Train::free(int numbehind, int sens) {
    return 0;
}

int Train::getSpeed() {
    return _speed;
}

void Train::updSpeed(int speed) {
  _speed = speed;
}

int Train::getDir() {
  return _direction;
}

void Train::changeDir(int dir) {
  if (dir > 0) {
    _direction = 1;
  } else {
    _direction = -1;
  }
}