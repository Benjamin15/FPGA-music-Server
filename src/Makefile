### WARNING: If librestbed.so cannot be found, use the foloowing command: 
###
### export LD_LIBRARY_PATH=/usr/local/lib/restbed/distribution/library
###

ifndef RESTBED_MODULES_PATH
export RESTBED_MODULES_PATH=/usr/local/lib/restbed
endif

INCLUDE = -I"$(RESTBED_MODULES_PATH)/distribution/include/"
LIBRARIES = -L"$(RESTBED_MODULES_PATH)/distribution/library" -lrestbed

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