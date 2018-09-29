PROG = server
CC = g++
LDFLAGS = -lboost_system -lcrypto -lssl -lrestbed -lpthread -pthread -L /usr/local/lib
CPPFLAGS = -g -Wall -std=c++11 -L /usr/lib  -L /usr/local/lib
OBJS = Rest.o ManagerUser.o ManagerStatistiques.o ManagerMusic.o
PRECOMPILED_HEADER =

$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJS) 
ManagerMusic.o : manager/ManagerMusic.h
	$(CC) $(CPPFLAGS) -c manager/ManagerMusic.cpp	
ManagerUser.o : manager/ManagerUser.h
	$(CC) $(CPPFLAGS) -c manager/ManagerUser.cpp	
ManagerStatistiques.o : manager/ManagerStatistiques.h
	$(CC) $(CPPFLAGS) -c manager/ManagerStatistiques.cpp		
Rest.o : Rest.h
	$(CC) $(CPPFLAGS) -c Rest.cpp	
clean:
	rm -f core $(PROG) $(OBJS) $(PRECOMPILED_HEADER)