#include "execute.h"
#include <limits.h>
#include <stdio.h>
#include <assert.h>
/**
 * execute.c contains specific execute functions for each instruction type, and
 * a general execute function that passes the relevant arguments to
 * the correct execute function.
 */


/**
 * Takes in the required arguments and executes the specified instruction
 * making changes to the machine state.
 * @param *dest is the pointer to the destination register.
 * @param rn, rs, rm are the respective registers.
 * @param accumulate determines whether or not rn should be added to the result.
 * @param setFlags determines whether flags should be set during the execution.
 * @param state is the current machine state.
 */
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
	if (!*dest) {
	  *cpsr = setBit(*cpsr, 30);
	} else {
	  *cpsr = clearBit(*cpsr, 30);
	}
  }
}

/**
 * Takes in the required arguments and executes the specified data processing
 * instruction, making changes to the machine state.
 *
 * @param *dest is the pointer to the destination register.
 * @param op1 is the register containing operand1.
 * @param opcode determines the operation
 * @param setFlags determines whether flags should be set during the execution.
 * @param shiftCarryOut is the carryOut value
 * @param state is the current machine state.
 */
void executeProcessing(REGISTER *dest,
					   REGISTER op1,
					   OPCODE opcode,
					   unsigned int op2,
					   int setFlags,
					   unsigned int shiftCarryOut,
					   MACHINE_STATE state) {
  int result;
  switch (opcode) {
	case AND: *dest = result = op1 & op2;
	  break;
	case EOR: *dest = result = op1 ^ op2;
	  break;
	case SUB: *dest = result = op1 - op2;
	  break;
	case RSB: *dest = result = op2 - op1;
	  break;
	case ADD: *dest = result = op1 + op2;
	  break;
	case TST: result = op1 & op2;
	  break;
	case TEQ: result = op1 ^ op2;
	  break;
	case CMP: result = op1 - op2;
	  break;
	case ORR: *dest = result = op1 | op2;
	  break;
	case MOV: *dest = result = op2;
	  break;
	default:perror("Invalid opcode.");
	  break;
  }

  if (setFlags) {
	REGISTER *cpsr = &state.registers[16];
	// For C flag
	switch (opcode) {
	  case AND:
	  case EOR:
	  case ORR:
	  case TEQ:
	  case TST:
	  case MOV:
		if (shiftCarryOut == 1) {
		  *cpsr = setBit(*cpsr, 29);
		} else {
		  *cpsr = clearBit(*cpsr, 29);
		}
		break;
	  case ADD:
		// Check if addition produced overflow
		if ((op1 + op2) > INT_MAX) {
		  *cpsr = setBit(*cpsr, 29);
		} else {
		  *cpsr = clearBit(*cpsr, 29);
		}
		break;
	  case SUB:
	  case CMP:
		// Check if subtraction produces carry
		if (op1 < op2) {
		  *cpsr = clearBit(*cpsr, 29);
		} else {
		  *cpsr = setBit(*cpsr, 29);
		}
		break;
	  case RSB:
		// Check if subtraction produces carry
		if (op2 < op1) {
		  *cpsr = clearBit(*cpsr, 29);
		} else {
		  *cpsr = setBit(*cpsr, 29);
		}
	  default: perror("Invalid opcode.");
		break;
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
	} else {
	  *cpsr = clearBit(*cpsr, 31);
	}
  }
}

/**
 * Takes in relevant arguments and executes single data transfer instructions i.e
 * load/store instructions. Changes machine state.
 *
 * @param baseReg Pointer to our base register, to which an offset is
 * added or subtracted in order to calculate the memory address.
 * @param targetReg Pointer to the src/dest register. In the case of
 * load, targetReg is the destination register, to which a value from
 * memory is loaded. In the case of store, targetReg is the source register,
 * from which a value is loaded and then stored at an address in memory.
 * @param rmReg Pointer to the rm register, used in decode to calculate
 * the offset. We take it as an argument so that we can ensure rm is not the same
 * as base register if we are post-indexing.
 * @param preBit If set means we are using pre-indexing, otherwise we are
 * post-indexing.
 * @param upBit If set means offset is added to the base register, otherwise
 * the offset is subtracted.
 * @param ldBit If set means we are loading, otherwise we are storing
 * @param offset To be added/subtracted to the base register
 * @param state A structure representing the machine state
 */
void executeSingleTransfer(REGISTER *baseReg,
						   REGISTER *targetReg,
						   REGISTER *rmReg,
						   int preBit,
						   int upBit,
						   int ldBit,
						   int offset,
						   MACHINE_STATE state) {
  uint32_t address;
  if (preBit) {
	/* Pre-indexing: the offset is added/subtracted to the
	 * base register before transferring the data. Does not
	 * change base register value
	 */
	if (upBit) {
	  // Offset added
	  address = *baseReg + offset;
	} else {
	  // Offset subtracted
	  address = *baseReg - offset;
	}
  } else {
	/* Post-indexing: the offset is added/subtracting after
	 * transferring the data. Changes base register value.
	 * A post-indexing ldr or str in which rm is the same
	 * register as Rn, is not allowed.
	 */
	assert(rmReg != baseReg);
	address = *baseReg;
	if (upBit) {
	  *baseReg += offset;
	} else {
	  *baseReg -= offset;
	}
  }
  if (address > MAX_ADDRESSES - 1) {
	// Handles case where address is bigger than 16 bits
	printf("Error: Out of bounds memory access at address 0x%08x\n", address);
  } else {
	ADDRESS address32bit = (ADDRESS) address;
	if (ldBit) {
	  // Load instruction
	  *targetReg = getWord(address32bit, state);
	} else {
	  // Store instruction
	  storeWord(*targetReg, address32bit, state);
	}
  }
}

/**
 * executeBranch function takes in the offset value which is to be added to program counter.
 *
 * @param offset is the amount that the program counter needs to be incremented by.
 * @param state is the current machine state.
 */
void executeBranch(int offset, MACHINE_STATE state) {
  state.registers[15] += offset;
}

/**
 * execute function takes in a struct as an arguement and depending
 * on the type of the instruction, the relevant values are passed onto the helper
 * functions for each instruction type.
 *
 * @param decoded is the struct containing all the information from the function decode.
 * @param state is the current machine state.
 * @param toDecode/toExecute represent whether each respective part of the pipeline will
 * run in the next cycle.
 */
void execute(DECODED_INSTR decoded, MACHINE_STATE state, int *toDecode, int *toExecute) {
  if (willExecute(decoded.condition, state)) {
	REGISTER *rdReg;
	REGISTER rnReg;
	REGISTER rsReg;
	REGISTER rmReg;
	switch (decoded.type) {
	  case MULTIPLY:rdReg = &state.registers[decoded.rd];
		rnReg = state.registers[decoded.rn];
		rsReg = state.registers[decoded.rs];
		rmReg = state.registers[decoded.rm];
		//TODO: access registers in individual execute functions, not out here
		executeMultiply(rdReg, rnReg, rsReg, rmReg, decoded.A, decoded.S, state);
		break;

	  case BRANCH:*toDecode = 0;
		*toExecute = 0;
		int shiftedOffset = decoded.offset << 2;
		int offset = signExtend(shiftedOffset, 25);
		executeBranch(offset, state);
		break;
	  case PROCESSING:rdReg = &state.registers[decoded.rd];
		rnReg = state.registers[decoded.rn];
		executeProcessing(rdReg, rnReg, decoded.opcode, decoded.op2, decoded.S, decoded.shiftCarryOut, state);
		break;
	  case TRANSFER: {
		REGISTER *rnReg = &state.registers[decoded.rn];
		REGISTER *rdReg = &state.registers[decoded.rd];
		REGISTER *rmReg = &state.registers[decoded.rm];
		executeSingleTransfer(rnReg, rdReg, rmReg, decoded.P, decoded.U, decoded.L, decoded.offset, state);
		break;
	  }
	  case HALT:break;
	}
  }
}
