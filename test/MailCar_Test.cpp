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

#include <string>
#include <thread>
#include <unistd.h>
#include "gtest/gtest.h"
#include "../src/MailCar.h"

class TestTransmitter {
  public:
    void run() {
    	usleep(100000);
    	myCar->sendMail((char *)"First string to test.");
      	std::string newStr("Second string to test.");
      	myCar->sendMail(newStr);
      	return;
    }
    static void start_thread(void* t){
      	((TestTransmitter*) t)->run();
    }
    TestTransmitter(MailCar *car){
      	myCar = car;
    }
  private:
    MailCar *myCar;
};

class MailCarTestNS:public testing::Test {
	protected:
		virtual void SetUp() {
			stdStr1 = "Standard string object number 1.";
			stdStr2 = "Standard string object number 2.";
			stdStr3 = "Standard string object number 3.";
			charStr1 = "Character string object number 1.";
			charStr2 = "Character string object number 2.";
			charStr3 = "Character string object number 3.";
		}
		MailCar 		testCar;
		int 			sendStat1, checkStat1,
						sendStat2, checkStat2,
						sendStat3, checkStat3,
						sendStat4, checkStat4,
						sendStat5, checkStat5,
						sendStat6, checkStat6;
		std::string stdStr1;
		std::string stdStr2;
		std::string stdStr3;
		const char*	charStr1;
		const char*	charStr2;
		const char*	charStr3;
		std::string outStr1, outStr2, outStr3, outStr4, outStr5, outStr6;
};

class MailCarTestWS:public testing::Test {
	protected:
		virtual void SetUp() {
			charStr1 = "Character string object number 1.";
			charStr2 = "Character string object number 2.";
			charStr3 = "Character string object number 3.";
		}
		MailCar* 		testCar;
		const char*	charStr1;
		const char*	charStr2;
		const char*	charStr3;
		std::string outStr1, outStr2, outStr3;
};

TEST_F(MailCarTestNS, stringSend) {
	sendStat1  = testCar.sendMail(stdStr1);
	checkStat1 = testCar.checkMail(&outStr1);

	EXPECT_STREQ(stdStr1.c_str(), outStr1.c_str());
	EXPECT_EQ(0, sendStat1);
	EXPECT_EQ(0, checkStat1);
}

TEST_F(MailCarTestNS, charStarSend) {
	sendStat1  = testCar.sendMail(charStr1);
	checkStat1 = testCar.checkMail(&outStr1);

	EXPECT_STREQ(charStr1, outStr1.c_str());
	EXPECT_EQ(0, sendStat1);
	EXPECT_EQ(0, checkStat1);
}

TEST_F(MailCarTestNS, stringPrioritySend) {
	sendStat1  = testCar.sendMail(0, stdStr2);
	sendStat2  = testCar.sendMail(1, stdStr1);
	checkStat1 = testCar.checkMail(&outStr1);
	checkStat2 = testCar.checkMail(&outStr2);

	EXPECT_STREQ(stdStr1.c_str(), outStr1.c_str());
	EXPECT_STREQ(stdStr2.c_str(), outStr2.c_str());
	EXPECT_EQ(0, sendStat1);
	EXPECT_EQ(0, sendStat2);
	EXPECT_EQ(0, checkStat1);
	EXPECT_EQ(0, checkStat2);
}

TEST_F(MailCarTestNS, charStarPrioritySend) {
	sendStat1  = testCar.sendMail(0, charStr2);
	sendStat2  = testCar.sendMail(1, charStr1);
	checkStat1 = testCar.checkMail(&outStr1);
	checkStat2 = testCar.checkMail(&outStr2);

	EXPECT_STREQ(charStr1, outStr1.c_str());
	EXPECT_STREQ(charStr2, outStr2.c_str());
	EXPECT_EQ(0, sendStat1);
	EXPECT_EQ(0, sendStat2);
	EXPECT_EQ(0, checkStat1);
	EXPECT_EQ(0, checkStat2);
}

TEST_F(MailCarTestNS, PriorityMixedSend) {
	sendStat1 = testCar.sendMail(0, stdStr2);
	sendStat2 = testCar.sendMail(0, charStr2);
	sendStat3 = testCar.sendMail(stdStr3);
	sendStat4 = testCar.sendMail(charStr3);
	sendStat5 = testCar.sendMail(1, stdStr1);
	sendStat6 = testCar.sendMail(1, charStr1);
	checkStat1 = testCar.checkMail(&outStr1);
	checkStat2 = testCar.checkMail(&outStr2);
	checkStat3 = testCar.checkMail(&outStr3);
	checkStat4 = testCar.checkMail(&outStr4);
	checkStat5 = testCar.checkMail(&outStr5);
	checkStat6 = testCar.checkMail(&outStr6);

	EXPECT_STREQ(stdStr1.c_str(), 	outStr1.c_str());
	EXPECT_STREQ(charStr1,			outStr2.c_str());
	EXPECT_STREQ(stdStr2.c_str(),	outStr3.c_str());
	EXPECT_STREQ(charStr2,			outStr4.c_str());
	EXPECT_STREQ(stdStr3.c_str(),	outStr5.c_str());
	EXPECT_STREQ(charStr3,			outStr6.c_str());
	EXPECT_EQ(0, sendStat1);
	EXPECT_EQ(0, sendStat2);
	EXPECT_EQ(0, sendStat3);
	EXPECT_EQ(0, sendStat4);
	EXPECT_EQ(0, sendStat5);
	EXPECT_EQ(0, sendStat6);
	EXPECT_EQ(0, checkStat1);
	EXPECT_EQ(0, checkStat2);
	EXPECT_EQ(0, checkStat3);
	EXPECT_EQ(0, checkStat4);
	EXPECT_EQ(0, checkStat5);
	EXPECT_EQ(0, checkStat6);
}

TEST_F(MailCarTestNS, checkMail) {
	checkStat1  = testCar.checkMail(&outStr1);
	(void) testCar.sendMail(stdStr1);
	checkStat2  = testCar.checkMail(NULL);
	checkStat3 	= testCar.checkMail(&outStr1);

	EXPECT_EQ(10, checkStat1);
	EXPECT_EQ(11, checkStat2);
	EXPECT_EQ(0, checkStat3);
}

TEST_F(MailCarTestNS, waitForMail) {
	TestTransmitter *tx = new TestTransmitter(&testCar);
	std::thread 	tx_thread(TestTransmitter::start_thread, tx);

	checkStat2 	= testCar.waitForMail(&outStr1);
	checkStat1  = testCar.waitForMail(NULL);
	checkStat3 	= testCar.waitForMail(&outStr1);

	tx_thread.join();

	EXPECT_EQ(11, checkStat1);
	EXPECT_EQ(0, checkStat2);
	EXPECT_EQ(0, checkStat3);
	delete tx;
}

TEST_F(MailCarTestWS, pipe) {
	int myPipe[2];
	EXPECT_EQ(0, pipe(myPipe));
	EXPECT_EQ(strlen(charStr1), write(myPipe[1],charStr1,strlen(charStr1)));

	char buffer[1024];

	int numread = read(myPipe[0], buffer, 1024);

	buffer[numread] = 0;

	EXPECT_STREQ(buffer, charStr1);
}

TEST_F(MailCarTestWS, socketSend) {
	int myPipe[2];
	EXPECT_EQ(0, pipe(myPipe));

	testCar = new MailCar(myPipe[0]);

	EXPECT_EQ(strlen(charStr1), write(myPipe[1],charStr1,strlen(charStr1)));
	usleep(100000);			// Writing too quick causes all to be read in as a single entity.
	EXPECT_EQ(strlen(charStr2), write(myPipe[1],charStr2,strlen(charStr2)));
	usleep(100000);
	EXPECT_EQ(strlen(charStr3), write(myPipe[1],charStr3,strlen(charStr3)));

	(void) testCar->checkMail(&outStr1);
	(void) testCar->checkMail(&outStr2);
	(void) testCar->waitForMail(&outStr3);

	EXPECT_STREQ(charStr1, outStr1.c_str());
	EXPECT_STREQ(charStr2, outStr2.c_str());
	EXPECT_STREQ(charStr3, outStr3.c_str());

	delete testCar;
}