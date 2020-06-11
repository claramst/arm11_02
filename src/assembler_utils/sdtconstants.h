#ifndef ARM11_02_SRC_ASSEMBLER_UTILS_SDTCONSTANTS_H_
#define ARM11_02_SRC_ASSEMBLER_UTILS_SDTCONSTANTS_H_
#include <stdint.h>
#include "../global_utils/errorhandling.h"

typedef struct constants {
  uint32_t *constants;
  int size;
  int noOfInstructions;
} SDT_CONSTANTS;

SDT_CONSTANTS *createSDTConstants(int noOfInstructions);
void addToConstants(SDT_CONSTANTS *sdtConstants, uint32_t constant);
void freeConstants(SDT_CONSTANTS *sdtConstants);

#endif //ARM11_02_SRC_ASSEMBLER_UTILS_SDTCONSTANTS_H_
