#include <stdio.h>
#include <stdlib.h>
#include "emulator_utils/execute.h"
/**
 * emulate.c contains a main function which handles file reading,
 * machine state initialisation and our pipeline.
 */

/**
 * main ensures the given file can be read and
 * opened. We then allocate space in the heap for our machine state,
 * initialising the contents of our registers and memory to be 0.
 * We read the contents of the file into memory.
 * We then have a while loop handling the pipelining of our fetch,
 * decode and execute instruction cycle.
 * Finally, we free memory.
 *
 * @param argc Count of the arguments supplied
 * @param argv Array of strings passed as arguments
 * @return Integer representing exit success
 */
//int main(int argc, char **argv) {
//    if (argc != 2) {
//        fprintf(stderr, "No filename given");
//        return EXIT_FAILURE;
//    }
//    FILE *objCode = fopen(argv[1], "rb");
//
//    if (objCode == NULL) {
//        fprintf(stderr, "File could not be opened");
//        return EXIT_FAILURE;
//    }
//
//    MACHINE_STATE state;
//
//    state.registers = calloc(NUM_OF_REG, sizeof(REGISTER));
//    if (!state.registers) {
//        printf("Memory Failure\n");
//        exit(EXIT_FAILURE);
//    }
//    state.memory = calloc(MAX_ADDRESSES, sizeof(BYTE));
//    if (!state.memory) {
//        printf("Memory Failure \n");
//        exit(EXIT_FAILURE);
//    }
//
//    int i = 0;
//    while(!feof(objCode)) {
//        fread(&state.memory[i], sizeof(BYTE), 1, objCode);
//        i++;
//    }
//    if (ferror(objCode)) {
//        printf("An error has occurred whilst file reading");
//        exit(EXIT_FAILURE);
//    }
//    fclose(objCode);
//
//    REGISTER* pc = &state.registers[15];
//    INSTRUCTION fetched;
//    DECODED_INSTR decoded;
//    int toDecode = 0;
//    int toExecute = 0;
//    while (1) {
//        if (toExecute) {
//            if (decoded.type == HALT) {
//                break;
//            }
//            execute(decoded, state, &toDecode, &toExecute);
//        }
//        if (toDecode) {
//            decoded = decode(fetched, state);
//            toExecute = 1;
//        }
//        fetched = fetch(pc, state);
//        if (*pc >= 4) {
//            toDecode = 1;
//        }
//    }
//    printState(state);
//    /* Frees the memory allocated for the machine state */
//    free(state.registers);
//    free(state.memory);
//    return EXIT_SUCCESS;
//}

