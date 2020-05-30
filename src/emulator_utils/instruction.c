#include <stdint.h>
#include <stdio.h>
#include "../binary_utils/bitmanipulation.h"
#include "state.h"
#include "instruction.h"

//fetches the instruction from the address currently stored in the PC
INSTRUCTION fetch(REGISTER *pc, MACHINE_STATE state) {
  INSTRUCTION fetched = getWord(*pc, state);
  *pc += 4;
  return fetched;
}


INSTR_TYPE findType(INSTRUCTION instr) {
  // read first 2 bits. 01 - single data transfer
  // 10 - branch.
  //case 00: if 001 - data processing
  // if 000 we need to read bits 4 - 7 as well, i.e top half of 
  // last byte? If bits 4 - 7 are 1001 then multiply instruction
  // if not 1001, it is either data processing or halt
  // get second nibble, shift right. then cases are 0000 (multiply, halt or dataprocessing), 0001 (dataprocessing), 0010 (single data transfer)
  // 0011 (single data transfer), 0100 (nothing), 0101 (branch), 0111 (nothing)
  if (instr == 0) {
    return HALT;
  }

  if (getBit(instr, 27)) {
    return BRANCH;
  } else if (getBit(instr, 26)) {
      return TRANSFER;
  } else if (getBit(instr, 25)) {
      return PROCESSING;
  } else { // 0
      if (getBit(instr, 4) && getBit(instr, 7)) {
//    if (getNibble(instr, 1) == 9) {
      return MULTIPLY;
    } else {
      return PROCESSING; 
    }
  }
  //should be unreachable
  return HALT;
}

int willExecute(CONDITION cond, MACHINE_STATE state) {
  int cpsr = getNibble(state.registers[16], 7);
  int N = getBit(cpsr, 31);
  int Z = getBit(cpsr, 30);
  //C is not used in part 1
  //int C = getBit(cpsr, 29);
  int V = getBit(cpsr, 28);

  switch (cond) {
    case EQ:
      return Z;
    case NE:
      return !Z;
    case GE:
      return N == V;
    case LT:
      return N != V;
    case GT:
      return !Z && N == V;
    case LE:
      return Z || N != V;
    case AL:
      return 1;
    default:
      return 0;
  }
}

