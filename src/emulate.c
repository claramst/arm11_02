#include <stdio.h>
#include <stdlib.h>
#include "emulator_utils/execute.h"
#include <assert.h>

/**
 * emulate.c contains a main function which handles file reading,
 * machine state initialisation and our pipeline.
 */

/**
 * main ensures the given file can be read and
 * opened. We then allocate space in the heap for our machine state,
 * initialising the contents of our registers and memory to be 0.
 * We read the contents of the file into memory.
 * We then have a while loop handling the pipelining of our fetch,
 * decode and execute instruction cycle.
 * Finally, we free memory.
 *
 * @param argc Count of the arguments supplied
 * @param argv Array of strings passed as arguments
 * @return Integer representing exit success
 */
int main(int argc, char **argv) {
  CHECK_PRED(argc != 2, "Invalid number of arguments.");

  FILE *objCode = fopen(argv[1], "rb");
  CHECK_PRED(!objCode, "File could not be opened.");

  MACHINE_STATE state;

  state.registers = calloc(NUM_OF_REG, sizeof(REGISTER));
  CHECK_PRED(!state.registers, "Error allocating register memory.");

  state.memory = calloc(MAX_ADDRESSES, sizeof(BYTE));
  CHECK_PRED(!state.memory, "Error allocating memory for ARM machine.");

  for (int i = 0; fread(&state.memory[i], sizeof(BYTE), 1, objCode) == 1; i++) {}
  CHECK_PRED(ferror(objCode), "An error has occurred whilst file reading.");
  fclose(objCode);

  REGISTER *pc = &state.registers[15];
  assert(pc);

  INSTRUCTION fetched;
  DECODED_INSTR decoded;
  int toDecode = 0;
  int toExecute = 0;
  while (1) {
	if (toExecute) {
	  if (decoded.type == HALT) {
		break;
	  }
	  execute(decoded, state, &toDecode, &toExecute);
	}
	if (toDecode) {
	  decoded = decode(fetched, state);
	  toExecute = 1;
	}
	fetched = fetch(pc, state);
	if (*pc >= 4) {
	  toDecode = 1;
	}
  }
  printState(state);

  free(state.registers);
  free(state.memory);
  return EXIT_SUCCESS;
}

