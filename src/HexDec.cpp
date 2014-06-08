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

#include "HexDec.h"

/** CONVERT THE dec INTO A HEX */
byte HexDec::decFFhex(int dec) {
   //byte x = dec;
   return (byte) dec;
}


/** CONVERT THE dec INTO A HEX WITH THE HIGHEST BIT MASKED
    TO ZERO. */
byte HexDec::decFFhex_lo(int dec) {
   //convert dec into 2byte hex.
   unsigned short s = dec;
   unsigned short n = {0x7F};

   // mask the first highest 7 bits.
   return (byte) s & n;
}


/** CONVERT THE dec INTO A HEX SHIFTED RIGHT 7 BITS. */
byte HexDec::decFFhex_hi(int dec) {
   //convert dec into 2byte hex.
   unsigned short s = dec;

   //return the byte shifted 7 bits over.
   return (byte) (s >> 7);
}


/** CREATE THE CHECKSUM GIVEN THE byte* and its size.
    THIS IS DONE BY XOR-ING EACH BYTE AND THEN APPLYING ONES COMPLIMENT. */
byte HexDec::compute_checksum(byte* msg, int size) {
   if(size < 1) return -1;
   byte h;
   int i;

   h = msg[0];
   for(i=0; i<size-1; i++)
      h = h xor msg[i+1];

   return ~h;
}


/** SAME AS compute_checksum EXCEPT THE RETURN VALUE IS NOT JUST THE
    COMPUTED CHECKSUM BUT THE MESSAGE WITH CHECKSUM APPENDED TO BACK. */
byte* HexDec::create_packet(byte* cmd, int size) {
   int i;
   byte* c = new byte[size+1];

   for(i=0; i<size; i++)
      c[i] = cmd[i];

   c[size] = compute_checksum(cmd, size);

   return c;
}
