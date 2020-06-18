#include <stdint.h>
/**
 * Header file for bitmanipulation.c.
 */

#ifndef BITMANIPULATION_H
#define BITMANIPULATION_H

typedef uint8_t BYTE;

uint32_t get_bits(uint32_t bin, uint32_t noOfBits, uint32_t n);
uint32_t clear_bits(uint32_t bin, uint32_t noOfBits, uint32_t n);
uint32_t set_bit(uint32_t bin, uint32_t n);
uint32_t set_bits(uint32_t bin, uint32_t noOfBits, uint32_t n);
uint32_t get_bit(uint32_t bin, uint32_t n);
uint32_t get_nibble(uint32_t bin, uint32_t n);
uint32_t get_byte(uint32_t bin, uint32_t n);
uint32_t arithmetic_right(uint32_t bin, uint32_t n);
uint32_t rotate_right(uint32_t bin, uint32_t n);
uint32_t rotate_left(uint32_t bin, uint32_t n);
uint32_t clear_bit(uint32_t bin, uint32_t n);
int sign_extend(uint32_t bin, uint32_t n);

#endif


