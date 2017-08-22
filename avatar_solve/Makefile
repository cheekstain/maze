
# Makefile for 'avatar_solve' module
#
# Bashful Brigade: Christina Lu, August 2017

PROG = solve_test
OBJS = avatar_solve.o ../amazing.o ../mazestruct/mazestruct.o ../libcs50/counters.o 
LIBS = -lm

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 ../ 
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LIBS)

solve_test.o: solve_test.c

avatar_solve.o: avatar_solve.h ../amazing.h ../mazestruct/mazestruct.h ../maze_pointer/maze_pointers.h ../libcs50/counters.h 

amazing.o: ../amazing.h

mazestruct.o: ../mazestruct/mazestruct.h

counters.o: ../libcs50/counters.h ../libcs50/memory.h

.PHONY: test clean

test: $(PROG)
	./$(PROG)

clean: 
	rm -rf $(PROG)
