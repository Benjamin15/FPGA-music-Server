PROG = server
CC = g++
LDFLAGS = -lboost_system -lcrypto -lssl -lrestbed -lpthread -pthread -L /usr/local/lib
CPPFLAGS = -g -Wall -std=c++11 -L /usr/lib  -L /usr/local/lib
OBJS = Rest.o
PRECOMPILED_HEADER = Rest.h

$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJS) 
Rest.o : Rest.h
	$(CC) $(CPPFLAGS) -c Rest.cpp	
clean:
	rm -f core $(PROG) $(OBJS) $(PRECOMPILED_HEADER)