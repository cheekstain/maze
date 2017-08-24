
# Makefile for 'avatar_solve' module
#
# Bashful Brigade: Christina Lu, August 2017

PROG = solve_test
OBJS = avatar_solve.o ../mazestruct/mazestruct.o ../libcs50/memory.o ../libcs50/counters.o solve_test.o 
LIBS = 

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LIBS)


avatar_solve.o: avatar_solve.h ../amazing.h ../mazestruct/mazestruct.h ../maze_pointers.h ../libcs50/counters.h 

mazestruct.o: ../mazestruct/mazestruct.h 

memory.o: ../libcs50/memory.h

counters.o: ../libcs50/memory.h ../libcs50/counters.h

solve_test.o: avatar_solve.h ../amazing.h ../mazestruct/mazestruct.h ../maze_pointers.h 


.PHONY: test clean 

test: $(PROG)
	./$(PROG)


clean: 
	rm -rf $(PROG)
	rm -rf *.o
	#rm ./test_file
