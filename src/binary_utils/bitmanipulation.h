#include <stdint.h>

#ifndef BITMANIPULATION_H
#define BITMANIPULATION_H

typedef uint8_t BYTE;

int getBits(int bin, int noOfBits, int n);
int clearBits(int bin, int noOfBits, int n);
int setBit(int bin, int n);
int setBits(int bin, int noOfBits, int n);
int getBit(int bin, int n);
int getNibble(int bin, int n);
int getByte(int bin, int n);
int arithmeticRight(int bin, int n);
int rotateRight(int bin, int n);
int clearBit(int bin, int n);
int signExtend(int bin, int n);

#endif


