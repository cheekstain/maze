# Makefile for Bashful Brigade's CS50 Final Project
#
# Emma Hobday, July 2017

PROG = AM_STARTUP
OBJS = startup.o mazestruct/mazestruct.o avatar_comm/avatar_comm.o maze_pointers.o avatar_solve/avatar_solve.o 
LIBS = -lpthread -lm libcs50/libcs50.a 
CFLAGS = -Wall -pedantic -std=gnu11 -ggdb
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LIBS)

startup.o: startup.c amazing.h
mazestruct/mazestruct.o: mazestruct/mazestruct.h amazing.h 
avatar_comm/avatar_comm.o: avatar_comm/avatar_comm.h amazing.h 
maze_pointers.o: maze_pointers.h 
avatar_solve/avatar_solve.o: avatar_solve/avatar_solve.h amazing.h 

.PHONY: test clean

test: 
	bash solve_maze_test.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	$(MAKE) -C avatar_comm clean
	$(MAKE) -C avatar_solve clean
	$(MAKE) -C mazestruct clean




	