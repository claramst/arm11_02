#include "decode.h"
#include "state.h"
#include <stdio.h>
/**
 * decode.c defines functions for 'decoding' big-endian 32-bit words into ARM
 * instructions, ultimately producing a structure representing the decoded
 * instruction to be executed.
 */

/**
 * The function decode takes in a 32 bit int instruction and the machine state to
   calculate and set all the necessary values in the return struct.

 * @param instr is the 32 bit int instruction to be decoded.
 * @returns A struct containing all the relevant information needed to for the instruction to be executed.
 */
DECODED_INSTR decode(INSTRUCTION instr, MACHINE_STATE *state) {
  struct decodedInstr decoded;
  INSTR_TYPE type = findType(instr);
  decoded.type = type;
  decoded.condition = get_nibble(instr, 7);
  switch (type) {
	case MULTIPLY:decoded.A = get_bit(instr, 21);
	  decoded.S = get_bit(instr, 20);
	  decoded.rd = get_nibble(instr, 4);
	  decoded.rm = get_nibble(instr, 0);
	  decoded.rs = get_nibble(instr, 2);
	  decoded.rn = get_nibble(instr, 3);
	  break;
	case BRANCH:decoded.offset = get_bits(instr, 24, 0);
	  break;
	case PROCESSING:decoded.opcode = get_bits(instr, 4, 21);
	  decoded.I = get_bit(instr, 25);
	  decoded.S = get_bit(instr, 20);
	  decoded.rn = get_nibble(instr, 4);
	  decoded.rd = get_nibble(instr, 3);
	  //Calculate value of op2
	  if (decoded.I) {
		unsigned int shiftAmount = 2 * get_nibble(instr, 2);
		decoded.shiftCarryOut = get_bit(instr, shiftAmount);
		decoded.op2 = rotate_right(get_byte(instr, 0), 2 * get_nibble(instr, 2));
	  } else {
		unsigned int shiftAmount;
		unsigned int shiftType = get_bits(instr, 2, 5);
		decoded.rm = get_nibble(instr, 0);
		if (get_bit(instr, 4)) {
		  // gets the last bit of the register specified.
		  shiftAmount = get_byte(state->registers[get_nibble(instr, 2)], 0);
		} else {
		  // Shift by a constant amount (5-bit unsigned int).
		  shiftAmount = get_bits(instr, 5, 7);
		}
		switch (shiftType) {
		  case 0:
			//Logical shift left
			decoded.shiftCarryOut = (shiftAmount == 0) ? 0 : get_bit(instr, 32 - shiftAmount);
			decoded.op2 = (state->registers[decoded.rm]) << shiftAmount;
			break;
		  case 1:
			//Logical shift right
			decoded.shiftCarryOut = (shiftAmount == 0) ? 0 : get_bit(instr, shiftAmount - 1);
			decoded.op2 = (state->registers[decoded.rm]) >> shiftAmount;
			break;
		  case 2:
			//Arithmetic shift right
			decoded.shiftCarryOut = (shiftAmount == 0) ? 0 : get_bit(instr, shiftAmount - 1);
			decoded.op2 = arithmetic_right((state->registers[decoded.rm]), shiftAmount);
			break;
		  case 3:
			//Rotate right
			decoded.shiftCarryOut = (shiftAmount == 0) ? 0 : get_bit(instr, shiftAmount - 1);
			decoded.op2 = rotate_right((state->registers[decoded.rm]), shiftAmount);
			break;
		}
	  }
	  break;
	case TRANSFER:decoded.I = get_bit(instr, 25);
	  decoded.P = get_bit(instr, 24);
	  decoded.U = get_bit(instr, 23);
	  decoded.L = get_bit(instr, 20);
	  decoded.rn = get_nibble(instr, 4);
	  decoded.rd = get_nibble(instr, 3);
	  decoded.rm = 0;
	  if (decoded.I) {
		int shiftAmount;
		int shiftType = get_bits(instr, 2, 5);
		decoded.rm = state->registers[get_nibble(instr, 0)];
		if (get_bit(instr, 4)) {
		  shiftAmount = get_bit(state->registers[get_nibble(instr, 2)], 31);
		} else {
		  shiftAmount = get_bits(instr, 5, 7);
		}
		switch (shiftType) {
		  case 0:decoded.offset = decoded.rm << shiftAmount;
			break;
		  case 1:decoded.offset = decoded.rm >> shiftAmount;
			break;
		  case 2:decoded.offset = arithmetic_right(decoded.rm, shiftAmount);
			break;
		  case 3:decoded.offset = rotate_right(decoded.rm, shiftAmount);
			break;
		}
	  } else {
		decoded.offset = get_bits(instr, 12, 0);
	  }
	  break;
	case HALT:break;
	default:perror("Invalid instruction type. Cannot be decoded.");
  }
  return decoded;
}
