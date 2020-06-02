#include <stdio.h>
#include <stdlib.h>
#include "./assembler_utils/filewriter.h"
#include "./assembler_utils/hashmap.h"
#include "./assembler_utils/tokenizer.h"
#include <strings.h>
#include "./assembler_utils/hashmap.h"

#define MAX_LINE_LENGTH 511

//PRE: fileName is not null
int countLines(FILE *file) {
  int lines = 0;
  char *str;
  while (!feof(file)) {
    fgets(str, MAX_LINE_LENGTH, file);
    lines++;
  }
  return lines;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Invalid argument");
    return EXIT_FAILURE;
  }

  char* sourceFileName = argv[1];
  char* outputFileName = argv[2];

  FILE *sourceFile = fopen(sourceFileName, "r");
  if(sourceFile== NULL) {
    fprintf(stderr, "File could not be opened");
    return EXIT_FAILURE;
  }
   
  char **array_of_lines = (char **) calloc(countLines(sourceFile), MAX_LINE_LENGTH * sizeof(char));
  int arraySize = countLines(sourceFile);

  
  for (int i = 0; !feof(sourceFile); i++) {
    fscanf(sourceFile, "%[^\n]%*c", array_of_lines[i]);
    
   // fgets(&array_of_lines[i], MAX_LINE_LENGTH, sourceFile);
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
  
  Map *symbolTable = createMap();
  for (int i = 0; i < arraySize; i++) {
    //Checks if the line at array_of_lines[i] contains ':'
    char *str = strchr(array_of_lines[i], ':');
    if (str) {
      char *temp = array_of_lines[i];
      char *label = strtok(temp, ":");
      int address = 4 * i;
      Node *node = createNode(label, address);
      addNode(symbolTable, node);
    }
  }
  
  // Second pass:
  // In the second pass the assembler reads in the opcode mnemonic and
  // operand field(s) for each instruction and generates the corresponding binary encoding of that instruction.
  // As part of this process it replaces label references in operand fields with their corresponding addresses,
  // as defined in the symbol table computed during the first pass.



  for (int i = 0; i < arraySize; i++) {
    tokenize(array_of_lines[i]);
  }

  // Assemble into binary, use filewriter

  // Free memory and exit

}


