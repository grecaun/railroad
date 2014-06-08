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

#include "Turnout.h"
#include "constants.h"

Turnout::Turnout()
{
   _number = 0;
   _state  = 0;
   _lock   = 0;
}

Turnout::~Turnout()
{
   //dtor
}

void Turnout::setState(int state) {
	if (state > 0) {
		_state = 1;   // closed
	} else {
		_state = -1;  // thrown
	}
}

int Turnout::setNumber(int number) {
	if (_number) {
		return TO_NPSET;
	}
	_number = number;
	return 0;
}

int Turnout::lock() {
    if (_lock) { return TRCK_ALRDY; }
    _lock = 1;
    return 0;
}

int Turnout::unlock() {
    if (!_lock) { return TRCK_ALRDY; }
    _lock = 0;
    return 0;
}

int Turnout::isLocked() {
    return _lock;
}