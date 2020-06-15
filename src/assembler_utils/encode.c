#include "encode.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define PC_REG_NUM 15

/**
 * encode.c contains functions for encoding each type of instruction.
 */


/**
 * encodeInstruction finds the type of the instruction using its opcode, and
 * passes the relevant arguments to the suitable encode function. For load
 * instructions the function checks whether the instruction can be treated as
 * a mov.
 *
 * @param tokens: A pointer to our INSTR_TOKENS structure, representing the
 * tokenised form of the instruction.
 * @param symbolTable: A pointer to our symbolTable hashmap, which stores labels
 * and their addresses, and mnemonics with their enum value.
 * @param constants: A pointer to our SDT_CONSTANTS struct which stores the
 * load constants to be printed at the end of the file.
 * @return An INSTRUCTION (a 32 bit unsigned int) result of encoding the tokenised
 * instruction
 */
INSTRUCTION encodeInstruction(INSTR_TOKENS *tokens, Map *symbolTable, SDT_CONSTANTS *constants) {
  if (isProcessing(tokens->opcode))
	return dpi(tokens, symbolTable);

  if (isMultiply(tokens->opcode))
	return multiply(tokens);

  if (isTransfer(tokens->opcode)) {
	if (tokens->opcode == LDR && tokens->noOfRegisters == 1 && tokens->immediateEquals[0] < 0xff) {
	  tokens->immediateHash[0] = tokens->immediateEquals[0];
	  tokens->immediateEquals[0] = 0;
	  tokens->noOfImmsHash = 1;
	  tokens->noOfImmsEquals = 0;
	  tokens->opcode = MOV;
	  return dpi(tokens, symbolTable);
	}
	return sdt(tokens, constants);
  }

  if (isBranch(tokens->opcode))
	return branch(tokens);

  if (tokens->opcode == LSL) {
	tokens->shift = "lsl";
	tokens->opcode = MOV;
	tokens->registers[1] = tokens->registers[0];
	tokens->noOfRegisters++;
	return dpi(tokens, symbolTable);
  }
  return EXIT_FAILURE;
}

/**
 * For a given branch instruction, returns its condition.
 */
int getCondition(int opcode) {
  switch (opcode) {
	case B:return AL;
	case BEQ:return EQ;
	case BNE:return NE;
	case BGE:return GE;
	case BLT:return LT;
	case BGT:return GT;
	case BLE:return LE;
	default:return AL;
  }
}

/**
 * For a given shift string, returns its binary value.
 */
unsigned int convertShift(char *shift) {
  if (!shift || !strcmp(shift, "lsl"))
	return 0;

  if (strcmp(shift, "lsr") == 0)
	return 1;

  if (strcmp(shift, "asr") == 0)
	return 2;

  if (strcmp(shift, "ror") == 0)
	return 3;

  perror("Invalid shift type");
  return -1;
}

/**
 * @param tokens A struct representing the instruction to be encoded.
 * @param I The value of the I flag of the corresponding instruction.
 * @return A 16-bit binary number representing operand2 (top 4 bits are unused).
 */
uint16_t findOperand2(INSTR_TOKENS *tokens, int I) {
  uint16_t operand2 = 0;
  uint32_t expr;
  if (I) {
	expr = tokens->immediateHash[0];
	//immediate and rotation
	int foundRepresentation = 0;
	for (int rotate = 0; rotate < 32; rotate += 2) {
	  if (!(expr & ~0xff)) {
		operand2 = expr + (rotate / 2 << 8);
		foundRepresentation = 1;
		break;
	  }
	  expr = rotateLeft(expr, 2);
	}
	CHECK_PRED(!foundRepresentation, "Numeric constant can't be represented");
  } else { // I == 0
	//set the shift type bits (5 and 6)
	if (tokens->shift == NULL) {
	  operand2 = 0;
	} else if (!strcmp(tokens->shift, "lsr")) {
	  operand2 = setBit(operand2, 5);
	} else if (!strcmp(tokens->shift, "asr")) {
	  operand2 = setBit(operand2, 6);
	} else if (!strcmp(tokens->shift, "ror")) {
	  operand2 = setBits(operand2, 2, 5);
	} // otherwhise tokens->shift should be lsl with code 00
	// OR, there is no shift specified, just a register.

	switch (tokens->opcode) {
	  case AND:
	  case EOR:
	  case SUB:
	  case RSB:
	  case ADD:
	  case ORR: operand2 |= tokens->registers[2];
		if (tokens->noOfRegisters == 3) {
		  operand2 |= tokens->immediateHash[0] << 7;
		} else if (tokens->noOfRegisters == 4) { // should be the only other case
		  operand2 = setBit(operand2, 4);
		  operand2 |= tokens->registers[3] << 8;
		}
		break;
	  case MOV:
	  case TST:
	  case TEQ:
	  case CMP: operand2 |= tokens->registers[1];
		if (tokens->noOfRegisters == 2) {
		  operand2 |= tokens->immediateHash[0] << 7;
		} else if (tokens->noOfRegisters == 3) { // should be the only other case
		  operand2 = setBit(operand2, 4);
		  operand2 |= tokens->registers[2] << 8;
		}
		break;
	  default: CHECK_PRED(1, "Invalid data processing opcode. Should be unreachable.");
		break;
	}
  }
  return operand2;
}

/**
 * Takes in the tokenized form of an ARM instruction and uses the values to encode the specified data processing instruction.
 *
 * @param tokens is a pointer to a struct containing all the information required passed in from tokenizer
 * @return the equivalent encoded instruction of type INSTRUCTION
 */
INSTRUCTION dpi(INSTR_TOKENS *tokens, Map *map) {
  INSTRUCTION instr;
  int opcode = tokens->opcode;
  int rn, rd, I, S, operand2;

  switch (opcode) {
	case ANDEQ: assert(tokens->noOfRegisters == 3);
	  return 0;
	case TST:
	case TEQ:
	case CMP: S = 1;
	  I = (tokens->shift == NULL && tokens->noOfRegisters == 1) ? 1 : 0;
	  rd = 0;
	  rn = tokens->registers[0];
	  break;
	case MOV: S = 0;
	  I = (tokens->shift == NULL && tokens->noOfRegisters == 1) ? 1 : 0;
	  rd = tokens->registers[0];
	  rn = 0;
	  break;
	default: // and, eor, sub etc...
	  S = 0;
	  I = (tokens->shift == NULL && tokens->noOfRegisters == 2) ? 1 : 0;
	  rd = tokens->registers[0];
	  rn = tokens->registers[1];
	  break;
  }

  operand2 = findOperand2(tokens, I);

  instr = (AL << 28) + (I << 25) + (opcode << 21) + (S << 20) + (rn << 16) + (rd << 12) + (operand2);

  return instr;
}

/**
 * Takes in the tokenized form of an ARM instruction and uses the values to encode the specified multiply instruction.
 *
 * @param *tokens is a pointer to a struct containing all the information required passed in from tokenizer
 * @return the equivalent encoded instruction of type INSTRUCTION
 */
INSTRUCTION multiply(INSTR_TOKENS *tokens) {
  int accumulate = (tokens->opcode == MLA);
  int rd = tokens->registers[0];
  int rm = tokens->registers[1];
  int rs = tokens->registers[2];
  int rn = (accumulate) ? tokens->registers[3] : 0;
  return (AL << 28) + (accumulate << 21) + (rd << 16) + (rn << 12) + (rs << 8) + (9 << 4) + rm;

}

/**
 * @param *symbols is an array of type TOKEN_TYPE containing all the symbols in order from a given instruction
 * @param len The length of symbols.
 * @return the index of the second register in the array.
 */
unsigned int findSecondReg(TOKEN_TYPE *symbols, int len) {
  int regCount = 0;
  for (int i = 0; i < len; i++) {
	if (symbols[i] == REG) {
	  regCount++;
	  if (regCount == 2)
		return i;
	}
  }
  fprintf(stderr, "Malformed SDT instruction.");
  return EXIT_FAILURE;
}

/**
 * Takes in the tokenized form of an ARM instruction and uses the
 * values to encode the specified data transfer instruction.
 *
 * @param tokens is a pointer to a struct containing all the
 * information required passed in from tokenizer
 * @param constants is a pointer to a struct which holds an array and the size of the array. This is used to store immediate values
 * which need to be written after the rest of the instructions are
 * assembled.
 * @return the equivalent encoded instruction of type INSTRUCTION
 */
INSTRUCTION sdt(INSTR_TOKENS *tokens, SDT_CONSTANTS *constants) {
  int I = 0;
  int L = (tokens->opcode == LDR);
  int P = 1;
  int U = tokens->sign != '-';
  REGISTER rn;
  REGISTER rd = tokens->registers[0];
  int offset = 0;
  if (tokens->noOfRegisters == 1) {
	// form ldr r1 =1
	rn = PC_REG_NUM;
	rd = tokens->registers[0];
	offset = (constants->noOfInstructions + constants->size) * 4 - tokens->currAddr - 8;
	addToConstants(constants, tokens->immediateEquals[0]);
  } else if (tokens->noOfRegisters == 2 && tokens->noOfImmsHash == 0) {
	// form ldr r1 [r2]
	rn = tokens->registers[1];
  } else if (tokens->noOfRegisters == 3) {
	//form ldr r1 r2, r3, shift, #imm
	rn = tokens->registers[1];
	I = 1;
	P = (tokens->symbols[findSecondReg(tokens->symbols, tokens->noOfSymbols) + 1] != CLOSE);
	offset = (tokens->immediateHash[0] << 7) + (convertShift(tokens->shift) << 5) + tokens->registers[2];
  } else {
	//form ldr r1, r2, #imm
	rn = tokens->registers[1];
	offset = tokens->immediateHash[0];
	P = (tokens->symbols[findSecondReg(tokens->symbols, tokens->noOfSymbols) + 1] != CLOSE);
  }
  return (AL << 28) + (1 << 26) + (I << 25) + (P << 24) + (U << 23) + (L << 20) + (rn << 16) + (rd << 12) + offset;
}

/**
 * Takes in the tokenized form of an ARM instruction and uses the values to encode the specified branch instruction.
 *
 * @param *tokens is a pointer to a struct containing all the information required passed in from tokenizer
 * @return the equivalent encoded instruction of type INSTRUCTION
 */
INSTRUCTION branch(INSTR_TOKENS *tokens) {
  int cond = getCondition(tokens->opcode);
  int offset = tokens->branchAddr - tokens->currAddr - 8;
  // We minus 8 to take in to account the PC pipelining offset
  if (offset > 0x3FFFFFF || tokens->currAddr + offset > MAX_ADDRESSES) {
	fprintf(stderr, "Branch jump is too large");
  } else {
	offset >>= 2;
  }
  return (cond << 28) + (10 << 24) + getBits(offset, 24, 0);
}
