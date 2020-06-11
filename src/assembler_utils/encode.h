#include "../emulator_utils/instruction.h"
#include "tokenizer.h"
#include "sdtconstants.h"

INSTRUCTION encodeInstruction(INSTR_TOKENS *tokens, Map *symbolTable, SDT_CONSTANTS *constants);

INSTRUCTION branch(INSTR_TOKENS *tokens);

INSTRUCTION dpi(INSTR_TOKENS *tokens, Map *map);

INSTRUCTION multiply(INSTR_TOKENS *tokens);

INSTRUCTION sdt(INSTR_TOKENS *tokens, SDT_CONSTANTS *constants);
