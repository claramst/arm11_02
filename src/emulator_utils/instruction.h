#include "state.h"
#include <stdbool.h>
/**
 * Header file for instruction.c,
 * defines enums for instruction conditions and instruction types.
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

typedef uint32_t INSTRUCTION;

typedef enum instrType {
  MULTIPLY,
  BRANCH,
  PROCESSING,
  TRANSFER,
  HALT
} INSTR_TYPE;

typedef enum condition {
  EQ = 0,
  NE = 1,
  GE = 10,
  LT = 11,
  GT = 12,
  LE = 13,
  AL = 14,
} CONDITION;

INSTRUCTION fetch(REGISTER *pc, MACHINE_STATE *state);
INSTR_TYPE findType(INSTRUCTION instr);
int willExecute(CONDITION cond, MACHINE_STATE *state);
bool validInstr(char *str);

#endif


