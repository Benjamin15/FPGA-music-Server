INCLUDE = -Iinclude/

LIBRARIES = -Llib/ -lrestbed -lssl -lcrypto
CXX=g++  -Wno-unused-result -std=c++11 -fuse-ld=bfd
RM=rm -f
CPPFLAGS=-c -Wall
LDFLAGS=-g $(INCLUDE)
LDLIBS=$(LIBRARIES)

SOURCES_PLAYER=test.cpp
OBJECTS_PLAYER=$(SOURCES_PLAYER:.cpp=.o)
EXECUTABLE_PLAYER=test


all: $(SOURCES_PLAYER) $(EXECUTABLE_PLAYER)

$(EXECUTABLE_PLAYER): $(OBJECTS_PLAYER)
	$(CXX) $(LDFLAGS) $(OBJECTS_PLAYER) -o $@ $(LDLIBS)

.cpp.o:
	$(CXX) $(LDFLAGS) $(CPPFLAGS) $< -o $@ $(LDLIBS)

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
