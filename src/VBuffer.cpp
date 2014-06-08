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

#include "VBuffer.h"

/**CONSTRUCTOR SETS SIZE OF BUFFER*/
VBuffer::VBuffer() {
   _buf = new char[MAX_BUF+1];
   for(int i=0; i<MAX_BUF+1; i++)
      _buf[i] = EOS;
   _size = 0;
}

/**DESTRUCTOR FREES BUFFER*/
VBuffer::~VBuffer() {
   delete[] _buf;
}

/**ADDS THE CHARACTER TO THE END OF BUFFER*/
void VBuffer::add(char c) {
   if(_size < MAX_BUF) {
      _buf[_size++] = toupper(c);
      _buf[_size+1] = EOS;
   }
}

/**ADDS THE CHAR ARRAY TO THE END OF THE BUFFER*/
void VBuffer::add(char* c) {
   int i, j, sz = strlen(c);
   //add the upercase c to the buffer.
   for(i=_size, j=0; i+1<MAX_BUF && j<sz; i++, j++) {
      _buf[i] = toupper(c[j]);
      _size++;
   }
   _buf[i] = EOS;
}

/**CLEARS THE WHOLE BUFFER*/
void VBuffer::clear() {
   for(int i=0; i<MAX_BUF+1; i++) {
      _buf[i] = EOS;
   }
   _size = 0;
}

/**CLEARS THE FRONT OF THE BUFFER LEADING UP TO AND INCLUDING THE
   FIRST OCCURANCE OF CHAR c. IF NOT FOUND RETURNS 0, OTHERWISE
   RETURNS THE ELEMENT NUMBER OF c IN THE BUFFER*/
int VBuffer::clear_thru(char c) {
   int i;

   // find first occurance of c.
   for(i=0; i<_size; i++) {
      if(_buf[i] == c) {
	      // clear out the begining of the buffer.
         memmove(_buf, _buf+(i+1), MAX_BUF-i);
         _size -= (i+1);
         return i;
      }
   }

   // c not found return 0.
   return 0;
}

/** CLEANS UP TO BUT NOT INCLUDING THE FIRST OCCURANCE OF CHAR 'C'.
    IF CHAR 'C' IS NOT FOUND IN BUFFER THAN THE WHOLE BUFFER WILL BE
    CLEARED. RETURNS THE ELEMENT NUMBER OF c IN THE BUFFER. OTHERWISE 0 */
int VBuffer::clear_upto(char c) {
   int i;

   // find first occurance of c.
   for(i=0; i<_size; i++)
      if(_buf[i] == c)
	 break;

   if(_buf[i] == c) {
      // clear out the begining of the buffer.
      memmove(_buf, _buf+(i), MAX_BUF - (i));
      _size -= i;
      return i;
   }

   // c not found clear buffer and return 0.
   clear();
   return 0;
}

/** Returns the buffer. */
char* VBuffer::get_buf() {
   char *output = new char[MAX_BUF+1];
   int i, j;
   for (i=0, j=0; i<=MAX_BUF; i++, j++) {
      output[i] = _buf[j];
   }
   return output;
}

/**RETURNS THE CHAR ARRAY OF size AFTER THE FIRST OCCURANCE OF c1 AND BEFORE
   THE FIRST OCCURANCE OF c2 AFTER c1. IF NOT FOUND OR CHAR ARRAY BETWEEN
   c1 AND c2 IS LARGER THAN size, RETURNS NULL*/
char* VBuffer::get_frto(char c1, char *c2, int size){
   int   i, j, c2ix;
   int   done = 0;
   char* ret = new char[size+1];

   for(i=0; i<_size; i++) {
      if(_buf[i] == c1) {                     // c1 was found, search for a max of size chars
         c2ix = 0;
         for(++i, j=0; i<_size && j<size+1; i++, j++) {
            c2ix = -1;
            while (c2[++c2ix]) {              // c2 must be null terminated
               if(_buf[i] == c2[c2ix]) {      // c2 char found
                  done = 1;
                  break;
               }
            }
            if (done) { break; }
            ret[j] = _buf[i];
         }
         if(j<=size && _buf[i] == c2[c2ix]) { // check for match found
            ret[j] = EOS;
            return ret;
         }
         break;
      }
   }
   delete[] ret;
   return NULL;
}

/**RETURNS THE CHAR ARRAY UP TO THE FIRST OCCURANCE OF c. IF NOT FOUND RETURNS NULL*/
char* VBuffer::get_to(char *c, int size) {
   int   i, cix;
   char* ret = new char[size+1];
   int   done = 0;

   for(i=0; i<_size && i<size+1; i++) {
      cix = -1;
      while (c[++cix]) {
         if(_buf[i] == c[cix]) {      // go until c found.
            done = 1;
            break;
         }
      }
      if (done) {
         break;
      }
      ret[i] = _buf[i];              //store up to c.
   }
   //return the stored char* if c found
   if(i<=size && _buf[i] == c[cix]) {
      ret[i] = EOS;
      return ret;
   }
   //c not found return null.
   delete[] ret;
   return NULL;
}

/**DEBUG PRINT OF FULL BUFFER.*/
void VBuffer::print() {
   cout<<endl<<"buf: ";
   for(int i=0; i<_size; i++)
      cout<<_buf[i];

   cout<<endl;
}