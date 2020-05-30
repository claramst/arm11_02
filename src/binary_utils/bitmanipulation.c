#include "bitmanipulation.h"
#include <stdint.h>

// Returns noOfBits bits starting from nth bit (in big endian)
uint32_t getBits(uint32_t bin, uint32_t noOfBits, uint32_t n) {
    uint32_t mask = (1 << noOfBits) - 1;
    return (bin & (mask << n)) >> n;
}

// Returns number after setting noOfBits bits from the nth bit to zero (in big endian)
uint32_t clearBits(uint32_t bin, uint32_t noOfBits, uint32_t n) {
    uint32_t mask = (1 << noOfBits) - 1;
    return bin & ~(mask << n);
}

// Sets noOfBits bits to one from the nth bit (in big endian)
uint32_t setBits(uint32_t bin, uint32_t noOfBits, uint32_t n) {
    uint32_t mask = (1 << noOfBits) - 1;
    return bin | (mask << n);
}


// Returns number with nth bit set to 1
uint32_t setBit(uint32_t bin, uint32_t n) {
    return (bin | (1 << n));
}

// Returns number with nth bit set to 0
uint32_t clearBit(uint32_t bin, uint32_t n) {
    return clearBits(bin, 1, n);
}

// Returns the nth bit of the bin
uint32_t getBit(uint32_t bin, uint32_t n) {
    return getBits(bin, 1, n);
}

// Returns the nth nibble of bin
uint32_t getNibble(uint32_t bin, uint32_t n) {
    return getBits(bin, 4, n * 4);
}

// Returns the nth byte of bin
uint32_t getByte(uint32_t bin, uint32_t n) {
    return getBits(bin, 8, n * 8);
}

// Performs arithmetic right shift by n on bin
uint32_t arithmeticRight(uint32_t bin, uint32_t n) {
    uint32_t sign = getBit(bin, 31);
    bin >>= n;
    if (sign) {
        bin = setBit(bin, 31);
    }
    return bin;
}

// Rotates bin right by n
uint32_t rotateRight(uint32_t bin, uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        uint32_t lsb = getBit(bin, 0);
        bin = (bin >> 1) | (lsb << 31);
    }
    return bin;
}

// Extends the sign of bin with the nth bit as the sign bit
int signExtend(uint32_t bin, uint32_t n) {
    uint32_t signBit = getBit(bin, n);
    if (signBit) {
        bin = setBits(bin, 32 - n, n);
    }
    return bin;
}
