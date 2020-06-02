#include "tokenizer.h"
#include <string.h>

instrTokens_t tokenize(char *instrLine) {
  instrTokens_t instrTokens;
  char** strings;
  char* token = strtok(instrLine, " ");
  instrTokens.mnemonic = token; 
  for (int i = 0; token; i++) {
    strings[i] = token;
    token = strtok(NULL, " ");
  }

}

