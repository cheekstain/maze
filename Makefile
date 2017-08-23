# Makefile for Bashful Brigade's CS50 Final Project
#
# Emma Hobday, July 2017

PROG = AM_STARTUP
OBJS = startup.o mazestruct/mazestruct.o avatar_comm/avatar_comm.o maze_pointer/maze_pointers.o avatar_solve/avatar_solve.o 
LIBS = -lpthread libcs50/libcs50.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LIBS)

startup.o: startup.c amazing.h
mazestruct/mazestruct.o: mazestruct/mazestruct.h amazing.h 
avatar_comm/avatar_comm.o: avatar_comm/avatar_comm.h amazing.h 
maze_pointers/maze_pointers.o: maze_pointer/maze_pointers.h 
avatar_solve/avatar_solve.o: avatar_solve/avatar_solve.h amazing.h 

.PHONY: test clean

test: 
	bash solve_maze_test.sh

clean:
	rm -f *~ *.o *.dSYM *.log
	rm -f $(PROG)
	$(MAKE) -C avatar_comm clean
	$(MAKE) -C avatar_solve clean
	$(MAKE) -C mazestruct clean
	cd maze_pointer
	rm -f *~ *.o *.dSYM
	cd ..





	