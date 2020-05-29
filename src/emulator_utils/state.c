#include "instruction.h"
#include "../binary_utils/bitmanipulation.h"
#include "state.h"

// This function returns the word from memory 
// at the given address (in big endian)
WORD getWord(ADDRESS address, MACHINE_STATE state) {
  int32_t sum = 0;
  for (int i = 0; i < 4; i++) {
    sum += state.memory[address + i] << 8 * i;
  }
  return sum;
}

// Stores the given word at the address given in big endian
void storeWord(WORD word, ADDRESS address, MACHINE_STATE state) {
  for (int i = 0; i < 4; i++) {
    state.memory[address + i] = getByte(word, i);
  }
}
