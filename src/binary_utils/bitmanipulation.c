#include "bitmanipulation.h"

// Returns noOfBits bits starting from nth bit (in big endian)
int getBits(int bin, int noOfBits, int n) {
  int mask = (1 << noOfBits) - 1;
  return (bin & (mask << n)) >> n;
}

// Returns number after setting noOfBits bits from the nth bit to zero (in big endian)
int clearBits(int bin, int noOfBits, int n) {
  int mask = (1 << noOfBits) - 1;
  return bin & ~(mask << n);
}

// Sets noOfBits bits to one from the nth bit (in big endian)
int setBits(int bin, int noOfBits, int n) {
  int mask = (1 << noOfBits) - 1;
  return bin | (mask << n);
}


// Returns number with nth bit set to 1
int setBit(int bin, int n) {
  return (bin | (1 << n));
}

// Returns number with nth bit set to 0
int clearBit(int bin, int n) {
  return clearBits(bin, 1, n);
}

// Returns the nth bit of the bin
int getBit(int bin, int n) {
  return getBits(bin, 1, n);
}

// Returns the nth nibble of bin
int getNibble(int bin, int n) {
  return getBits(bin, 4, n * 4);
}

// Returns the nth byte of bin
int getByte(int bin, int n) {
  return getBits(bin, 8, n * 8);
}

// Performs arithmetic right shift by n on bin
int arithmeticRight(int bin, int n) {
  int sign = getBit(bin, 31);
  bin >>= n;
  if (sign) {
    bin = setBit(bin, 31);
  }
  return bin;
}

// Rotates bin right by n
int rotateRight(int bin, int n) {
  for (int i = 0; i < n; i++) {
    int lsb = getBit(bin, 0);
    bin = (bin >> 1) | (lsb << 31);
  }
  return bin;
}

// Extends the sign of bin with the nth bit as the sign bit
int signExtend(int bin, int n) {
  int signBit = getBit(bin, n);
  if (signBit) {
    bin = setBits(bin, 32 - n, n); 
  }
  return bin;
}
