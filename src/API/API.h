#ifndef _API_H
#define _API_H

#include "tools/FunctionMacros.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

APIDEC(void *,
       FDEP_ApiMalloc,
       const size_t Size);

APIDEC(void *,
       FDEP_ApiRealloc,
       void        *Ptr,
       const size_t Sz);

APIDEC(int,
       FDEP_ApiFerror,
       FILE *Stream);

APIDEC(int,
       FDEP_ApiFprintf,
       FILE *const       Stream,
       const char *const Format,
       ...);

typedef enum _FDEP_ErrorCode {
  NO_ERROR,
  ERROR_INPUT,
  ERROR_STREAM,
  ERROR_ALLOC
} FDEP_ErrorCode;

APIDEC(void,
       FDEP_ApiError,
       const FDEP_ErrorCode ErrCode,
       const char *const    StrFile,
       const uint16_t       Line);

#define FDEP_API_ERROR(CODE) FDEP_ApiError(CODE, __FILE__, __LINE__)

#endif
