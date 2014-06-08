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

#ifndef __SECTION_H__
#define __SECTION_H__

#include "Turnout.h"

class nextSect {
   public:
      int sensor;
      int sectNum;
};

class Section
{
   public:
      Section(int type, int number, nextSect* sects, Turnout* turnouts);
      ~Section();
      int isYardTerm() {return 0;}
      int isSimple()   {return 0;}
      int isTurnCross(){return 0;}
      int isTurnout()  {return 0;}
      int isTrainYard(){return 0;}
      int isCross()    {return 0;}

   protected:
      int       _type;
      int       _number;
      int       _reserved;
      nextSect* _nextSects;
      Turnout*  _turnouts;
};

#endif // __SECTION_H__
