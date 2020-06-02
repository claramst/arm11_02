#include "instruction.h"
#include <stdint.h>

#ifndef DECODE_H
#define DECODE_H

typedef enum opcode {
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8,
    TEQ = 9,
    CMP = 10,
    ORR = 12,
    MOV = 13
} OPCODE;

typedef struct decodedInstr {
  enum instrType type;
  int condition;
  unsigned int rn;
  unsigned int rd;
  unsigned int shiftCarryOut;
  union {
    struct {
      int I;
      int S;
      OPCODE opcode;
      unsigned int op2;
    };
    struct {
      int A; 
      int S;
      int rs;
      int rm;
    };
    struct {
      int I;
      int P;
      int U;
      int L;
      int offset;
    };
    struct {
      int offset;
    };
  };
} DECODED_INSTR;

DECODED_INSTR decode(INSTRUCTION instr, MACHINE_STATE state);

#endif

