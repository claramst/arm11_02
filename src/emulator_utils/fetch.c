#include "instruction.h"
#include "../../binary_utils/bitmanipulation.h"
#include <stdint.h>

INSTRUCTION fetch(REGISTER *pc, MACHINE_STATE state) {
  INSTRUCTION fetched = getWord(pc, state);
  *pc += 4;
  return fetched;
}

