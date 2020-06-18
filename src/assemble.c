#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler_utils/encode.h"

/** assemble.c contains a main function and several helper methods to do with reading
  * the file contents into an array of instructions. Also contains functions to
  * build up our symbol table (a hashmap structure). In our first pass we
  * read the labels and their corresponding addresses into our symbol table.
  * Then we also load mnemonics and their corresponding opcode value as pairs into the
  * symbol table.
  */


//PRE: fileName is not null
/**
 * Returns the number of non-empty lines in the provided file, rewinding the file pointer after counting.
 */
int count_lines(FILE *fptr) {
  int lines = 0;
  char c, last = '\n';
  while (!feof(fptr)) {
    c = fgetc(fptr);
    if (c == '\n' && last != '\n')
      lines++;
    last = c;
  }
  rewind(fptr);

  return lines;
}

/**
 * Adds all valid opcodes into the given symbol table.
 */
void add_opcodes(Map *symbolTable) {
  char *mnemonics[] =
	  {"and", "eor", "sub", "rsb", "add", "tst", "teq", "cmp", "orr", "mov", "ldr", "str", "beq", "bne", "bge", "blt",
		  "bgt", "ble", "b", "lsl", "andeq", "mul", "mla"};
  int mnemonicValues[] =
	  {AND, EOR, SUB, RSB, ADD, TST, TEQ, CMP, ORR, MOV, LDR, STR, BEQ, BNE, BGE, BLT, BGT, BLE, B, LSL, ANDEQ, MUL,
		  MLA};
  add_nodes(symbolTable, mnemonics, mnemonicValues, 23);
}

/**
 * Reads a given number of lines from the given file into a 2D array.
 *
 * @param array_of_lines The array to be read into.
 * @param noOfLines The number of lines to read in.
 * @param sourceFile File to read lines from.
 */
void read_lines(char **array_of_lines, int noOfLines, FILE *sourceFile) {
  for (int i = 0; i < noOfLines; i++) {
    array_of_lines[i] = (char *) calloc(MAX_LINE_LENGTH, sizeof(char));
    CHECK_PRED(!array_of_lines[i], "Error allocating memory for entry in array of lines");

    fgets(array_of_lines[i], MAX_LINE_LENGTH, sourceFile);
    array_of_lines[i][strlen(array_of_lines[i]) - 1] = '\0';

    if (EMPTY_STRING(array_of_lines[i])) {
      i--;
      continue;
    }
  }
}

/**
 * first_pass reads from our array of lines, which contains all lines from the
 * file. If the line is a label, we read the label and its address into our
 * symbol table. If the line is not a label, i.e it is an instruction,
 * we allocate memory in our array of instructions for it and store it there.
 * Hence array of instructions becomes a subset of array of lines, but containing
 * only instructions and no labels. We then use this array in our
 * second pass.
 *
 * @param symbolTable: Our symbol table, which we read labels and their addresses into
 * @param array_of_lines: An array containing every line of the file in order
 * @param array_of_instructions: A
 */
int first_pass(Map *symbolTable, char *array_of_lines[], char *array_of_instructions[], int noOfLines) {
  int j = 0;
  for (int i = 0; i < noOfLines; i++) {
    //Checks if the line at array_of_lines[i] contains ':'
    char *str = strchr(array_of_lines[i], ':');
    if (str) {
      char *temp = array_of_lines[i], *label = strtok(temp, ":");
      add_node(symbolTable, label, sizeof(ADDRESS) * address);
    } else {
      array_of_instructions[j] = array_of_lines[i];
      j++;
    }
  }

  return j;
}

/**
 * Writes one 32-bit big-endian word to the given file in little-endian.
 */
void write_to_file(FILE *file, WORD word) {
  for (int j = 0; j < sizeof(ADDRESS); j++) {
    BYTE byte = getByte(word, j);
    fwrite(&byte, sizeof(BYTE), 1, file);
  }
}

int main(int argc, char **argv) {
  CHECK_PRED(argc != 3, "Invalid number of arguments.");

  char *sourceFileName = argv[1];
  char *outputFileName = argv[2];

  FILE *sourceFile = fopen(sourceFileName, "r");
  CHECK_PRED(!sourceFile, "File could not be opened");

  //Note noOfLines includes label lines in the count.
  int noOfLines = count_lines(sourceFile);

  char **array_of_lines = (char **) calloc(noOfLines, sizeof(char *));
  CHECK_PRED(!array_of_lines, "Error allocating memory for array of lines");

  read_lines(array_of_lines, noOfLines, sourceFile);
  CHECK_PRED(ferror(sourceFile), "An error has occurred whilst file reading");

  fclose(sourceFile);

  // First pass:
  // Create symbol table, alloc memory
  // Associate labels with memory addresses

  char **array_of_instructions = (char **) calloc(noOfLines, sizeof(char *));
  CHECK_PRED(!array_of_instructions, "Error allocating memory for array of instructions");

  Map *symbolTable = create_map();
  int noOfInstructions = first_pass(symbolTable, array_of_lines, array_of_instructions, noOfLines);
  // Add opcodes to symbol table
  add_opcodes(symbolTable);

  // Second pass:
  // In the second pass the assembler reads in the opcode mnemonic and
  // operand field(s) for each instruction and generates the corresponding binary encoding of that instruction.
  // As part of this process it replaces label references in operand fields with their corresponding addresses,
  // as defined in the symbol table computed during the first pass.
  FILE *outputFile = fopen(outputFileName, "wb");
  CHECK_PRED(!outputFile, "File already exists");

  SDT_CONSTANTS *sdtConstants = create_sdt_constants(noOfInstructions);
  // Assemble into binary, use filewriter
  for (int i = 0; i < noOfInstructions; i++) {
    INSTR_TOKENS *tokens = tokenize(array_of_instructions[i], i, symbolTable);
    INSTRUCTION instr = encode_instruction(tokens, symbolTable, sdtConstants);
    free_tokens(tokens);
    write_to_file(outputFile, instr);
  }

  //3rd loop to write in load constants
  for (int i = 0; i < sdtConstants->size; i++) 
	  write_to_file(outputFile, sdtConstants->constants[i]);

  fclose(outputFile);

  free_constants(sdtConstants);
  for (int i = 0; i < noOfLines; i++)
	  free(array_of_lines[i]);
  
  free(array_of_lines);
  free(array_of_instructions);

  // Free memory and exit
  freeMap(symbolTable);
  return 0;
}

