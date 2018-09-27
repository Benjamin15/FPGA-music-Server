PROG = testServer
CC = g++
LDFLAGS = -lboost_system -lcrypto -lssl -lrestbed -lpthread -pthread -L /usr/local/lib
CPPFLAGS = -g -Wall -std=c++11 -L /usr/lib  -L /usr/local/lib
OBJS = testRestbed.o
PRECOMPILED_HEADER =

$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJS) 
testRestbed.o :
	$(CC) $(CPPFLAGS) -c testRestbed.cpp	
clean:
	rm -f core $(PROG) $(OBJS) $(PRECOMPILED_HEADER)