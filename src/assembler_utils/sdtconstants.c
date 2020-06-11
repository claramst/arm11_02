#include "sdtconstants.h"
#include <stdlib.h>
#include <assert.h>

SDT_CONSTANTS *createSDTConstants(int noOfInstructions) {
  SDT_CONSTANTS *constStruct = (SDT_CONSTANTS *) malloc(sizeof(SDT_CONSTANTS));
  CHECK_PRED(!constStruct, "Error allocating SDT_CONSTANTS memory.");
  constStruct->constants = calloc(noOfInstructions, sizeof(uint32_t));
  CHECK_PRED(!constStruct->constants, "Error allocating SDT_CONSTANTS array memory.");
  constStruct->size = 0;
  constStruct->noOfInstructions = noOfInstructions;
  return constStruct;
}

void addToConstants(SDT_CONSTANTS *sdtConstants, uint32_t constant) {
  assert(sdtConstants);
  sdtConstants->constants[sdtConstants->size++] = constant;
}

void freeConstants(SDT_CONSTANTS *sdtConstants) {
  free(sdtConstants->constants);
  free(sdtConstants);
}


