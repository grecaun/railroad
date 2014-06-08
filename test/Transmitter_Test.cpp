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

#include "gtest/gtest.h"
#include "../src/Transmitter.h"
#include "Test_SerialPort.h"

class TransmitterTest:public testing::Test {
	protected:
		// Items required for Receiver.
		Test_SerialPort   _usb;
		// Strings for getting information.
		byte* sentData1;
		byte* sentData2;
		byte* sentData3;
		byte* sentData4;
		byte* sentData5;
		// FD for reading from.
		int _readFD;
		virtual void SetUp() {
			_readFD         = _usb.ConfigureTrans();
			Transmitter _tx = new Transmitter(&_usb, 0);
		}
};

TEST_F(TransmitterTest, sendPower) {
	_tx.send_power("ON");
	_tx.send_power("OFF");
}

TEST_F(TransmitterTest, sendRequest) {
	_tx.send_request("420");
	_tx.send_request("1204");
	_tx.send_request("411");

}

TEST_F(TransmitterTest, sendStop) {
	_tx.send_stop("1");
	_tx.send_stop("2");
	_tx.send_stop("3");
}

TEST_F(TransmitterTest, sendMove) {
	_tx.send_move("1", "2");
	_tx.send_move("2", "3");
}

TEST_F(TransmitterTest, sendSpeed) {
	_tx.send_speed("1","0");
	_tx.send_speed("2", "35");
	_tx.send_speed("3", "1");
	_tx.send_speed("3", "126");
}

TEST_F(TransmitterTest, sendDirection) {
	_tx.send_direction("1", "F");
	_tx.send_direction("2", "R");
	_tx.send_direction("3", "F");

}

TEST_F(TransmitterTest, sendSound) {
	_tx.send_sound("1", "OFF");
	_tx.send_sound("2", "ON");
	_tx.send_sound("3", "ON");
}

TEST_F(TransmitterTest, sendTurnout) {
	_tx.send_turnout("4", "T");
	_tx.send_turnout("7", "C");
	_tx.send_turnout("13", "T");
	_tx.send_turnout("22", "C");
}