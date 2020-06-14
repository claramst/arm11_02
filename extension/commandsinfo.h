#include <stdio.h>
#include <stdlib.h>
#include "commands.h"

#ifndef COMMANDSINFO_H
#define COMMANDSINFO_H

void mnemonic(void);
void dataProcessingInfo(void);
void branchInfo(void);
void multiplyInfo(void);
void singleDataTransferInfo(void);
void dataProcessingBinary(void);
void branchBinary(void);
void multiplyBinary(void);
void singleDataTransferBinary(void);
void instruction(Editor *state);
void info(Editor *state);
#endif
