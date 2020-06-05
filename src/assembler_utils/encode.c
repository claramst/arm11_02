#include "encode.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

INSTRUCTION encodeInstruction(INSTR_TOKENS *tokens, Map *symbolTable) {
    if (isProcessing(tokens->opcode)) {
        return dpi(tokens, symbolTable);
    } else if (isMultiply(tokens->opcode)) {
        return multiply(tokens);
    } else if (isTransfer(tokens->opcode)) {
        if (tokens->opcode == LDR && tokens->noOfRegisters == 1 && tokens->immediateEquals[0] < 0xff) {
            tokens->opcode = MOV;
            return dpi(tokens, symbolTable);
        } else {
            return sdt(tokens);
        }
    } else if (isBranch(tokens->opcode)) {
        return branch(tokens);
    }
    else {
        return EXIT_FAILURE;
    }
}

int getCondition(int opcode) {
    switch(opcode) {
        case B:
            return AL;
        case BEQ:
            return EQ;
        case BNE:
            return NE;
        case BGE:
            return GE;
        case BLT:
            return LT;
        case BGT:
            return GT;
        case BLE:
            return LE;
        default:
            return AL;
    }
}

INSTRUCTION branch(INSTR_TOKENS *tokens) {
    int cond = getCondition(tokens->opcode);
    int offset = tokens->branchAddr - tokens->currAddr - 8;
    // I think we minus 8 to take into account the PC being 8 bytes ahead?
    if (offset > 0x3FFFFFF || tokens->currAddr + offset > MAX_ADDRESSES) {
        fprintf(stderr, "Branch jump is too large");
    } else {
        offset >>= 2;
    }
    unsigned int branchIdentifier = 10;
    return (cond << 28) + (branchIdentifier << 24) + offset;
}

uint16_t findOperand2(INSTR_TOKENS *tokens) {
    uint16_t operand2 = 0;
    uint32_t expr;
    if (tokens->shift == NULL) { // I == 1
        expr = tokens->immediateHash[0];
        //immediate and rotation
        int foundRepresentation = 0;
        for (int rotate = 0; rotate < 16; rotate++) {
            if (!(expr & ~0xff)) {
                operand2 = expr + (rotate << 8);
                foundRepresentation = 1;
                break;
            }
            expr = rotateLeft(expr, 2);
        }
        if (!foundRepresentation) {
            fprintf(stderr, "Numeric constant can't be represented");
            return EXIT_FAILURE;
        }
    } else { // I == 0

        //set the shift type bits (5 and 6)
        if (!strcmp(tokens->shift, "lsr")) {
            operand2 = setBit(operand2, 5);
        } else if (!strcmp(tokens->shift, "asr")) {
            operand2 = setBit(operand2, 6);
        } else if (!strcmp(tokens->shift, "ror")) {
            operand2 = setBits(operand2, 2, 5);
        } // otherwhise tokens->shift should be lsl with code 00

        switch (tokens->opcode) {
            case AND:
            case EOR:
            case SUB:
            case RSB:
            case ADD:
            case ORR:
                operand2 |= tokens->registers[2];
                if (tokens->noOfRegisters == 3) {
                    operand2 |= tokens->immediateHash[0] << 7;
                } else if (tokens->noOfRegisters == 4) { // should be the only other case
                    operand2 |= tokens->registers[3] << 8;
                }
                break;
            case MOV:
            case TST:
            case TEQ:
            case CMP:
                operand2 |= tokens->registers[1];
                if (tokens->noOfRegisters == 2) {
                    operand2 |= tokens->immediateHash[0] << 7;
                } else if (tokens->noOfRegisters == 3) { // should be the only other case
                    operand2 |= tokens->registers[2] << 8;
                }
                break;
            default:
                fprintf(stderr, "Invalid data processing opcode. Should be unreachable.");
                break;
        }
    }
    return operand2;
}

INSTRUCTION dpi(INSTR_TOKENS *tokens, Map *map) {
    INSTRUCTION instr;
    int opcode = tokens->opcode;
    int rn;
    int rd;
    int I = (tokens->shift == NULL) ? 1 : 0;
    int S;
    int operand2;
    int cond = AL;

    switch(opcode) {
        case TST:
        case TEQ:
        case CMP:
            S = 1;
            rd = 0;
            rn = tokens->registers[0];
            break;
        case MOV:
            S = 0;
            rd = tokens->registers[0];
            rn = 0;
            break;
        default: // and, eor, sub etc...
            S = 0;
            rd = tokens->registers[0];
            rn = tokens->registers[1];
            break;
    }

    operand2 = findOperand2(tokens);

    instr = (cond << 28) + (I << 25) + (opcode << 21) + (S << 20) + (rn << 16) + (rd << 12) + (operand2);

    return instr;
}


INSTRUCTION multiplyAssemble(CONDITION cond, int accumulate, int rd, int rn, int rs, int rm) {
    return (cond << 28) + (accumulate << 21) + (rd << 16) + (rn << 12) + (rs << 8) + (9 << 4) + rm;
}

INSTRUCTION multiply(INSTR_TOKENS *tokens) {
    if (tokens->noOfRegisters != 4) {
        perror("Invalid number of registers for a dpi instruction");
        exit(EXIT_FAILURE);
    }
    //Register order in spec is rd, rn, rs, rm
    int accumulate = !(tokens->opcode == MLA);
    // ++ removes the preceding 'r' from the string.
    int rd = tokens->registers[0];
    int rn = tokens->registers[1];
    int rs = tokens->registers[2];
    int rm = tokens->registers[3];
    return multiplyAssemble(AL, accumulate, rd, rn, rs, rm);

}

// ldr r1 [rd]
// ldr r0, =0x42
// ldr r0, [Rn,<#expression>]

//We need to calculate the offset



// I've just been looking at this
// ldr Rd [Rn,{+/-}Rm{,<shift>}] This is just nasty if we pass in
// [Rn,{+/-}Rm{,<shift>}]
// We would have to remove multiple brackets and also tokenize again?
// maybe we have a funtion that replaces all brackets with spaces so that
// we can do a nice full tokenize to get all values split correctly. Including Pre/Post indexing.
// This could maybe be done inside tokenizer? like eg tokenize sends to    // tokenizeSDT and further breaks down so that we can set the calculated value in instrTokens.
//translateSDT and translateDPI would then be simpler.

//Found this on stack overflow
char* replace_char(char* str, char toBeReplaced, char replacement) {
    char *current_pos = strchr(str,toBeReplaced);
    while (current_pos){
        *current_pos = replacement;
        current_pos = strchr(current_pos, toBeReplaced);
    }
    return str;
}

int convertShift(char *shift)  {
    if (strcmp(shift, "lsl") == 0) {
        return 0;
    } else if (strcmp(shift, "lsr") == 0) {
        return 1;
    } else if (strcmp(shift, "asr") == 0) {
        return 2;
    } else if (strcmp(shift, "ror") == 0) {
        return 3;
    } else {
        perror("Invalid shift type");
        return -1;
    }
}

INSTRUCTION sdtAssemble(CONDITION cond, int immediateExp, int preIndex, int up, int load, REGISTER rn, REGISTER rd, int offset) {
    return (cond << 28) + (1 << 26) + (immediateExp << 25) + (preIndex << 24) + (up << 23) + (load << 20) + (rn << 16) + (rd << 12) + offset;
}

//SDT register order is rd, rn, rm
// This flow chart could definetely optimised.

INSTRUCTION sdt(INSTR_TOKENS *tokens) {
    REGISTER rn;
    REGISTER rd;
    int load = (tokens->opcode == LDR);
    int immediateExp = 0;
    int up = (tokens->sign == '+');

    int i = 0;
    while (tokens->symbols[i] != REG) {
        i++;
    }
    int preIndex = !(tokens->symbols[i + 1] == OPEN);
    int offset = 0;
    if (tokens->noOfRegisters == 1) {
        // form ldr r1 [=1]
        rn = 0;
        rd = tokens->registers[0];
        immediateExp = 1;
        offset = tokens->immediateEquals[0];
    } else if (tokens->noOfImmsHash == 0) {
        // form ldr r1 [r2]
        rd = tokens->registers[0];
        rn = tokens->registers[1];
    } else if (tokens->shift != NULL) {
        //form ldr r1 [r2, r3, shift imm]
        rd = tokens->registers[0];
        rn = tokens->registers[1];
        offset = (tokens->immediateHash[0] << 7) + (convertShift(tokens->shift) << 5) + (1 << 4) + tokens->registers[2];
    } else {
        //1 immediate value, more than 1 register, no shift
        //form ldr r1, r2, imm
        rd = tokens->registers[0];
        rn = tokens->registers[1];
        offset = tokens->registers[0];
    }
    return sdtAssemble(AL, immediateExp, preIndex, up, load, rn, rd, offset);
}
