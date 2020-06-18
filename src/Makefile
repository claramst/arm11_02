CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic 

.SUFFIXES: .c .o

.PHONY: all clean

all:  emulate assemble ../extension/armour

../extension/armour: ../extension/armour.o ../extension/commands.o ../extension/commandsinfo.o global_utils/arm.o emulator_utils/execute.o emulator_utils/decode.o emulator_utils/instruction.o emulator_utils/state.o global_utils/bitmanipulation.o

../extension/armour.o: ../extension/armour.c ../extension/commands.h ../extension/commandsinfo.h global_utils/arm.h

../extension/commands.o: ../extension/commands.c ../extension/commands.h

../extension/commandsinfo.o: ../extension/commandsinfo.c ../extension/commandsinfo.h

assemble: assemble.o emulator_utils/state.o global_utils/bitmanipulation.o assembler_utils/encode.o emulator_utils/instruction.o assembler_utils/hashmap.o assembler_utils/tokenizer.o assembler_utils/sdtconstants.o

assemble.o: assemble.c assembler_utils/encode.h

assembler_utils/sdtconstants.o: assembler_utils/sdtconstants.c assembler_utils/sdtconstants.h

assembler_utils/encode.o: assembler_utils/encode.c assembler_utils/encode.h

assembler_utils/hashmap.o: assembler_utils/hashmap.c assembler_utils/hashmap.h

assembler_utils/tokenizer.o: assembler_utils/tokenizer.c assembler_utils/tokenizer.h

emulate: emulate.o emulator_utils/decode.o emulator_utils/execute.o emulator_utils/instruction.o emulator_utils/state.o global_utils/bitmanipulation.o global_utils/arm.o

emulate.o: emulate.c emulator_utils/execute.h global_utils/arm.h

global_utils/arm.o: global_utils/arm.c global_utils/arm.h

global_utils/bitmanipulation.o: global_utils/bitmanipulation.c global_utils/bitmanipulation.h

emulator_utils/decode.o: emulator_utils/decode.c emulator_utils/decode.h emulator_utils/state.h

emulator_utils/execute.o: emulator_utils/execute.c emulator_utils/execute.h

emulator_utils/instruction.o: emulator_utils/instruction.c emulator_utils/instruction.h

emulator_utils/state.o: emulator_utils/state.c emulator_utils/state.h


clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	rm -f emulator_utils/*.o assembler_utils/*.o global_utils/*.o
	rm -f ../extension/*.o
	rm -f ../extension/armour
	rm -f ../extension/text.s ../extension/temp.bin
