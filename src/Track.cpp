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

#include <iostream>
#include "Track.h"
#include "../pugixml/src/pugixml.hpp"
#include "constants.h"

Track::Track() {
   _sections = nullptr;
   _turnouts = nullptr;
   _trains   = nullptr;
   _sensors  = nullptr;
}

Track::~Track() {
   clear();
}

void Track::setup() {
   // Ensure that if we've been setup before we cleanup.
   clear();
   // Create trains. This is hard coded. Could be changed laster.
   _numTrains = 2;
   _trains    = new Train*[_numTrains];
   _trains[0] = new Train(1,411);
   _trains[1] = new Train(3,1204);
   // Parse the XML for building track information.
   pugi::xml_document doc;
   doc.load_file("./data/layout.xml");

	_numSensors  = atoi(doc.child_value("numSensors"));
	_numSections = atoi(doc.child_value("numSections"));
	_numTurnouts = atoi(doc.child_value("numTurnouts"));

	_sensors  = new int[_numSensors][2];
	_turnouts = new Turnout*[_numTurnouts];

	int ix;
	for (ix = 0; ix < _numSensors; ix++) {
		_sensors[ix][0] = 0;
		_sensors[ix][1] = 0;
	}

	/* TODO remove output code. next three lines
	std::cout << "Number of Sensors: "   << _numSensors
	          << " Number of Sections: " << _numSections
	          << " Number of Turnouts: " << _numTurnouts << "\n";
	//*/

	_sections = new Section*[_numSections];
   for (ix=0;ix<_numSections;ix++) {
      _sections[ix] = nullptr;
   }

	for (pugi::xml_node section : doc.children("section")) {
		int type = atoi(section.child_value("type"));
		int number = atoi(section.child_value("number"));
		/* TODO remove output code. next two lines
		std::cout << "Section #: " << number;
		std::cout << " type: " << type << "\n";
		//*/
		//int numNextSects    = 0;
		int numSectTurnouts = 0;
		switch (type) {
			case 1: // 2 next sections
				//numNextSects = 2;
				break;
			case 3: // 3 next sections and 1 turnout 1 possible link
				//numNextSects     = 3;
				numSectTurnouts  = 1;
				break;
			case 0: // 4 next sections
			case 2:
				//numNextSects = 4;
				break;
			case 4: // 5 next sections 3 turnout
				numSectTurnouts = 2;
			case 5: // 5 sections 1 turnouts
				//numNextSects    = 5;
				numSectTurnouts++;
				break;
		}
		nextSect  newNS[10];
		Turnout*  newTOS = nullptr;
		int sectIX;
		for (pugi::xml_node sect : section.children("nextSect")) {
			sectIX = atoi(sect.child_value("id"))-1;
			newNS[sectIX].sensor  = atoi(sect.child_value("sensor"));
			newNS[sectIX].sectNum = atoi(sect.child_value("sectNum"));
			if (_sensors[newNS[sectIX].sensor-1][0]) {
				if (_sensors[newNS[sectIX].sensor-1][1]) {
					std::cout << "Triple section for this sensor.\n";
				}
				_sensors[newNS[sectIX].sensor-1][1] = newNS[sectIX].sectNum;
			} else {
				_sensors[newNS[sectIX].sensor-1][0] = newNS[sectIX].sectNum;
			}
			/* TODO remove output code. next three lines
			std::cout << "Next Section id: " << sectIX
		        << " sensor number: "  << newNS[sectIX].sensor
		        << " section number: " << newNS[sectIX].sectNum << "\n";
		    //*/
		}
		if (numSectTurnouts > 0) {
			newTOS = new Turnout[numSectTurnouts];
			int turnIX;
			for (pugi::xml_node turnout : section.children("turnout")) {
				turnIX = atoi(turnout.child_value("id"))-1;
				newTOS[turnIX].setNumber(atoi(turnout.child_value("num")));
				_turnouts[newTOS[turnIX].getNumber()-1] = &newTOS[turnIX];
				// TODO add linked turnout information
				/* TODO remove output code. next two lines
				std::cout << "Turnout id: " << turnIX
			    	      << " turnout number: "  << newTOS[turnIX].getNumber() << "\n";
				//*/
			}
		}
		Section* aSect;
		int linked = atoi(section.child_value("sectLink"));
		switch (type) {
			case 0:  // Uses default Section, Yard Termination
				aSect = new Section(type, number, newNS, newTOS);
				break;
			case 1:  // Uses SimpleSection
				aSect = new SimpleSect(type, number, newNS, newTOS);
				break;
			case 2:  // Uses CrossSection
				aSect = new CrossSect(type, number, newNS, newTOS);
				break;
			case 3:  // Uses TurnoutSection
				aSect = new TurnoutSect(type, number, newNS, newTOS, (linked > 0)? linked : -1);
				break;
			case 4:  // Uses TrainYardSection
				aSect = new TrainYardSect(type, number, newNS, newTOS);
				break;
			case 5:  // Uses TurnCrossSection
				aSect = new TurnCrossSect(type, number, newNS, newTOS);
				break;
		}
		_sections[number] = aSect;//*/
	}
}

void Track::clear() {
	int ix;
	if (_sections != nullptr) {
		for (ix=0; ix< _numSections; ix++) {
			if (_sections[ix] != nullptr) {
				delete _sections[ix];
			}
		}
		delete [] _sections;
	}
	if (_trains != nullptr) {
		for (ix=0;ix<_numTrains;ix++) {
			if (_trains[ix] != nullptr) {
				delete _trains[ix];
			}
		}
		delete [] _trains;
	}
   if (_turnouts != nullptr) {
      delete [] _turnouts;
   }
	if (_sensors != nullptr) {
		delete [] _sensors;
	}
}

int Track::move_train(int slot, int front, int back) {
	if (_trains == nullptr) {return TRCK_NOTSETUP;}
	// TODO no idea what this is supposed to do
	return 0;
}

int Track::chspd_train(int slot, int speed) {
	if (_trains == nullptr) {return TRCK_NOTSETUP;}
	int ix;
	for (ix=0;ix<_numTrains;ix++) {
		if (_trains[ix]->getSlot() == slot) {
			_mtx.lock();
			_trains[ix]->updSpeed(speed);
			_mtx.unlock();
			return 0;
		}
	}
	return TRCK_NOTRN;
}

int Track::chdir_train(int slot, int dir) {
	if (_trains == nullptr) {return TRCK_NOTSETUP;}
	int ix;
	for (ix=0;ix<_numTrains;ix++) {
		if (_trains[ix]->getSlot() == slot) {
			_mtx.lock();
			_trains[ix]->changeDir(dir);
			_mtx.unlock();
			return 0;
		}
	}
	return TRCK_NOTRN;
}

int Track::lock_train(int slot, int state) {
	if (_trains == nullptr) {return TRCK_NOTSETUP;}
	int ix;
	for (ix=0;ix<_numTrains;ix++) {
		if (_trains[ix]->getSlot() == slot) {
			_mtx.lock();
			if (state) {
				_trains[ix]->lock();
			} else {
				_trains[ix]->unlock();
			}
			_mtx.unlock();
			return 0;
		}
	}
	return TRCK_NOTRN;
}

int Track::is_train_locked(int slot) {
	if (_trains == nullptr) {return TRCK_NOTSETUP;}
	int ix;
	for (ix=0;ix<_numTrains;ix++) {
		if (_trains[ix]->getSlot() == slot) {
			_mtx.lock();
			int retval = _trains[ix]->isLocked();
			_mtx.unlock();
			return retval;
		}
	}
	return TRCK_NOTRN;
}

int Track::move_turnout(int tout, int toc) {
	if (tout < 1 || tout > _numTurnouts) { return TRCK_NOTOUT; }
	if (_turnouts == nullptr) {return TRCK_NOTSETUP;}
	_mtx.lock();
	_turnouts[tout-1]->setState(toc);
	_mtx.unlock();
	return 0;
}

int Track::lock_turnout(int tout, int state) {
	if (tout < 1 || tout > _numTurnouts) { return TRCK_NOTOUT; }
	if (_turnouts == nullptr) {return TRCK_NOTSETUP;}
	_mtx.lock();
	if (state) {
		_turnouts[tout-1]->lock();
	} else {
		_turnouts[tout-1]->unlock();
	}
	_mtx.unlock();
	return 0;
}

int Track::is_turnout_locked(int tout) {
	if (tout < 1 || tout > _numTurnouts) { return TRCK_NOTOUT; }
	if (_turnouts == nullptr) {return TRCK_NOTSETUP;}
	_mtx.lock();
	int retval = _turnouts[tout-1]->isLocked();
	_mtx.unlock();
	return retval;
}

Train* Track::getAssoc(int sens) {
	// TODO fill this out
	if (_trains == nullptr) {return nullptr;}
	int ix;
	for (ix=0;ix<_numTrains;ix++) {
		if (_trains[ix]->isAssoc(sens)) {
			return _trains[ix];
		}
	}
	return nullptr;
}

void Track::lockEither() {
	std::cout << "I was here." << endl;
}