# Makefile for 'mazestruct' module
#
# Emma Hobday, July 2017

PROG = mazestructtest
OBJS = mazestructtest.o mazestruct.o
LIBS = 

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LIBS)

mazestructtest.o: mazestructtest.c
mazestruct.o: mazestruct.h


.PHONY: test clean

test: 
	bash testmaze.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)