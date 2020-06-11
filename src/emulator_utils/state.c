#include "state.h"
#include <stdio.h>
/**
 * state.c defines functions used to manipulate machine
 * state. This includes functions for accessing and storing words
 * in memory, and printing a representation of machine state.
 */

/**
 * Returns the word from memory at the given address in big endian.
 */
WORD getWord(ADDRESS address, MACHINE_STATE state) {
  WORD sum = 0;
  for (int i = 0; i < 4; i++) {
	sum += state.memory[address + i] << (8 * i);
  }
  return sum;
}

/**
 * Stores the given word at the address given in big endian.
 * @param word represents a 32 bit unsigned int that represents a word in memory.
 */
void storeWord(WORD word, ADDRESS address, MACHINE_STATE state) {
  for (int i = 0; i < 4; i++) {
	state.memory[address + i] = getByte(word, i);
  }
}

/**
 * Returns the word stored at the given address in little endian.
 * @param address Address to get word from.
 * @return Little-endian 32-bit word.
 */
WORD getLittleEndian(ADDRESS address, MACHINE_STATE state) {
  WORD sum = 0;
  for (int i = 0; i < 4; i++) {
	sum += state.memory[address + i] << (8 * (3 - i));
  }
  return sum;
}

/**
 * Takes in our machine state and prints the values stored in
 * our registers and in our memory.
 *
 * @param state A structure MACHINE_STATE representing the state of our ARM machine.
 */
void printState(MACHINE_STATE state) {
  printf("Registers:\n");
  /* Prints the first 13 registers */
  for (int i = 0; i < 13; i++) {
	printf("$%-3d: %10d (0x%08x)\n", i, state.registers[i], state.registers[i]);
  }
  printf("PC  : %10d (0x%08x)\n", state.registers[15], state.registers[15]);
  printf("CPSR: %10d (0x%08x)\n", state.registers[16], state.registers[16]);
  printf("Non-zero memory:\n");
  for (int i = 0; i < MAX_ADDRESSES; i += 4) {
	WORD word = getLittleEndian(i, state);
	if (word) {
	  printf("0x%08x: 0x%08x\n", i, word);
	}
  }
}
