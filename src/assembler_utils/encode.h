#include "tokenizer.h"
#include "sdtconstants.h"

/**
 * Header file for encode, containing function declarations
 * for each instruction's encode function
 */

#ifndef ENCODE_H
#define ENCODE_H
INSTRUCTION encode_instruction(INSTR_TOKENS *tokens, Map *symbolTable, SDT_CONSTANTS *constants);

INSTRUCTION branch(INSTR_TOKENS *tokens);

INSTRUCTION dpi(INSTR_TOKENS *tokens, Map *map);

INSTRUCTION multiply(INSTR_TOKENS *tokens);

INSTRUCTION sdt(INSTR_TOKENS *tokens, SDT_CONSTANTS *constants);

#endif
