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


void executeProcessing(REGISTER *dest, REGISTER op1, OPCODE opcode, int op2, MACHINE_STATE state) {
  int result;
  switch (opcode) {
    case AND:
      *dest = result = op1 && op2;
      break;
    case EOR:
      *dest = result = op1 ^ op2;
      break;
    case SUB:
      *dest = result = op1 - op2;
      break;
    case RSB:
      *dest = result = op2 - op1;
      break;
    case ADD:
      *dest = result = op1 + op2;
      break;
    case TST:
      result = op1 && op2;
      break;
    case TEQ:
      result = op1 ^ op2
      break;
    case CMP:
      result = op1 - op2;
      break;
    case ORR:
      *dest = result = op1 | op2;
      break;
    case MOV:
      *dest = result = op2;
      break;
  }

  if (setFlags) {
    REGISTER* cpsr = &state.registers[16];
    // For C bit we need the 31 bit carry out from the shift operations
    switch (decoded.opcode) {
      case AND:
      case EOR:
      case ORR:
      case TEQ:
      case TST:
      case MOV:
        if (decoded.shiftCarryOut) {
          *cpsr = setBit(*cpsr, 29);
        } else {
          *cpsr = clearBit(*cpsr, 29);
        }
        break;
      // NOT SURE ABOUT CASE WHEN RN = 0
      // Think its fine though  
      case ADD:
        //check if addition produced overflow
        if ((op1 > 0) && (op2 > INT_MAX - op1)) {
          *cpsr = setBit(*cpsr, 29);
        } else {
          *cpsr = clearBit(*cpsr, 29);
        }
        break;
      case SUB:
      case CMP:
      //check if subtraction produces carry
       if (op1 > 0) && (op2 < INT_MIN + op1)) {
         *cpsr = clearBit(*cpsr, 29);
       } else {
         *cpsr = setBit(*cpsr, 29);
       }
        break;
      case RSB:
      // other way around check
       if (op2 > 0) && (op1 < INT_MIN + op2)) {
        *cpsr = clearBit(*cpsr, 29);
       } else {
        *cpsr = setBit(*cpsr, 29);
       }
    }

    // For Z flag
    
    if (!result) {
      *cpsr = setBit(*cpsr, 30);
    } else {
      *cpsr = clearBit(*cpsr, 30);
    }

    //for N flag
    if (getBit(result, 31)) {
      *cpsr = setBit(*cpsr, 31);
    } else{
      *cpsr = clearBit(*cpsr, 31);
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
        executeSingleTransfer(rnReg, rdReg, rmReg, decoded.P, decoded.U, decoded.L, decoded.offset, state);
    }
  }
}
