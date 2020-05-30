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
    int condition;
    enum instrType type;
    int I;
    int A;
    int S;
    int P;
    int U;
    int L;
    int rn;
    int rd;
    int rs;
    int rm;
    int offset;
    OPCODE opcode;
    int op2;
    int shiftCarryOut;
} DECODED_INSTR;

DECODED_INSTR decode(INSTRUCTION instr, MACHINE_STATE state);

#endif

