OBJS = avatar_comm.o maze_pointers.o
LIB = libmaze_pointers.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 ../avatar_comm
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(LIB) *~ *.o
