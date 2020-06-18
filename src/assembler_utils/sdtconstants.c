#include "sdtconstants.h"
#include <stdlib.h>
#include <assert.h>

/**
 * sdtconstants.c contains functions for creating an array
 * to store and track the numebr of single data transfer instruction constants.
 */

/**
 * Creates an empty SDT_CONSTANTS struct. For the array constants in
 * our empty SDT_CONSTANTS structure we allocate memory for
 * noOfInstructions many 32 bit integers, since noOfInstructions is
 * the maximum number of load constants we would need to store.
 * This is because we know that the worst case would be where every
 * instruction passed in is a load instruction with a constant
 * bigger than 0xff. Clearly, in this case the maximum number of
 * load constants to store would be the number of instructions in
 * the program.
 * We check that we have been able to allocate memory using
 * our macro CHECK_PRED (defined in errorhandling.h).
 * We set the size of the constants array (i.e the number of load
 * constants currently stored) to be 0, since the array is initially
 * empty. We also store the noOfInstructions in our SDT_CONSTANTS
 * structure and so initialise this field of the structure also.
 *
 * @param noOfInstructions: The number of instructions in the
 * program we are executing
 * @return A pointer to the initialised SDT_CONSTANTS struct
 */
SDT_CONSTANTS *create_sdt_constants(int noOfInstructions) {
  SDT_CONSTANTS *constStruct = (SDT_CONSTANTS *) malloc(sizeof(SDT_CONSTANTS));
  CHECK_PRED(!constStruct, "Error allocating SDT_CONSTANTS memory.");
  constStruct->constants = calloc(noOfInstructions, sizeof(uint32_t));
  CHECK_PRED(!constStruct->constants, "Error allocating SDT_CONSTANTS array memory.");
  constStruct->size = 0;
  constStruct->noOfInstructions = noOfInstructions;
  return constStruct;
}

/**
 * Adds a constant to the SDT_CONSTANTS struct.
 */
void add_to_constants(SDT_CONSTANTS *sdtConstants, uint32_t constant) {
  assert(sdtConstants);
  sdtConstants->constants[sdtConstants->size++] = constant;
}

/**
 * For a given SDT_CONSTANTS struct, frees the struct and its elements.
 */
void free_constants(SDT_CONSTANTS *sdtConstants) {
  free(sdtConstants->constants);
  free(sdtConstants);
}


