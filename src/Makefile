CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o

emulate.o: emulate.c emulator_utils/instruction.h emulator_utils/decode.h emulator_utils/execute.h binary_utils/bitmanipulation.h
	$(CC) $(CFLAGS) emulate.c -c -o emulate.o
  
binary_utils/bitmanipulation.o: binary_utils/bitmanipulation.c binary_utils/bitmanipulation.h
      $(CC) $(CFLAGS) bitmanipulation.c -c -o bitmanipulation.o

emulator_utils/decode.o: emulator_utils/decode.c emulator_utils/decode.h
      $(CC) $(CFLAGS) decode.c -c -o decode.o

emulator_utils/execute.o: emulator_utils/execute.c emulator_utils/execute.h
      $(CC) $(CFLAGS) execute.c -c -o execute.o

emulator_utils/instruction.o: emulator_utils/instruction.c emulator_utils/instruction.h
      $(CC) $(CFLAGS) instruction.c -c -o instruction.o



clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
