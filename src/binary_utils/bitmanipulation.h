#include <stdint.h>

#ifndef BITMANIPULATION_H
#define BITMANIPULATION_H

typedef uint8_t BYTE;

uint32_t getBits(uint32_t bin, uint32_t noOfBits, uint32_t n);
uint32_t clearBits(uint32_t bin, uint32_t noOfBits, uint32_t n);
uint32_t setBit(uint32_t bin, uint32_t n);
uint32_t setBits(uint32_t bin, uint32_t noOfBits, uint32_t n);
uint32_t getBit(uint32_t bin, uint32_t n);
uint32_t getNibble(uint32_t bin, uint32_t n);
uint32_t getByte(uint32_t bin, uint32_t n);
uint32_t arithmeticRight(uint32_t bin, uint32_t n);
uint32_t rotateRight(uint32_t bin, uint32_t n);
uint32_t clearBit(uint32_t bin, uint32_t n);
int signExtend(uint32_t bin, uint32_t n);

#endif


