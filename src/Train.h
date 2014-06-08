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

#ifndef __TRAIN_H__
#define __TRAIN_H__

#include "Section.h"

class Train {
    public:
        Train(int slot, int num);
        ~Train();
        int      isLocked ();
        int      lock     ();
        int      unlock   ();
        bool     isAssoc  (int sens);
        Section* getAssoc (int sens);
        int      reserve  (int numahead, int sens);
        int      free     (int numbehind, int sens);
        int      getSpeed ();
        void     updSpeed (int speed);
        int      getDir   ();
        void     changeDir(int dir);

        int      getSlot()   {return _slot;}
        int      getNumber() {return _number;}

    private:
        int      _slot;
        int      _number;
        int      _speed;
        int      _lock;
        int      _direction;
        int      _numReserved;
        Section* _reserved[8];
};

#endif // __TRAIN_H__
