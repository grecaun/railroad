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

#ifndef __OPCODES_H__
#define __OPCODES_H__

#define NUM_USEFULL 6
#define OP_USEFULL {0xD0, 0xB2, 0xB1, 0xA0, 0xA1, 0xA2} // all other messages are considered junk.

#define OP_POW 0xD0 // power on/off signal
#define OP_SENSOR 0xB2 // sensor signal
#define OP_TURNOUT 0xB1 // turnout signal
#define OP_LOCO_SPEED 0xA0 // loco speed
#define OP_LOCO_FUNC_1 0xA1 // loco dir/functions F1-F4
#define OP_LOCO_FUNC_2 0xA2 // loco funcions F5-F8

#define OP_LOCO_REV 0x20
#define OP_LOCO_FWD 0x00
#define OP_SND_OFF 0x08
#define OP_SND_ON 0x00

#endif // __OPCODES_H__
