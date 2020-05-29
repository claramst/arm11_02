#include <stdint.h>
#include "../../binary_utils/bitmanipulation.h"

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

INSTRUCTION fetch(ADDRESS address, MACHINE_STATE state);
INSTR_TYPE findType(INSTRUCTION instr);
int willExecute(CONDITION cond, MACHINE_STATE state);

#endif


