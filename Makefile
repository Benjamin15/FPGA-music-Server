INCLUDE = -Iinclude/

LIBRARIES = -Llib/ -lrestbed -lpthread -lmad -lasound -ltag -lssl -lcrypto
CXX=g++  -Wno-unused-result -std=c++11 -fuse-ld=bfd
RM=rm -f
CPPFLAGS=-c -Wall
LDFLAGS=-g $(INCLUDE)
LDLIBS=$(LIBRARIES)

SOURCES_SERVER=src/model/User.cpp src/model/Music.cpp src/manager/ManagerMusic.cpp src/manager/ManagerStatistics.cpp src/manager/ManagerUser.cpp src/Rest.cpp src/utils/json.cpp src/manager/ManagerMicroService.cpp src/dispatcher/DispatcherMusic.cpp src/dispatcher/DispatcherStatistics.cpp src/dispatcher/DispatcherUser.cpp src/exception/BadRequestException.cpp src/exception/ConflictException.cpp src/exception/ForbiddenException.cpp src/exception/InternalException.cpp src/exception/NotAllowedException.cpp src/exception/RequestLargeException.cpp src/exception/UnauthorizedException.cpp src/exception/UnsupportedException.cpp src/utils/base64.cpp src/utils/log.cpp src/utils/responseGenerator.cpp src/utils/file.cpp
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
