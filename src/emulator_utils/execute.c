#include "../../binary_utils/bitmanipulation.h"
#include "../state.h"
#include "instruction.h"


void executeMultiply(REGISTER *dest, REGISTER rn, REGISTER rs, REGISTER rm,
 int accumulate, int setFlags, MACHINE_STATE state) {
  if (accumulate) {
    *dest = rm * rs + rn;
  } else {
    *dest = rm * rs;
  }
  if (setFlags) {
    //for N flag
    REGISTER *cpsr = &state.registers[16];
    if (getBit(*dest, 31)) {
      *cpsr = setBit(*cpsr, 31);
    } else {
      *cpsr = clearBit(*cpsr, 31);
    }
    //for Z flag
    if (!*rd) {
      *cpsr = setBit(*cpsr, 30);
    } else {
      *cpsr = clearBit(*cpsr, 30);
    }
  }
}


void executeSingleTransfer(REGISTER* baseReg, REGISTER* targetReg, REGISTER* rmReg, int preBit, int upBit, int ldBit, int offset, MACHINE_STATE state) {
  ADDRESS address = NULL;
  if (preBit) {
    // Pre-indexing: the offset is added/subtracted to the
    // base register before transferring the data. Does not
    // change base register value
    if (upBit) {
      // Offset added
      address = *baseReg + offset;
    } else {
      // Offset subtracted
      address = *baseReg - offset; 
    }
  } else {
    // Post-indexing: the offset is added/subtracting after
    // transferring the data. Changes base register value.
    // A post-indexing ldr or str in which rm is the same 
    // register as Rn, is not allowed.
    assert(rmReg != baseReg);
    address = *baseReg;
    if (upBit) {
      *baseReg += offset;
    } else {
      *baseReg -= offset;
    }
  } 
  if (address > MAX_ADDRESSES) {
    // Handles case where address is bigger than 16 bits
    fprintf(stderr, "Address is too large");
    exit();
  }
  address32bit = (ADDRESS) address;
  if (ldBit) {
    // Load instruction
/*    if (baseReg == pc) {
        We may need to check this case to ensure our pipelining works.
    }
*/    
    *targetReg = getWord(address32bit, state);
  } else {
    // Store instruction
    storeWord(*targetReg, address32bit, state);
  }
}

void executeBranch(int offset, MACHINE_STATE state) {
  state.registers[15] += offset;
}