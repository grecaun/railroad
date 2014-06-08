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

#include "Section.h"
#include <iostream>

Section::Section(int type, int number, nextSect* sects, Turnout* turnouts) {
	_type      = type;
	_number    = number;
	_reserved  = 0;
	_nextSects = new nextSect[10];
    int i;
    for (i=0;i<10;i++) {
        _nextSects[i].sensor  = sects[i].sensor;
        _nextSects[i].sectNum = sects[i].sectNum;
    }
	_turnouts  = turnouts;
}

Section::~Section() {
    if (_nextSects != nullptr) {
        delete [] _nextSects;
    }
	if (_turnouts != nullptr) {
		delete [] _turnouts;
	}
}