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
#include "../src/VBuffer.h"

class VBufferTest:public testing::Test {
	protected:
		VBuffer _buf;
		char*    testStr;
};

TEST_F(VBufferTest, addSingle) {
	_buf.add('h');
	testStr = _buf.get_buf();
	EXPECT_STREQ("H", testStr);
	delete testStr;
	_buf.add('e');
	_buf.add('l');
	_buf.add('L');
	_buf.add('o');
	_buf.add(' ');
	_buf.add('W');
	_buf.add('o');
	_buf.add('R');
	_buf.add('l');
	_buf.add('D');
	_buf.add('?');
	testStr = _buf.get_buf();
	EXPECT_STREQ("HELLO WORLD?", testStr);
	delete testStr;
}

TEST_F(VBufferTest, addString) {
	_buf.add((char*)"A Test");
	testStr = _buf.get_buf();
	EXPECT_STREQ("A TEST", testStr);
	delete testStr;
	_buf.add((char*)", and only a test");
	testStr = _buf.get_buf();
	EXPECT_STREQ("A TEST, AND ONLY A TEST", testStr);
	delete testStr;
}

TEST_F(VBufferTest, clearAll) {
	_buf.add((char*)"A test string is very funny?");
	_buf.clear();
	testStr = _buf.get_buf();
	EXPECT_STREQ("", testStr);
	delete testStr;
}

TEST_F(VBufferTest, clearThru) {
	_buf.add((char*)"I'm a test string!");
	_buf.clear_thru('\'');
	testStr = _buf.get_buf();
	EXPECT_STREQ("M A TEST STRING!", testStr);
	delete testStr;
}

TEST_F(VBufferTest, clearUpTo) {
	_buf.add((char*)"This is a test string!");
	_buf.clear_upto('!');
	testStr = _buf.get_buf();
	EXPECT_STREQ("!", testStr);
	delete testStr;
}

TEST_F(VBufferTest, getFromTo) {
	_buf.add((char*)"hellothere[pow|on]");
	testStr = _buf.get_frto('[', (char*)"|]", 2);
	EXPECT_STREQ(NULL, testStr);
	testStr = _buf.get_frto('[', (char*)"|]", 3);
	EXPECT_STREQ("POW", testStr);
	delete testStr;
	testStr = _buf.get_frto('[', (char*)"|]", 5);
	EXPECT_STREQ("POW", testStr);
	delete testStr;
}

TEST_F(VBufferTest, getTo) {
	_buf.add((char*)"pow|on]");
	testStr = _buf.get_to((char*)"|]\0", 2);
	EXPECT_STREQ(NULL, testStr);
	testStr = _buf.get_to((char*)"|]\0", 3);
	EXPECT_STREQ("POW", testStr);
	delete testStr;
	testStr = _buf.get_to((char*)"|]\0", 5);
	EXPECT_STREQ("POW", testStr);
	delete testStr;
}

TEST_F(VBufferTest, size) {
	_buf.add((char*)"123456789");
	EXPECT_EQ(9, _buf.size());
	_buf.clear();
	EXPECT_EQ(0, _buf.size());
	_buf.add('1');
	_buf.add('2');
	_buf.add('3');
	_buf.add('4');
	_buf.add('5');
	_buf.add('6');
	EXPECT_EQ(6, _buf.size());
	EXPECT_EQ(1, _buf.clear_thru('2'));
	EXPECT_EQ(4, _buf.size());
	EXPECT_EQ(1, _buf.clear_upto('4'));
	EXPECT_EQ(3, _buf.size());
}