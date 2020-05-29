#include <stdint.h>
#include "instruction.h"
#include "bitmanipulation.h"

#ifndef FETCH_H
#define FETCH_H

INSTRUCTION fetch(REGISTER *pc, MACHINE_STATE state);

#endif

