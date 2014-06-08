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


EXE=runtrain

CC=g++
CXXFLAGS=-ggdb -Wall -ansi -pedantic -std=c++0x
LFLAGS=-lm -lpthread
VFLAGS=-c

SRCS:=$(wildcard ./src/*.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(SRCS))
DEPS:=$(patsubst %.cpp,%.d,$(SRCS))
HEAD:=$(patsubst %.cpp,%.h,$(SRCS)) src/constants.h src/opcodes.h src/stdns.h

$(EXE): $(DEPS) $(OBJS)
	$(CC) $(CXXFLAGS) -o $(EXE) $(OBJS) pugixml/src/pugixml.cpp $(LFLAGS)
	chmod +x api/api.py

%.d: %.cpp
	$(CC) $(CXXFLAGS) -M $< > $@

valgrind: $(EXE)
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./$(EXE) $(VFLAGS)

clean:
	@echo Removing object, dependency, and other files.
	@rm -f $(OBJS) $(DEPS) $(EXE) *~ api/*.pyc data/Train.db

$(OBJS): $(HEAD)