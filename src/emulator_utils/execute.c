#include "../../binary_utils/bitmanipulation.h"
#include "../state.h"
#include "instruction.h"

void execute(DECODED_INSTR decoded, MACHINE_STATE state) {
  if (willExecute(decoded.cond, state)) {
    switch (decoded.type) {
      case MULTIPLY:
        REGISTER* rdReg = &state.registers[decoded.rd]
        REGISTER rnReg = state.registers[decoded.rn];
        REGISTER rsReg = state.registers[decoded.rs];
        REGISTER rmReg = state.registers[decoded.rm];
        executeMultiply(rdReg, rnReg, rsReg, rmReg, decoded.A, decoded.S, state);
        break;
      case BRANCH:
        int offset = signExtend(decoded.offset << 2); 
        executeBranch(offset, state);
        break;
      case PROCESSING:
        REGISTER* rdReg = &state.registers[decoded.rd]
        REGISTER rnReg = state.registers[decoded.rn];
        executeProcessing(rdReg, rnReg, decode.offset, decode.op2, state);
      case TRANSFER:
        REGISTER* rnReg = &state.registers[decoded.rn];
        REGISTER* rdReg = &state.registers[decoded.rd];
        REGISTER* rmReg = &state.registers[decoded.rm];
        executeSingleTransfer(rnReg, rdReg, rmReg, decoded.P, decoded.U, decoded.offset, state);
    }
  }
}



void executeSingleTransfer(REGISTER* baseReg, REGISTER* targetReg, REGISTER* rmReg, int preBit, int upBit, int offset, MACHINE_STATE state) {
  ADDRESS address = NULL;
  if (decoded.P) {
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
  if (decoded.L) {
    // Load instruction
    if (baseReg == pc) {
      // The spec says we need to ensure in this case
      // that pc holds the address of the current instruction
      // plus 8. 
    }
    *targetReg = getWord(address32bit, state);
  } else {
    // Store instruction
    storeWord(*targetReg, address32bit, state);
  }
}