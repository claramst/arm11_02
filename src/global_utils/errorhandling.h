#ifndef ARM11_02_SRC_GLOBAL_UTILS_ERRORHANDLING_H_
#define ARM11_02_SRC_GLOBAL_UTILS_ERRORHANDLING_H_

#include <stdio.h>

/**
 * A file defining useful macros for error handling and
 * identifying empty strings.
 */

#define CHECK_PRED(pred, msg) \
 do { \
if ((pred)) { \
fprintf(stderr, \
"%s\n", (msg)); exit(EXIT_FAILURE);} \
} while (0)

#define EMPTY_STRING(s) (s[0] == '\0')

#endif //ARM11_02_SRC_GLOBAL_UTILS_ERRORHANDLING_H_
