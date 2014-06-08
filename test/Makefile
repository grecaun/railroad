#   This file is part of RailRoad.
#
#   RailRoad is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, version 3 of the License.
#
#   RailRoad is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with RailRoad.  If not, see <http://www.gnu.org/licenses/>.

# Test code directory.
GTEST_DIR = ../gtest

# Where to find user code.
USER_DIR = ../src
TEST_DIR = .

# Choose your compiler
CC = g++

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -ansi -Wextra -pthread -std=c++0x
LFLAGS=-lm -lpthread

# All tests produced by this Makefile.
TESTS = MailCar_Test VBuffer_Test Receiver_Test

# Test script file to run all tests.
TESTSCRIPT = testall.sh

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all: $(TESTSCRIPT) $(TESTS)

clean:
	@echo Cleaning up the directory.
	@rm -f $(TESTS) gtest.a gtest_main.a *.o $(TESTSCRIPT)

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

gtest.a: gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a: gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds a sample test.  A test should link with either gtest.a or
# gtest_main.a, depending on whether it defines its own main()
# function.

Coupling.o: $(USER_DIR)/Coupling.cpp $(USER_DIR)/Coupling.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/Coupling.cpp

MailCar.o: $(USER_DIR)/MailCar.cpp $(USER_DIR)/MailCar.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/MailCar.cpp

MailCar_Test.o: $(TEST_DIR)/MailCar_Test.cpp $(USER_DIR)/MailCar.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/MailCar_Test.cpp

MailCar_Test: MailCar.o MailCar_Test.o Coupling.o gtest_main.a
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ $(LFLAGS)
	@echo ./$@ >> $(TESTSCRIPT)

VBuffer.o: $(USER_DIR)/VBuffer.cpp $(USER_DIR)/VBuffer.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/VBuffer.cpp

VBuffer_Test.o: $(TEST_DIR)/VBuffer_Test.cpp $(USER_DIR)/VBuffer.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/VBuffer_Test.cpp

VBuffer_Test: VBuffer.o VBuffer_Test.o gtest_main.a
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ $(LFLAGS)
	@echo ./$@ >> $(TESTSCRIPT)

SerialPort.o: $(USER_DIR)/SerialPort.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/SerialPort.cpp

Receiver.o: $(USER_DIR)/Receiver.cpp $(USER_DIR)/Receiver.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/Receiver.cpp

Receiver_Test.o: $(TEST_DIR)/Receiver_Test.cpp $(USER_DIR)/Receiver.h $(TEST_DIR)/Test_SerialPort.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/Receiver_Test.cpp

Receiver_Test: MailCar.o Coupling.o Receiver.o SerialPort.o Receiver_Test.o gtest_main.a
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ $(LFLAGS)
	@echo ./$@ >> $(TESTSCRIPT)

testall.sh:
	@echo Creating script to run all tests.
	@echo \#! /usr/bin/env bash >$@
	@chmod u+x $@

add_new: z set_props

z:
	@svn add *.cpp
	@svn add Makefile

set_props:
	@svn propset svn:keywords "Date Author Revision" *.cpp
	@svn propset svn:keywords "Date Author Revision" Makefile