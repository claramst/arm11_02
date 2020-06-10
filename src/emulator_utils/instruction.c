#include "instruction.h"
#include <stdio.h>
#include <string.h>

/**
 * Misc. functions to do with instructions, fetching them from the PC,
 * seeing if they will execute and getting their type.
 */


/**
 * Retrieves an instruction from memory located at the address stored in the PC, increments PC by 4.
 * @param pc A pointer to the PC register.
 * @param state Struct representing the state of our ARM machine.
 * @return A 32-bit binary instruction in big endian.
 */
INSTRUCTION fetch(REGISTER *pc, MACHINE_STATE state) {
    INSTRUCTION fetched = getWord(*pc, state);
    *pc += 4;
    return fetched;
}

/**
 * The function findType takes in a 32 bit instruction and determines its instruction type.
 * @param instr The 32 bit int instruction we want to find the type for.
 * @return The instruction type in the form of the enum INSTR_TYPE.
 */
INSTR_TYPE findType(INSTRUCTION instr) {
    if (instr == 0) {
        return HALT;
    }
    if (getBit(instr, 27)) {
        return BRANCH;
    } else if (getBit(instr, 26)) {
        return TRANSFER;
    } else if (getBit(instr, 25)) {
        return PROCESSING;
    } else {
        if (getBit(instr, 4) && getBit(instr, 7)) {
            return MULTIPLY;
        } else {
            return PROCESSING;
        }
    }
}


/**
 * Used to determine if an instruction will execute based on its condition and
 * the current state of the CPSR register.
 * @param cond The condition of the instruction to be checked.
 * @param state Struct representing our ARM machine state.
 * @return 1 if instruction will execute, 0 otherwise.
 */
int willExecute(CONDITION cond, MACHINE_STATE state) {
    int cpsr = state.registers[16];
    int N = getBit(cpsr, 31);
    int Z = getBit(cpsr, 30);

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
            perror("Invalid condition code");
            return 0;
    }
}

bool isNum(char c) {
  return ('0' <= c && c <= '9') || c == '-' || c == '+';
}

bool isLetter(char c) {
  return ('a' <= c && c <= 'z');
}

bool isHex(char c) {
  return ('A' <= c && c <= 'F') || c == 'x';
}

bool isBracket(char c) {
  return (c == '[' || c == ']');
}

bool validChar(char c) {
  bool valid = isNum(c) || isLetter(c) || isHex(c) || isBracket(c);
  valid = valid || c == ' ' || c == '#' || c == ',' || c == '=' || c == ':';
  return valid;
}

bool validInstr(char *str) {
  int len = strlen(str);
  char c;
  int brackets = 0;
  int pairs = 0;
  for (int i = 0; i < len; i++) {
    c = str[i];
    switch (c) {
      case '[':
        brackets++;
        break;
      case ']':
        brackets--;
        pairs++;
        break;
      case 'x':
        if (i == 0) {
          return false;
        } else if (str[i - 1] == '0') {
          if (i == len - 1) {
            return false;
          } else if (!isNum(str[i + 1]) || str[i + i] == '-') {
            return false;
          }
        }
        break;
      case '-':
      case '#':
      case '=':
      case 'r':
        if (i == len - 1) {
          return false;
        } else if (!isNum(str[i +  1])) {
          return false;
        }
        break;
      default:
        if (!validChar(c)) {
          return false;
        }
    }
    if (brackets < 0 || pairs > 1) { 
      return false;
    }
  }
  return brackets == 0;
}
