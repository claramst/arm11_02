#ifndef ARM11_02_SRC_ASSEMBLER_UTILS_SDTCONSTANTS_H_
#define ARM11_02_SRC_ASSEMBLER_UTILS_SDTCONSTANTS_H_

#include <stdint.h>
#include "../global_utils/errorhandling.h"

/**
 * Header file for sdtConstants.
 * contains the struct definiton for SDT_CONSTANTS.
 * contains function declarations for functions manupulating SDT_CONSTANTS.
 */

/**
 * A structure storing an array of load constants, and the current
 * size of this array. These load constants are stored so that
 * they can later be printed to the end of the file.
 * We also store the number of instructions in the program we
 * are assembling, so that we know the offset for where the load
 * constants should be printed in our output file.
 */
typedef struct constants {
  uint32_t *constants;
  int size;
  int noOfInstructions;
} SDT_CONSTANTS;

SDT_CONSTANTS *create_sdt_constants(int noOfInstructions);
void add_to_constants(SDT_CONSTANTS *sdtConstants, uint32_t constant);
void free_constants(SDT_CONSTANTS *sdtConstants);

#endif //ARM11_02_SRC_ASSEMBLER_UTILS_SDTCONSTANTS_H_
