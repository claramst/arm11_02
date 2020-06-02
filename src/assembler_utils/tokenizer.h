#include "../emulator_utils/decode.h"


typedef struct instrTokens {
   char *cond;
   char *mnemonic;
   char *rn;
   char *rd;
   char *rm;
   char *operand;
   

} instrTokens_t;

