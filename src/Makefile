CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all:  emulate assemble

assemble: assemble.o emulator_utils/state.o global_utils/bitmanipulation.o assembler_utils/encode.o emulator_utils/instruction.o assembler_utils/hashmap.o assembler_utils/tokenizer.o assembler_utils/sdtconstants.o

assemble.o: assemble.c emulator_utils/state.h global_utils/bitmanipulation.h assembler_utils/encode.h assembler_utils/sdtconstants.h

assembler_utils/sdtconstants.o: assembler_utils/sdtconstants.c assembler_utils/sdtconstants.h

assembler_utils/encode.o: assembler_utils/encode.c assembler_utils/encode.h

assembler_utils/hashmap.o: assembler_utils/hashmap.c assembler_utils/hashmap.h

assembler_utils/tokenizer.o: assembler_utils/tokenizer.c assembler_utils/tokenizer.h

emulate: emulate.o emulator_utils/decode.o emulator_utils/execute.o emulator_utils/instruction.o emulator_utils/state.o global_utils/bitmanipulation.o
	$(CC) emulate.o emulator_utils/decode.o emulator_utils/execute.o emulator_utils/instruction.o emulator_utils/state.o global_utils/bitmanipulation.o -o emulate

emulate.o: emulate.c emulator_utils/execute.h
	$(CC) $(CFLAGS) emulate.c -c -o emulate.o

global_utils/bitmanipulation.o: global_utils/bitmanipulation.c global_utils/bitmanipulation.h
	$(CC) $(CFLAGS) global_utils/bitmanipulation.c -c -o global_utils/bitmanipulation.o

emulator_utils/decode.o: emulator_utils/decode.c emulator_utils/decode.h emulator_utils/state.h
	$(CC) $(CFLAGS) emulator_utils/decode.c -c -o emulator_utils/decode.o

emulator_utils/execute.o: emulator_utils/execute.c emulator_utils/execute.h
	$(CC) $(CFLAGS) emulator_utils/execute.c -c -o emulator_utils/execute.o

emulator_utils/instruction.o: emulator_utils/instruction.c emulator_utils/instruction.h
	$(CC) $(CFLAGS) emulator_utils/instruction.c -c -o emulator_utils/instruction.o

emulator_utils/state.o: emulator_utils/state.c emulator_utils/state.h
	$(CC) $(CFLAGS) emulator_utils/state.c -c -o emulator_utils/state.o


clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
