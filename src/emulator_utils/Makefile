CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: clean

decode.o: decode.c decode.h state.h
	$(CC) $(CFLAGS) decode.c -c -o decode.o

execute.o: execute.c execute.h
	$(CC) $(CFLAGS) execute.c -c -o execute.o

instruction.o: instruction.c instruction.h
	$(CC) $(CFLAGS) instruction.c -c -o instruction.o

state.o: state.c state.h
	$(CC) $(CFLAGS) state.c -c -o state.o

clean:
	rm -f $(wildcard *.o)
