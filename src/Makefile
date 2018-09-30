INCLUDE = -I../include/
LIBRARIES = -L../lib/ -lrestbed

CXX=g++  -Wno-unused-result -std=c++11 -fuse-ld=bfd
RM=rm -f
CPPFLAGS=-c -Wall
LDFLAGS=-g $(INCLUDE)
LDLIBS=$(LIBRARIES)

SOURCES=Rest.cpp manager/ManagerMusic.cpp manager/ManagerStatistiques.cpp manager/ManagerUser.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=server

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LDLIBS)

.cpp.o:
	$(CXX) $(LDFLAGS) $(CPPFLAGS) $< -o $@ $(LDLIBS)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)

SHELL := /bin/bash

all-tests := $(addsuffix .test, $(basename $(wildcard *.test-in)))

.PHONY : test all %.test

BC := /usr/bin/bc

test : $(all-tests)

%.test : %.test-in %.test-cmp $(BC)
	@$(BC) <$< 2>&1 | diff -q $(word 2, $?) - >/dev/null || (echo "Test $@ failed" && exit 1)

all : test 
	@echo "Success, all tests passed."