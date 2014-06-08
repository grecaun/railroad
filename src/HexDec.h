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

#ifndef __HEXDEC_H__
#define __HEXDEC_H__

typedef unsigned char byte;

class HexDec
{
  public:
   byte decFFhex(int dec);
   byte decFFhex_lo(int dec);
   byte decFFhex_hi(int dec);
   byte compute_checksum(byte* msg, int size);
   byte* create_packet(byte* cmd, int size);

  private:
   byte* _hexff;
   unsigned short* _hexffff;
};

#endif
