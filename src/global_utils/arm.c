#include "arm.h"
#include <stdlib.h>
#include <string.h>

/**
 * pipelineCycle executes a stage of the pipeline
 *
 * @param state: A MACHINE_STATE structure representing the state of our
 * machine
 * @param fetched: A fetched INSTRUCTION
 * @param decoded: A DECODED_INSTR struct representing a decoded instruction
 * @param toDecode: A pointer to an int representing whether or not an
 * instruction should be decoded
 * @param toExecute: A pointer to an int representing whether or not an instruction
 * should be executed. So if there is a branch being executed, to execute should
 * be 0, since we should not be executing and decoding the subsequent instructions,
 * since the branch means they should be skipped.
 */
int pipelineCycle(MACHINE_STATE *state, INSTRUCTION *fetched, DECODED_INSTR *decoded, int *toDecode, int *toExecute) {
  REGISTER *pc = &state->registers[15];
  if (*toExecute) {
	if (decoded->type == HALT) {
	  return 1;
	}
	execute(*decoded, state, toDecode, toExecute);
  }
  if (*toDecode) {
	*decoded = decode(*fetched, state);
	*toExecute = 1;
  }
  *fetched = fetch(pc, state);
  if (*pc >= 4) {
	*toDecode = 1;
  }
  return 0;
}

/**
 * Clears all register / general memory, setting all to 0.
 * @param state Struct representing the ARM machine state.
 */
void resetState(MACHINE_STATE *state) {
  memset(state->registers, 0, NUM_OF_REG * sizeof(REGISTER));
  memset(state->memory, 0, MAX_ADDRESSES);
}

/**
 * Allocates memory for a struct representing our ARM machine state.
 * @return a pointer to the initialised state.
 */
MACHINE_STATE *initialiseState() {
  MACHINE_STATE *state = malloc(sizeof(MACHINE_STATE));

  state->registers = calloc(NUM_OF_REG, sizeof(REGISTER));
  CHECK_PRED(!state->registers, "Error allocating register memory.");

  state->memory = calloc(MAX_ADDRESSES, sizeof(BYTE));
  CHECK_PRED(!state->memory, "Error allocating memory for ARM machine.");

  return state;
}


