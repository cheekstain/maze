# Makefile for Bashful Brigade's CS50 Final Project
#
# Emma Hobday, July 2017

PROG = AM_STARTUP
OBJS = startup.o amazing.o mazestruct.o
LIBS = 

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LIBS)

startup.o: startup.c
amazing.o: amazing.h
mazestruct.o: mazestruct/mazestruct.h
avatar_comm.o: avatar_comm/avatar_comm.h

.PHONY: test clean

test: 
	bash solve_maze_test.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	