#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "emulator_utils/instruction.h"
#include "emulator_utils/state.h"
#include "emulator_utils/decode.h"
#include "emulator_utils/execute.h"
#include "binary_utils/bitmanipulation.h"

//TODO
//1. Try and remove global variables
//2. check that our callocs and file reading are all successful
//3. put this all on git...... 
//4. compile this monster


void printState(MACHINE_STATE state) {
  printf("Registers:\n");
  //print the first 13 registers
  for (int i = 0; i < 13; i++) {
    printf("$%-3d:%11d (%#010x)\n", i, state.registers[i], state.registers[i]);
  }
  printf("PC  :%11d (%#010x)\n", state.registers[15], state.registers[15]);
  printf("CPSR:%11d (%#010x)\n", state.registers[16], state.registers[16]);
  printf("Non-zero memory:\n");
  for (int i = 0; i < MAX_ADDRESSES; i++) {
    WORD word = getWord(i, state);
    if (word) {
      printf("%#010x: %#010x\n", i, word);
    }
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "No filename given");
    return EXIT_FAILURE;
  }
  FILE *objCode = fopen(argv[1], "rb");
  
  if(objCode== NULL) {
    fprintf(stderr, "File could not be opened");
    return EXIT_FAILURE;
  }
  
  MACHINE_STATE state;
  
  state.registers = calloc(NUM_OF_REG, sizeof(REGISTER));
  if (!state.registers) {
    printf("Memory Failure \n");
    exit(EXIT_FAILURE);
  }
  state.memory = calloc(MAX_ADDRESSES, sizeof(BYTE));
  if (!state.memory) {
    printf("Memory Failure \n");
    exit(EXIT_FAILURE);
  }

  REGISTER* pc = &state.registers[15];
  
  while(!feof(objCode)) {
   fread(state.memory, sizeof(BYTE), 1, objCode);
  }
  if (ferror(objCode)) {
    printf("An error has occurred whilst file reading");
    exit(EXIT_FAILURE);
  }
  fclose(objCode);
   

  INSTRUCTION fetched = fetch(pc, state);
  INSTRUCTION toDecode = fetched;
  DECODED_INSTR toExecute = decode(toDecode, state); 

  while (toExecute.type != HALT) {
    //fetch
    fetched = fetch(pc, state);
    toDecode = fetched;

    //decode
    toExecute = decode(toDecode, state);
    //execute
    execute(toExecute, state);
  }
  // Reading instruction into pc; fetch step
  // findType(pc);
  

  //// lots of stuff
  printState(state);
  free(state.registers);
  free(state.memory);
  return EXIT_SUCCESS;
}

