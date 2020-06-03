#include "bitmanipulation.h"
/**
 * Various functions for the manipulation of 32-bit binary ints.
 * The functions do not modify the original numbers given,
 * they instead return fresh copies with the desired modifications.
 */

/**
 * Retrieves a given number of bits from a binary int.
 * @param bin Unsigned 32-bit binary int to get bits from.
 * @param noOfBits The number of bits to retrieve.
 * @param n Index of first bit to retrieve.
 * @return The desired selection of bits from bin.
 */
uint32_t getBits(uint32_t bin, uint32_t noOfBits, uint32_t n) {
    uint32_t mask = (1 << noOfBits) - 1;
    return (bin & (mask << n)) >> n;
}

/**
 * Gives a copy of a given binary int with certain bits cleared as designated.
 * @param bin Unsigned 32-bit binary int.
 * @param noOfBits The number of bits to clear.
 * @param n Index of first bit to clear.
 * @return The supplied binary int with the designated bits set to 0.
 */
uint32_t clearBits(uint32_t bin, uint32_t noOfBits, uint32_t n) {
    uint32_t mask = (1 << noOfBits) - 1;
    return bin & ~(mask << n);
}

/**
 * Analogous to clearBits, except sets them instead.
 */
uint32_t setBits(uint32_t bin, uint32_t noOfBits, uint32_t n) {
    uint32_t mask = (1 << noOfBits) - 1;
    return bin | (mask << n);
}


/**
 * Gives a copy of a given binary int with the desired bit set.
 * @param bin Unsigned 32-bit binary int.
 * @param n the index of the bit to clear.
 * @return The given binary number with its nth bit set to 1.
 */
uint32_t setBit(uint32_t bin, uint32_t n) {
    return (bin | (1 << n));
}

/**
 * Analogous to setBit, but clears a desired bit instead.
 */
uint32_t clearBit(uint32_t bin, uint32_t n) {
    return clearBits(bin, 1, n);
}

/**
 * Retrieves the nth bit of bin.
 */
uint32_t getBit(uint32_t bin, uint32_t n) {
    return getBits(bin, 1, n);
}

/**
 * Returns the nth nibble of bin.
 */
uint32_t getNibble(uint32_t bin, uint32_t n) {
    return getBits(bin, 4, n * 4);
}

/**
 * Returns the nth byte of bin.
 */
uint32_t getByte(uint32_t bin, uint32_t n) {
    return getBits(bin, 8, n * 8);
}

/**
 * Performs an arithmetic right shift by n on bin.
 */
uint32_t arithmeticRight(uint32_t bin, uint32_t n) {
    uint32_t sign = getBit(bin, 31);
    bin >>= n;
    if (sign) {
        bin = setBit(bin, 31);
    }
    return bin;
}

/**
 * Rotates bin right by n.
 */
uint32_t rotateRight(uint32_t bin, uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        uint32_t lsb = getBit(bin, 0);
        bin = (bin >> 1) | (lsb << 31);
    }
    return bin;
}

/**
 * Extends the sign of bin with the nth bit as the sign bit.
 */
int signExtend(uint32_t bin, uint32_t n) {
    uint32_t signBit = getBit(bin, n);
    if (signBit) {
        bin = setBits(bin, 32 - n, n);
    }
    return bin;
}
