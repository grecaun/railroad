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

#ifndef __TURNOUTSECT_H__
#define __TURNOUTSECT_H__

#include "Section.h"


class TurnoutSect : public Section
{
	public:
		TurnoutSect(int type, int number, nextSect* sects, Turnout* turnouts, int link);
		int isTurnout(){return 1;} //overide
	private:
		int _linked;
};

#endif // __TURNOUTSECT_H__
