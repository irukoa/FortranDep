#ifndef _API_H
#define _API_H

#include "tools/FunctionMacros.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

APIDEC(void *,
       ApiMalloc,
       const size_t Size);

APIDEC(int,
       ApiFprintf,
       FILE *const       Stream,
       const char *const Format,
       ...);

typedef enum _ErrorCode { ERROR_INPUT, ERROR_ALLOC } ErrorCode;

APIDEC(void,
       ApiError,
       const ErrorCode   ErrCode,
       const char *const StrFile,
       const uint16_t    Line);

#define API_ERROR(CODE) ApiError(CODE, __FILE__, __LINE__)

#endif
