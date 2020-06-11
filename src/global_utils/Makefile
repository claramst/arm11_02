CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: clean

bitmanipulation.o: bitmanipulation.c bitmanipulation.h
	$(CC) $(CFLAGS) bitmanipulation.c -c -o bitmanipulation.o

clean:
	rm -f $(wildcard *.o)

