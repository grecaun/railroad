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

#ifndef __TRACK_H__
#define __TRACK_H__

#include <mutex>
#include "Train.h"
#include "Section.h"
#include "SimpleSect.h"
#include "CrossSect.h"
#include "TurnoutSect.h"
#include "TrainYardSect.h"
#include "TurnCrossSect.h"
#include "Transmitter.h"

class Track {
   public:
      Track();
      ~Track();
      void setup();
      int move_train       (int slot, int front, int back);
      int chspd_train      (int slot, int speed);
      int chdir_train      (int slot, int dir);
      int lock_train       (int slot, int state);
      int is_train_locked  (int slot);
      int move_turnout     (int tout, int toc);
      int lock_turnout     (int tout, int state);
      int is_turnout_locked(int tout);

      Train* getAssoc(int sens);
      void   lockEither();

      int getNumSections() {return _numSections;}
      int getNumSensors()  {return _numSensors;}
      int getNumTurnouts() {return _numTurnouts;}
      int getNumTrains()   {return _numTrains;}

   private:
      Section**    _sections;
      Train**      _trains;
      Turnout**    _turnouts;   // Deleting not necessary
      int          (*_sensors)[2];
      Transmitter* _xmitter;    // Deleting not necessary
      std::mutex   _mtx;
      int _numSections;
      int _numSensors;
      int _numTurnouts;
      int _numTrains;
      void clear();
};

#endif // __TRACK_H__
