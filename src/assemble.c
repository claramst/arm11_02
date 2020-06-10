#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emulator_utils/state.h"
#include "assembler_utils/encode.h"
#include "binary_utils/bitmanipulation.h"

#define MAX_LINE_LENGTH 511

//PRE: fileName is not null
int countLines(FILE *fptr) {
  int lines = 0;
  char c;
  while (!feof(fptr)) {
	c = fgetc(fptr);
	lines += (c == '\n');
  }
  return lines;
}

// Writes n words (in big endian) to the file with the given name
// n is the number of instructions
void writeToFile(FILE *file, WORD word) {
  BYTE *byte = malloc(sizeof(BYTE));
  for (int j = 0; j < 4; j++) {
	*byte = getByte(word, j);
	fwrite(byte, sizeof(BYTE), 1, file);
  }
//  fflush(file);
//  free(byte);
}

int main(int argc, char **argv) {


  if (argc != 3) {
	fprintf(stderr, "Invalid argument");
	return EXIT_FAILURE;
  }

  char *sourceFileName = argv[1];
  char *outputFileName = argv[2];


  FILE *sourceFile = fopen(sourceFileName, "r");
  if (sourceFile == NULL) {
	fprintf(stderr, "File could not be opened");
	return EXIT_FAILURE;
  }
  
  //Note noOfLines includes label lines in the count.
  int noOfLines = countLines(sourceFile);
  char **array_of_lines = (char **) calloc(noOfLines, sizeof(char *));
  rewind(sourceFile);
  for (int i = 0; i < noOfLines; i++) {
//        fscanf(sourceFile, "%[^\n]%*c", array_of_lines[i]);
	array_of_lines[i] = (char *) calloc(1, MAX_LINE_LENGTH* sizeof(char));
	fgets(array_of_lines[i], MAX_LINE_LENGTH, sourceFile);
	array_of_lines[i][strlen(array_of_lines[i]) - 1] = '\0';
	// char* line = strtok_r(temp, "\n", &temp);
  }
  if (ferror(sourceFile)) {
	printf("An error has occurred whilst file reading");
	exit(EXIT_FAILURE);
  }

  fclose(sourceFile);

  // First pass:
  // Create symbol table, alloc memory
  // Associate labels with memory addresses


  char **array_of_instructions = calloc(noOfLines, sizeof (char *));
  Map *symbolTable = createMap();
  int noOfInstructions = 0;
  for (int i = 0; i < noOfLines; i++) {
	//Checks if the line at array_of_lines[i] contains ':'
	char *str = strchr(array_of_lines[i], ':');
	if (str) {
	  char *temp = array_of_lines[i];
	  char *label = strtok(temp, ":");
	  int address = 4 * noOfInstructions;
	  addNode(symbolTable, label, address);
	} else {
	  array_of_instructions[noOfInstructions] = (char *) calloc(1, MAX_LINE_LENGTH* sizeof(char));
	  array_of_instructions[noOfInstructions] = array_of_lines[i];
	  noOfInstructions++;
	}
  }
  // Add opcodes and values to symbol table
  char *mnemonics[] =
	  {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov", "ldr", "str", "beq", "bne", "bge", "blt",
		  "bgt", "ble", "b", "lsl", "andeq", "mul", "mla"};
  int mnemonicValues[] =
	  {AND, EOR, SUB, RSB, ADD, TST, TEQ, CMP, ORR, MOV, LDR, STR, BEQ, BNE, BGE, BLT, BGT, BLE, B, LSL, ANDEQ, MUL,
		  MLA};
  addNodes(symbolTable, mnemonics, mnemonicValues, 23);


  // Second pass:
  // In the second pass the assembler reads in the opcode mnemonic and
  // operand field(s) for each instruction and generates the corresponding binary encoding of that instruction.
  // As part of this process it replaces label references in operand fields with their corresponding addresses,
  // as defined in the symbol table computed during the first pass.

  FILE *outputFile = fopen(outputFileName, "ab");
  if (!outputFile) {
	fprintf(stderr, "File already exists");
	return EXIT_FAILURE;
  }


  // Assemble into binary, use filewriter
  for (int i = 0; i < noOfInstructions; i++) {
//	if (!strcmp(array_of_lines[i], ":")) {
//	  continue;
//	}
	INSTR_TOKENS *tokens = tokenize(array_of_instructions[i], i, symbolTable);
	INSTRUCTION instr = encodeInstruction(tokens, symbolTable);
	freeTokens(tokens);
	writeToFile(outputFile, instr);
  }

  for (int i = 0; i < noOfLines; i++) {
    free(array_of_lines[i]);
  }
  free(array_of_lines);

  for (int i = 0; i < noOfInstructions; i++) {
//    free(array_of_instructions[i]);
  }
  free(array_of_instructions);

  fclose(outputFile);
  // Free memory and exit
  freeMap(symbolTable);
  return 0;
}

