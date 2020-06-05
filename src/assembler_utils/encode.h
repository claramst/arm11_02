#include "../emulator_utils/instruction.h"
#include "tokenizer.h"

INSTRUCTION encodeInstruction(INSTR_TOKENS *tokens, Map *symbolTable);

INSTRUCTION branch(INSTR_TOKENS *tokens);

INSTRUCTION dpi(INSTR_TOKENS *tokens, Map *map);

INSTRUCTION multiply(INSTR_TOKENS *tokens);

INSTRUCTION sdt(INSTR_TOKENS *tokens);
