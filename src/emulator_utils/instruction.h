#include <stdint.h>
#include "../../binary_utils/bitmanipulation.h"

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

typedef uint32_t INSTRUCTION;
typedef uint32_t WORD;
typedef uint32_t REGISTER;
typedef uint16_t ADDRESS;

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

WORD getWord(REGISTER *pc, MACHINE_STATE state);
void storeWord(WORD word, ADDRESS address, MACHINE_STATE state);
INSTR_TYPE findType(INSTRUCTION instr);

#endif

