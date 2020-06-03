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
DECODED_INSTR decode(INSTRUCTION instr, MACHINE_STATE state) {
    struct decodedInstr decoded;
    INSTR_TYPE type = findType(instr);
    decoded.type = type;
    decoded.condition = getNibble(instr, 7);
    switch (type) {
        case MULTIPLY:
            decoded.A = getBit(instr, 21);
            decoded.S = getBit(instr, 20);
            decoded.rd = getNibble(instr, 4);
            decoded.rm = getNibble(instr, 0);
            decoded.rs = getNibble(instr, 2);
            decoded.rn = getNibble(instr, 3);
            break;
        case BRANCH:
            decoded.offset = clearBits(instr, 8, 24);
            break;
        case PROCESSING:
            decoded.opcode = getBits(instr, 4, 21);
            decoded.I = getBit(instr, 25);
            decoded.S = getBit(instr, 20);
            decoded.rn = getNibble(instr, 4);
            decoded.rd = getNibble(instr, 3);
            //Calculate value of op2
            if (decoded.I) {
                unsigned int shiftAmount = 2 * getNibble(instr, 2);
                decoded.shiftCarryOut = getBit(instr, shiftAmount);
                decoded.op2 = rotateRight(getByte(instr, 0), 2 * getNibble(instr, 2));
            } else {
                unsigned int shiftAmount;
                unsigned int shiftType = getBits(instr, 2, 5);
                decoded.rm = getNibble(instr, 0);
                if (getBit(instr, 4)) {
                    // gets the last bit of the register specified.
                    shiftAmount = getByte(state.registers[getNibble(instr, 2)], 0);
                } else {
                    // Shift by a constant amount (5-bit unsigned int).
                    shiftAmount = getBits(instr, 5, 7);
                }
                switch (shiftType) {
                    case 0:
                        //Logical shift left
                        decoded.shiftCarryOut = (shiftAmount == 0) ? 0 : getBit(instr, 32 - shiftAmount);
                        decoded.op2 = (state.registers[decoded.rm]) << shiftAmount;
                        break;
                    case 1:
                        //Logical shift right
                        decoded.shiftCarryOut = (shiftAmount == 0) ? 0 : getBit(instr, shiftAmount - 1);
                        decoded.op2 = (state.registers[decoded.rm]) >> shiftAmount;
                        break;
                    case 2:
                        //Arithmetic shift right
                        decoded.shiftCarryOut = (shiftAmount == 0) ? 0 : getBit(instr, shiftAmount - 1);
                        decoded.op2 = arithmeticRight((state.registers[decoded.rm]), shiftAmount);
                        break;
                    case 3:
                        //Rotate right
                        decoded.shiftCarryOut = (shiftAmount == 0) ? 0 : getBit(instr, shiftAmount - 1);
                        decoded.op2 = rotateRight((state.registers[decoded.rm]), shiftAmount);
                        break;
                }
            }
            break;
        case TRANSFER:
            decoded.I = getBit(instr, 25);
            decoded.P = getBit(instr, 24);
            decoded.U = getBit(instr, 23);
            decoded.L = getBit(instr, 20);
            decoded.rn = getNibble(instr, 4);
            decoded.rd = getNibble(instr, 3);
            if (decoded.I) {
                int shiftAmount;
                int shiftType = getBits(instr, 2, 5);
                decoded.rm = state.registers[getNibble(instr, 0)];
                if (getBit(instr, 4)) {
                    shiftAmount = getBit(state.registers[getNibble(instr, 2)], 31);
                } else {
                    shiftAmount = getBits(instr, 5, 7);
                }
                switch (shiftType) {
                    case 0:
                        decoded.offset = decoded.rm << shiftAmount;
                        break;
                    case 1:
                        decoded.offset = decoded.rm >> shiftAmount;
                        break;
                    case 2:
                        decoded.offset = arithmeticRight(decoded.rm, shiftAmount);
                        break;
                    case 3:
                        decoded.offset = rotateRight(decoded.rm, shiftAmount);
                        break;
                }
            } else {
                decoded.offset = getBits(instr, 12, 0);
            }
            break;
        case HALT:
            break;
        default:
            perror("Invalid instruction type. Cannot be decoded.");
    }
    return decoded;
}
