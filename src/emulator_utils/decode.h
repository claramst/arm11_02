#include "instruction.h"
/**
 * A header file for decode.c,
 * contains enum representing the opcodes for Data Processing Instructions.
 */

#ifndef DECODE_H
#define DECODE_H

typedef enum opcode {
  AND = 0, EOR = 1, SUB = 2, RSB = 3, ADD = 4, TST = 8, TEQ = 9, CMP = 10, ORR = 12, MOV = 13,
  LDR = 14, STR = 15,
  BEQ = 16, BNE = 17, BGE = 18, BLT = 19, BGT = 20, BLE = 21, B = 22,
  LSL = 23, ANDEQ = 24,
  MUL = 25, MLA = 26
} OPCODE;

typedef struct decodedInstr {
  enum instrType type;
  int condition;
  unsigned int rn, rd, shiftCarryOut;
  int I, S,  A, P, U, L, offset;
  OPCODE opcode;
  unsigned int op2, rs, rm;
  int
//    union {
//        struct dpi {
//            int I_dpi;
//            int S_dpi;
//            OPCODE opcode;
//            unsigned int op2;
//        };
//        struct {
//            int A;
//            int S_mul;
//            int rs;
//            int rm;
//        };
//        struct {
//            int I_sdt;
//            int P;
//            int U;
//            int L;
//            int offset_sdt;
//        };
//        struct {
//            int offset_bra;
//        };
//    };
} DECODED_INSTR;

DECODED_INSTR decode(INSTRUCTION instr, MACHINE_STATE state);

#endif

