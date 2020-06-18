#include "tokenizer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define IS_DIGIT(c) (('0' <= c && c <= '9') || c == '-')
#define IS_HEX(c) (('A' <= c && c <= 'F') || (('a' <=c && c <= 'f')) || c == 'x')

/**
 * tokenizer.c defines functions for 'tokenizing' ARM instruction strings,
 * ultimately producing a structure representing the information
 * required for an instruction to be encoded.
 */


/**
 * For a given opcode, These functions check whether or not it represents a certain type of instruction.
 */
int isProcessing(OPCODE opcode) {
  return (AND <= opcode && opcode <= MOV) || opcode == ANDEQ;
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

/**
 * A helper function for tokenizing branch instructions specifically.
 *
 */
void tokenize_branch(char *instrLine, int address, Map *symbolTable, INSTR_TOKENS *tokens) {
  char *tokenisedInstruction[2];
  char *temp, *token = strtok_r(instrLine, " ", &temp);
  for (int i = 0; token; i++) {
    tokenisedInstruction[i] = token;
    token = strtok_r(NULL, " ", &temp);
  }

  char *branchAddress = tokenisedInstruction[1];
  if (get_value(symbolTable, branchAddress) == -1) {
	// Address is in hex
	  char *endptr;
	  tokens->branchAddr = strtol(&branchAddress[2], &endptr, 16);
  } else
	  tokens->branchAddr = get_value(symbolTable, branchAddress);

  tokens->opcode = get_value(symbolTable, tokenisedInstruction[0]);
}

/**
 * Goes through a string, retrieving all numbers that are preceded by a common character.
 * e.g. from "add r3,r2,#1" we could return the list [3,2].
 * @param str The string to be iterated through
 * @param start The character that precedes the numbers to be retrieved.
 * @param max The maximum possible amount of numbers that should be retrieved.
 * @param length A pointer to a location where the amount of numbers retrieved should be written.
 * @return an array of ints.
 */
int *get_values(char *str, char start, int max, int *length) {
  int *values = calloc(max, sizeof(int));
  if (!values) {
	perror("Error allocating values memory");
	exit(EXIT_FAILURE);
  }
  int len = strlen(str);
  int index = 0;
  int isHex = 0;
  for (int i = 0; i < len - 1; i++) {
	if (str[i] == start) {
	  int end;
	  for (end = i + 1; IS_DIGIT(str[end]) || IS_HEX(str[end]); end++) {
		if (IS_HEX(str[end])) {
		  isHex = 1;
		}
	  }
	  if (end - i == 1) {
		continue;
	  }
	  char num[end - i];
	  if (isHex) {
		strncpy(&num[0], &str[i + 1], end - i - 1);
		num[end - i - 1] = '\0';
		values[index] = labs(strtol(num, NULL, 16));
	  } else {
		strncpy(&num[0], &str[i + 1], end - i - 1);
		num[end - i - 1] = '\0';
		values[index] = abs(atoi(num));
	  }
	  index++;
	}
  }
  *length = index;
  return values;
}

/**
 * Tokenizes an assembly instruction by extracting the register, immediate * and shift values and key characters
 * @param instrLine The string representing an assembly instruction to be tokenised.
 * @param address The index of the string in the lines of the program
 * @param  symbolTable Contains mappings from the labels to their
 *                     address as well as from the opcode strings to their enum values
 * @return tokens Structure to store all the inforamtion requires to
 *                convert the assembly instruction to a machine code instruction
 */
INSTR_TOKENS *tokenize(char *instrLine, int address, Map *symbolTable) {
  INSTR_TOKENS *tokens = (INSTR_TOKENS *) malloc(sizeof(INSTR_TOKENS));
  if (!tokens) {
    perror("Error allocating tokens memory");
    exit(EXIT_FAILURE);
  }
  tokens->registers = NULL;
  tokens->immediateHash = NULL;
  tokens->immediateEquals = NULL;
  tokens->symbols = NULL;
  tokens->currAddr = 4 * address;

  char *str = calloc(strlen(instrLine) + 1, sizeof(char));
  strcpy(str, instrLine);
  char *s = strtok(str, " ");

  OPCODE opcode = get_value(symbolTable, s);

  if (isBranch(opcode)) {
    tokenize_branch(instrLine, address, symbolTable, tokens);

    return tokens;
  }
  int index = 0;
  int MAX = (int) strlen(instrLine);
  tokens->symbols = calloc(MAX, sizeof(TOKEN_TYPE));
  int i;
  for (i = 0; i < MAX; i++) {
	switch (instrLine[i]) {
	  case 'r':
		if (IS_DIGIT(instrLine[i + 1]))
		  tokens->symbols[index++] = REG;
		break;
	  case '[': tokens->symbols[index++] = OPEN;
		break;
	  case ']': tokens->symbols[index++] = CLOSE;
		break;
	  case '+': tokens->symbols[index++] = PLUS;
		break;
	  case '-': tokens->symbols[index++] = MINUS;
		break;
	  case '#': tokens->symbols[index++] = HASHTAG;
		break;
	  case '=': tokens->symbols[index++] = EQUAL;
		break;
	}
  }
  tokens->noOfSymbols = index;

  tokens->opcode = opcode;
  tokens->registers = get_values(instrLine, 'r', 4, &tokens->noOfRegisters);
  tokens->immediateHash = get_values(instrLine, '#', 1, &tokens->noOfImmsHash);
  tokens->immediateEquals = get_values(instrLine, '=', 1, &tokens->noOfImmsEquals);

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
  free(str);

  return tokens;
}

/**
 * For a given INSTR_TOKENS struct, frees the struct and its elements.
 */
void free_tokens(INSTR_TOKENS *tokens) {
  free(tokens->registers);
  free(tokens->immediateHash);
  free(tokens->immediateEquals);
  free(tokens->symbols);
  free(tokens);
}
