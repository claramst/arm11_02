#include "tokenizer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define IS_DIGIT(c) ('0' <= c && c <= '9')

int isProcessing(OPCODE opcode) {
  return AND <= opcode && opcode <= MOV;
}

int isMultiply(OPCODE opcode) {
  return MUL <= opcode && opcode <= MLA;
}

int isTransfer(OPCODE opcode) {
  return LDR <= opcode && opcode <= STR;
}

int isBranch(OPCODE opcode) {
  return BEQ <= opcode && opcode <= B;
}
/*
void tokenizeOp2(INSTR_TOKENS *tokens, char **strings) {
   if (strchr(strings[0],'#')) {
      tokens->opType = IMMEDIATE;
      tokens->expression = strings[0];
  } else {
      tokens->rm = strings[0];
      tokens->shiftType = strings[1];
      if (strchr(strings[2], '#')) {
        tokens->opType = SHIFTED_IMM;
        tokens->expression = strings[2];
      } else {
        tokens->opType = SHIFTED_REG;
        tokens->rs = strings[2];
      }
  }
}
*/

void tokenizeBranch(char *instrLine, int address, Map *symbolTable, INSTR_TOKENS *tokens) {
  tokens->currAddr = address;
  char **tokenisedInstruction = NULL;
  char *temp;
  char *token = strtok_r(instrLine, " ", &temp);
  for (int i = 0; token; i++) {
	tokenisedInstruction[i] = token;
	token = strtok_r(NULL, " ", &temp);
  }

  char *branchAddress = tokenisedInstruction[2];
  if (getValue(symbolTable, branchAddress) == -1) {
	// Address is in hex
	char *endptr;
	tokens->branchAddr = strtol(&branchAddress[2], &endptr, 16);
  } else {
	tokens->branchAddr = getValue(symbolTable, branchAddress);
  }
  tokens->opcode = getValue(symbolTable, tokenisedInstruction[0]);
}

int *getValues(char *str, char start, int max, int *length) {
  int *values = malloc(max * sizeof(int));
  if (!values) {
    perror("Error allocating values memory");
    exit(EXIT_FAILURE);
  }
  int len = strlen(str);
  int index = 0;
  for (int i = 0; i < len - 1; i++) {
	if (str[i] == start /*&& str[i - 1] != 'd'*/) {
	  int end;
	  for (end = i + 1; IS_DIGIT(str[end]) || str[end] == 'x'; end++);
	  if (end - i == 1) {
		continue;
	  }
	  char num[end - i - 1];
	  strncpy(&num[0], &str[i + 1], end - i);
	  values[index] = atoi(num) | strtol(num, NULL, 16);
	  index++;
	}
  }
  *length = index;
  return values;
}

INSTR_TOKENS *tokenize(char *instrLine, int address, Map *symbolTable) {
  INSTR_TOKENS *tokens = (INSTR_TOKENS *) malloc(sizeof(INSTR_TOKENS));
  if (!tokens) {
    perror("Error allocating tokens memory");
    exit(EXIT_FAILURE);
  }

  char str[strlen(instrLine)];
  strcpy(str, instrLine);
  char *s = strtok(str, " ");
  OPCODE opcode = getValue(symbolTable, s);
  if (isBranch(opcode)) {
	tokenizeBranch(instrLine, address, symbolTable, tokens);
	return tokens;
  }
//    char **strings = malloc(512 * sizeof(char));
  int index = 0;
  int MAX = (int) strlen(instrLine);
  tokens->symbols = calloc(MAX, sizeof(TOKEN_TYPE));

  for (int i = 0; i < MAX; i++) {
	switch (instrLine[i]) {
	  case 'r':
		if (IS_DIGIT(instrLine[i + 1])) {
		  tokens->symbols[index++] = REG;
		}
    break;
	  case '[':tokens->symbols[index++] = OPEN;
    break;
	  case ']':tokens->symbols[index++] = CLOSE;
    break;
	  case '+':tokens->symbols[index++] = PLUS;
    break;
	  case '-':tokens->symbols[index++] = MINUS;
    break;
	  case '#':tokens->symbols[index++] = HASHTAG;
    break;
	  case '=':tokens->symbols[index++] = EQUAL;
    break;
	}
  }
  /*
  //Loop populates char **strings
  for (token = strtok_r(instrLine, seps, &saveptr);
	   token;
	   token = strtok_r(NULL, seps, &saveptr)) {
		 strings[i++] = token;
	   }
  //mov r1 r2 split into ["mov", "r1", "r2"]
  //mov r1,r2 split into ["mov", "r1", "r2"]
  */



  tokens->opcode = opcode;
  tokens->registers = getValues(instrLine, 'r', 4, &tokens->noOfRegisters);
  tokens->immediateHash = getValues(instrLine, '#', 1, &tokens->noOfImmsHash);
  tokens->immediateEquals = getValues(instrLine, '=', 1, &tokens->noOfImmsEquals);
  if (strstr(instrLine, "lsl")) {
	tokens->shift = "lsl";
  } else if (strstr(instrLine, "lsr")) {
	tokens->shift = "lsr";
  } else if (strstr(instrLine, "asr")) {
	tokens->shift = "asr";
  } else if (strstr(instrLine, "ror")) {
	tokens->shift = "ror";
  } else {
	tokens->shift = NULL;
  }
  if (strstr(instrLine, "+")) {
	tokens->sign = '+';
  } else if (strstr(instrLine, "-")) {
	tokens->sign = '-';
  } else {
	tokens->sign = '\0';
  }
  return tokens;
}

void freeTokens(INSTR_TOKENS *tokens) {
  free(tokens->registers);
  free(tokens->immediateHash);
  free(tokens->immediateEquals);
  free(tokens->symbols);
  free(tokens);
}
//   INSTR_TOKENS *tokens = (INSTR_TOKENS *) malloc(sizeof(INSTR_TOKENS));
//   tokens->arrayOfTokens = strings;
//   tokens->mnemonic = strings[0];
//   tokens->opcode = getValue(symbolTable, strings[0]);
//   if (isProcessing(tokens->opcode)) {
//     tokens->type = PROCESSING;
//     switch (tokens->opcode) {
//       case AND:
//       case EOR:
//       case SUB:
//       case RSB:
//       case ADD:
//       case ORR:
//         tokens->rd = strings[1];
//         tokens->rn = strings[2];
//         tokenizeOp2(tokens, strings + 3);
//         break;
//       case MOV:
//         tokens->rd = strings[1];
//         tokenizeOp2(tokens, strings + 2);
//         break;
//       case TST:
//       case TEQ:
//       case CMP:
//         tokens->rn = strings[1];
//         tokenizeOp2(tokens, strings + 2);
//         break;
//       default:
//         fprintf(stderr, "should be impossible to reach");
//     }
//   } else if (isMultiply(tokens->opcode)) {
//     tokens->type = MULTIPLY;
//     tokens->rd = strings[1];
//     tokens->rm = strings[2];
//     tokens->rs = strings[3];
//     if (!strcpy(tokens->mnemonic, "mla")) {
//       tokens->rn = strings[4];
//     }
//   } else if (isTransfer(tokens->opcode)) {
//     tokens->type = TRANSFER;
//     //todo
//   } else if (isBranch(tokens->opcode)) {
//     tokens->type = BRANCH;
//     //todo
//   }
//   return tokens;
// }
