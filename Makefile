INCLUDE = -Iinclude/
LIBRARIES = -Llib/ -lrestbed -lmpg123 -lao -lpthread

CXX=g++  -Wno-unused-result -std=c++11 -fuse-ld=bfd
RM=rm -f
CPPFLAGS=-c -Wall
LDFLAGS=-g $(INCLUDE)
LDLIBS=$(LIBRARIES)

SOURCES_SERVER=src/model/User.cpp src/model/Music.cpp src/manager/ManagerMusic.cpp src/manager/ManagerStatistiques.cpp src/manager/ManagerUser.cpp src/Rest.cpp src/utils/json.cpp src/manager/ManagerMicroService.cpp
OBJECTS_SERVER=$(SOURCES_SERVER:.cpp=.o)
EXECUTABLE_SERVER=server

SOURCES_PLAYER=src/player/player.cpp
OBJECTS_PLAYER=$(SOURCES_PLAYER:.cpp=.o)
EXECUTABLE_PLAYER=player


all: $(SOURCES_SERVER) $(EXECUTABLE_SERVER) $(SOURCES_PLAYER) $(EXECUTABLE_PLAYER)

$(EXECUTABLE_SERVER): $(OBJECTS_SERVER)
	$(CXX) $(LDFLAGS) $(OBJECTS_SERVER) -o $@ $(LDLIBS)

.cpp.o:
	$(CXX) $(LDFLAGS) $(CPPFLAGS) $< -o $@ $(LDLIBS)
	
$(EXECUTABLE_PLAYER): $(OBJECTS_PLAYER)
	$(CXX) $(LDFLAGS) $(OBJECTS_PLAYER) -o $@ $(LDLIBS)

clean:
	$(RM) $(OBJECTS_SERVER) $(EXECUTABLE_SERVER) $(OBJECTS_PLAYER) $(EXECUTABLE_PLAYER)

SHELL := /bin/bash

all-tests := $(addsuffix .test, $(basename $(wildcard *.test-in)))

.PHONY : test all %.test

BC := /usr/bin/bc

test : $(all-tests)

%.test : %.test-in %.test-cmp $(BC)
	@$(BC) <$< 2>&1 | diff -q $(word 2, $?) - >/dev/null || (echo "Test $@ failed" && exit 1)

all : test 
	@echo "Success, all tests passed."