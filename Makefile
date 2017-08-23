# Makefile for Bashful Brigade's CS50 Final Project
#
# Emma Hobday, July 2017

PROG = AM_STARTUP
OBJS = startup.o mazestruct/mazestruct.o libcs50/libcs50.a avatar_comm/avatar_comm.o avatar_solve/avatar_solve.o maze_pointer/libmaze_pointers.a
LIBS = -lpthread

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LIBS)

startup.o: startup.c amazing.h
mazestruct.o: amazing.h mazestruct/mazestruct.h
avatar_comm.o: amazing.h avatar_comm/avatar_comm.h

.PHONY: test clean

test: 
	bash solve_maze_test.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	