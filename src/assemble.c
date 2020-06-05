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
    for (int j = 0; j < 4; j++) {
        BYTE byte = getByte(word, j);
        fwrite(&byte, sizeof(BYTE), 1, file);
    }
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

    char **array_of_lines = (char **) calloc(countLines(sourceFile), MAX_LINE_LENGTH * sizeof(char));
    int noOfInstructions = countLines(sourceFile);


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
    for (int i = 0; i < noOfInstructions; i++) {
        //Checks if the line at array_of_lines[i] contains ':'
        char *str = strchr(array_of_lines[i], ':');
        if (str) {
            char *temp = array_of_lines[i];
            char *label = strtok(temp, ":");
            int address = 4 * i;
            addNode(symbolTable, label, address);
        }
    }
    // Add opcodes and values to symbol table
    char *mnemonics[] = {"and", "eor", "sub", "rsb", "add","tst", "teq", "cmp", "orr", "mov", "tst", "teq", "cmp", "mul", "mla", "ldr", "str", "beq", "bne", "bge", "blt", "bgt", "ble", "b", "lsl", "andeq"};
    int mnemonicValues[] = {AND, EOR, SUB, ADD, TST, TEQ, CMP, ORR, MOV, MUL, MLA, LDR, STR, BEQ, BNE, BGE, BLT, BGT, BLE,  B, LSL, ANDEQ};
    addNodes(symbolTable, mnemonics, mnemonicValues, 24);



    // Second pass:
    // In the second pass the assembler reads in the opcode mnemonic and
    // operand field(s) for each instruction and generates the corresponding binary encoding of that instruction.
    // As part of this process it replaces label references in operand fields with their corresponding addresses,
    // as defined in the symbol table computed during the first pass.

    FILE *outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        fprintf(stderr, "File already exists");
        return EXIT_FAILURE;
    }

    // Assemble into binary, use filewriter
    for (int i = 0; i < noOfInstructions; i++) {
        INSTR_TOKENS *tokens = tokenize(array_of_lines[i], i, symbolTable);
        INSTRUCTION instr = encodeInstruction(tokens, symbolTable);
        free(tokens);
        writeToFile(outputFile, instr);
    }

    fclose(outputFile);

    // Free memory and exit
    freeMap(symbolTable);
    return 0;
}

