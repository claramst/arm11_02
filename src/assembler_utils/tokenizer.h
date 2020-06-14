#include "hashmap.h"
#include "../emulator_utils/decode.h"

#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum token {
  REG,
  HASHTAG,
  EQUAL,
  COMMA,
  PLUS,
  MINUS,
  OPEN,
  CLOSE
} TOKEN_TYPE;

typedef struct instrTokens {
  OPCODE opcode;
  int *registers;
  int noOfRegisters;
  int *immediateHash;
  int noOfImmsHash;
  int *immediateEquals;
  int noOfImmsEquals;
  char *shift;
  char sign;
  ADDRESS branchAddr;
  ADDRESS currAddr;
  TOKEN_TYPE *symbols;
  int noOfSymbols;
} INSTR_TOKENS;

void freeTokens(INSTR_TOKENS *tokens);
int isProcessing(OPCODE opcode);
int isMultiply(OPCODE opcode);
int isTransfer(OPCODE opcode);
int isBranch(OPCODE opcode);
INSTR_TOKENS *tokenize(char *instrLine, int address, Map *symbolTable);
// Maybe not needed, pass an array of tokens to represent op2?
#endif

