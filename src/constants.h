/**
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

   CONSTANT VALUES FOR THE APPLICATION */

#ifndef __CONSTANTS_H_
#define __CONSTANTS_H__

#define PORTNO 			 54321
#define UX_API 			 "api/api.py"

// Good return values

#define RET_SUCCESS		 0		// Success
#define RET_NOMAIL		 10		// No Mail
#define RET_YGMAIL		 11		// You've got Mail. (I.E. unable to give mail to you)

// Error return values for functions

#define ERR_UNDEF 		-1		// Undefined error
#define ERR_MAILNF		-10		// Mail not found
#define ERR_NOMSG		-11 	// No message provided

// errors related to sockets
#define SOCK_OPS         90
#define SOCK_SSO 		 91
#define SOCK_BND 		 92
#define SOCK_LST 		 93
#define SOCK_FRK 		 94

// errors related to track objects
#define TRCK_NOTSETUP   -20
#define TO_NPSET        -21    // Turnout number previously set
#define TRCK_NOTRN      -22    // no train with said number
#define TRCK_NOTOUT     -23    // no turnout with said number
#define TRCK_ALRDY      -24    // train/turnout already locked/unlocked

#endif