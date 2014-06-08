package edu.wwu.railroad;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;

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

/**
 * Class for controlling socket information.
 *
 * @author James Sentinella
 * @version 1.0
 */
public class SockInfo {
	// Port for connecting to API.
	public static final int    PORTNO = 54320;

	// Variables for dealing with the socket.
	public static Socket      socket;
	public static PrintWriter out;

	/**
	 * Method that checks to see if we can still write to the
	 * socket.
	 *
	 * @return True if socket was closed (or there is a problem)
	 *         Otherwise False.
	 */
	public static boolean checkSock() {
		if (out.checkError()) {
			try {
				socket.close();
			} catch (IOException e) {
			}
			socket=null;
			out.close();
			out=null;
			return true;
		}
		return false;
	}
}
