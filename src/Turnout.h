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

#ifndef __TURNOUT_H__
#define __TURNOUT_H__


class Turnout
{
    public:
        Turnout();
        ~Turnout();
        int  setNumber(int number);
        int  getNumber() {return _number;};
        void setState (int state);
        int  getState () {return _state;};
        int  lock();
        int  unlock();
        int  isLocked();
    protected:
        int _number;
        int _state;
        int _lock;
    private:
};

#endif // __TURNOUT_H__
