CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o emulator_utils/decode.o emulator_utils/execute.o emulator_utils/instruction.o emulator_utils/state.o binary_utils/bitmanipulation.o
	$(CC) emulate.o emulator_utils/decode.o emulator_utils/execute.o emulator_utils/instruction.o emulator_utils/state.o binary_utils/bitmanipulation.o -o emulate

emulate.o: emulate.c emulator_utils/instruction.h emulator_utils/decode.h emulator_utils/execute.h binary_utils/bitmanipulation.h
	$(CC) $(CFLAGS) emulate.c -c -o emulate.o
  
binary_utils/bitmanipulation.o: binary_utils/bitmanipulation.c binary_utils/bitmanipulation.h
	$(CC) $(CFLAGS) binary_utils/bitmanipulation.c -c -o binary_utils/bitmanipulation.o

emulator_utils/decode.o: emulator_utils/decode.c emulator_utils/decode.h binary_utils/bitmanipulation.h emulator_utils/state.h emulator_utils/instruction.h
	$(CC) $(CFLAGS) emulator_utils/decode.c -c -o emulator_utils/decode.o

emulator_utils/execute.o: emulator_utils/execute.c emulator_utils/execute.h binary_utils/bitmanipulation.h emulator_utils/decode.h emulator_utils/state.h emulator_utils/instruction.h
	$(CC) $(CFLAGS) emulator_utils/execute.c -c -o emulator_utils/execute.o

emulator_utils/instruction.o: emulator_utils/instruction.c emulator_utils/instruction.h emulator_utils/state.h binary_utils/bitmanipulation.h
	$(CC) $(CFLAGS) emulator_utils/instruction.c -c -o emulator_utils/instruction.o

emulator_utils/state.o: emulator_utils/state.c emulator_utils/state.h emulator_utils/instruction.h binary_utils/bitmanipulation.h
	$(CC) $(CFLAGS) emulator_utils/state.c -c -o emulator_utils/state.o


clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
