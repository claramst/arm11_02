#include "bitmanipulation.h"

#ifndef MEMORY_H
#define MEMORY_H

typedef uint32_t WORD;
typedef uint32_t REGISTER;
typedef uint16_t ADDRESS;

typedef struct machineState {
  REGISTER* registers;
  BYTE* memory;
} MACHINE_STATE;


WORD getWord(ADDRESS address, MACHINE_STATE state);
void storeWord(WORD word, ADDRESS address, MACHINE_STATE state);

#endif