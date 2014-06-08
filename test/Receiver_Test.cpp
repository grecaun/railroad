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

#include <atomic>
#include "gtest/gtest.h"
#include "../src/MailCar.h"
#include "../src/Receiver.h"
#include "Test_SerialPort.h"

class ReceiverTest:public testing::Test {
	protected:
		// Items required for Receiver.
		Test_SerialPort   _usb;
		MailCar           _car;
		std::atomic<bool> _exit;
		// Strings for getting information.
		string testStr1;
		string testStr2;
		string testStr3;
		string testStr4;
		string testStr5;
		// Holder for thread pointer.
		std::thread *rx_thrd;
		Receiver    *rx;
		// FD for writing to.
		int _writeFD;
		virtual void SetUp() {
			_exit = false;
			_writeFD = _usb.ConfigureRecv();
			rx       = new Receiver(&_usb, &_car, &_exit, 0);
			rx_thrd  = new std::thread(Receiver::start_thread, rx);
		}
};

TEST_F(ReceiverTest, sensor) {
	byte sensor01[4] = {0xB2, 0x00, 0x50, 0x1D};
	byte sensor25[4] = {0xB2, 0x0C, 0x50, 0x11};
	byte sensor31[4] = {0xB2, 0x0F, 0x50, 0x12};
	byte sensor68[4] = {0xB2, 0x21, 0x70, 0x1C};
	byte sensor81[4] = {0xB2, 0x28, 0x50, 0x35};

	write(_writeFD, sensor01, 4);
	_car.waitForMail(&testStr1);

	write(_writeFD, sensor25, 4);
	_car.waitForMail(&testStr2);

	write(_writeFD, sensor31, 4);
	_car.waitForMail(&testStr3);

	write(_writeFD, sensor68, 4);
	_car.waitForMail(&testStr4);

	write(_writeFD, sensor81, 4);
	_car.waitForMail(&testStr5);

	_exit=true;
	rx_thrd->join();

	EXPECT_STREQ("[ISEN|1|HI]",testStr1.c_str());
	EXPECT_STREQ("[ISEN|25|HI]",testStr2.c_str());
	EXPECT_STREQ("[ISEN|31|HI]",testStr3.c_str());
	EXPECT_STREQ("[ISEN|68|HI]",testStr4.c_str());
	EXPECT_STREQ("[ISEN|81|HI]",testStr5.c_str());


	delete rx_thrd;
	delete rx;
}

TEST_F(ReceiverTest, turnout) {
	byte turnout05t[4] = {0xB1, 0x04, 0x60, 0x2A};
	byte turnout06c[4] = {0xB1, 0x05, 0x70, 0x3B};
	byte turnout11t[4] = {0xB1, 0x0A, 0x60, 0x24};
	byte turnout19c[4] = {0xB1, 0x12, 0x70, 0x2C};
	byte turnout22c[4] = {0xB1, 0x15, 0x70, 0x2B};

	write(_writeFD, turnout05t, 4);
	_car.waitForMail(&testStr1);

	write(_writeFD, turnout06c, 4);
	_car.waitForMail(&testStr2);

	write(_writeFD, turnout11t, 4);
	_car.waitForMail(&testStr3);

	write(_writeFD, turnout19c, 4);
	_car.waitForMail(&testStr4);

	write(_writeFD, turnout22c, 4);
	_car.waitForMail(&testStr5);

	_exit=true;
	rx_thrd->join();

	EXPECT_STREQ("[ITOT|5|T]",testStr1.c_str());
	EXPECT_STREQ("[ITOT|6|C]",testStr2.c_str());
	EXPECT_STREQ("[ITOT|11|T]",testStr3.c_str());
	EXPECT_STREQ("[ITOT|19|C]",testStr4.c_str());
	EXPECT_STREQ("[ITOT|22|C]",testStr5.c_str());

	delete rx_thrd;
	delete rx;
}

TEST_F(ReceiverTest, locoSpeed) {
	byte train1[4] = {0xA0, 0x01, 0x1F, 0x00}; // Checksum not valid.
	byte train2[4] = {0xA0, 0x02, 0x01, 0x00}; // CHecksum not valid.
	byte train3[4] = {0xA0, 0x03, 0x00, 0x5C};

	write(_writeFD, train1, 4);
	_car.waitForMail(&testStr1);

	write(_writeFD, train2, 4);
	_car.waitForMail(&testStr2);

	write(_writeFD, train3, 4);
	_car.waitForMail(&testStr3);

	_exit=true;
	rx_thrd->join();

	EXPECT_STREQ("[ISPD|1|31]",testStr1.c_str());
	EXPECT_STREQ("[ISPD|2|0]",testStr2.c_str());
	EXPECT_STREQ("[ISPD|3|0]",testStr3.c_str());

	delete rx_thrd;
	delete rx;
}

TEST_F(ReceiverTest, locoDir) {
	byte train1[4] = {0xA1, 0x01, 0x00, 0x00}; // Checksum not valid.
	byte train2[4] = {0xA1, 0x02, 0x20, 0x00}; // Checksum not valid.
	byte train3[4] = {0xA1, 0x03, 0x00, 0x00}; // Checksum not valid.

	write(_writeFD, train1, 4);
	_car.waitForMail(&testStr1);

	write(_writeFD, train2, 4);
	_car.waitForMail(&testStr2);

	write(_writeFD, train3, 4);
	_car.waitForMail(&testStr3);

	_exit=true;
	rx_thrd->join();

	EXPECT_STREQ("[IDIR|1|F]",testStr1.c_str());
	EXPECT_STREQ("[IDIR|2|R]",testStr2.c_str());
	EXPECT_STREQ("[IDIR|3|F]",testStr3.c_str());

	delete rx_thrd;
	delete rx;
}

TEST_F(ReceiverTest, locoSnd) {
	byte train1[4] = {0xA2, 0x01, 0x00, 0x00}; // Checksum not valid.
	byte train2[4] = {0xA2, 0x02, 0x08, 0x00}; // Checksum not valid.
	byte train3[4] = {0xA2, 0x03, 0x00, 0x00}; // Checksum not valid.

	write(_writeFD, train1, 4);
	_car.waitForMail(&testStr1);

	write(_writeFD, train2, 4);
	_car.waitForMail(&testStr2);

	write(_writeFD, train3, 4);
	_car.waitForMail(&testStr3);

	_exit=true;
	rx_thrd->join();

	EXPECT_STREQ("[ISND|1|ON]",testStr1.c_str());
	EXPECT_STREQ("[ISND|2|OFF]",testStr2.c_str());
	EXPECT_STREQ("[ISND|3|ON]",testStr3.c_str());

	delete rx_thrd;
	delete rx;
}

TEST_F(ReceiverTest, junk) {
	byte msg[4] = {0x00, 0x00, 0x00, 0x00};

	write(_writeFD, msg, 4);
	usleep(100000);
 	int noMail = _car.checkMail(&testStr1);

	_exit=true;
	rx_thrd->join();

	EXPECT_EQ(10, noMail);

	delete rx_thrd;
	delete rx;
}