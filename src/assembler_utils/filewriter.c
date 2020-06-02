#include "filewriter.h"
#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "bitmanipulation.h"

// Writes n words (in big endian) to the file with the given name
// n is the number of instructions
void writeToFile(FILE *fileName, WORD word) {
  for (int j = 0; j < 4; j++) {
    fwrite(getByte(words[i], j), sizeof(BYTE), 1, file);
  }
}

//mov r1 r2
//-> convert to binary
//-> write to a file
//-> pass to emulator




