# Makefile for 'set' module
#
# Borrows from the makefile for the set module provided by Prof. Xia
#Arun Anand, 2017

PROG = commtest
OBJS = commtest.o avatar_comm.o 
LIBS = -lm

all: commtest.o avatar_comm.o
# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING)
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

commtest.o: amazing.h avatar_comm.h
avatar_comm.o: amazing.h avatar_comm.h
#hashtable.o: ../libcs50/hashtable.h ../libcs50/jhash.h ../libcs50/set/set.h
#webpage.o: ../libcs50/file.h ../libcs50/webpage.h ../libcs50/memory.h
#bag.o: ../libcs50/bag.h
#set.o: ../libcs50/set.h


.PHONY: clean


clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
