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

#ifndef __VBUFFER_H__
#define __VBUFFER_H__

#include <string.h>
#include <iostream>
#include <atomic>
#include "stdns.h"

#define EOS '\0'
#define MAX_BUF 1024

class VBuffer
{
  private:
   int   _size;
   char* _buf;

  public:
   VBuffer();
   ~VBuffer();
   void  add       (char c);
   void  add       (char* c);
   void  clear     ();
   int   clear_thru(char c);
   int   clear_upto(char c);
   char* get_buf   ();
   char* get_frto  (char c1, char* c2, int size);
   char* get_to    (char* c, int size);
   int   size      () {return _size;}
   void  print     ();
};

#endif
