#include "../global_utils/bitmanipulation.h"
#include "../global_utils/errorhandling.h"
/**
 * Header file for state.c
 * Declares functions and type
 * definitions for manipulating our machine state.
 *
 * Defines constants for the number of registers and
 * the maximum number of addresses in our emulator
 * implementation.
 *
 * Defines a structure representing our machine state,
 * which holds an array of registers and an array
 * representing memory.
 *
 * Defines commonly used types WORD, REGISTER and ADDRESS
 *
 */

#ifndef MEMORY_H
#define MEMORY_H

#define MAX_ADDRESSES 65536
#define NUM_OF_REG 17

typedef uint32_t WORD;
typedef uint32_t REGISTER;
typedef uint16_t ADDRESS;

typedef struct machineState {
  REGISTER *registers;
  BYTE *memory;
} MACHINE_STATE;

WORD getWord(ADDRESS address, MACHINE_STATE *state);
void storeWord(WORD word, ADDRESS address, MACHINE_STATE *state);
WORD getLittleEndian(ADDRESS address, MACHINE_STATE *state);
void print_state(MACHINE_STATE *state);

#endif
