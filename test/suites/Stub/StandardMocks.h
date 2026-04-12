#ifndef _STANDARDMOCKS
#define _STANDARDMOCKS

#include "src/API/API.h"
#include "test/common/TSD.h"

void  ResetMallocFailCfg(void);
void  SetMallocFailCfg(const size_t FailAt);
void *ApiMalloc_CanFail(const size_t Sz);

int ApiFprintf_DoNothing(FILE *const       Stream,
                         const char *const Format,
                         ...);

void ApiError_JMP(const ErrorCode   ErrCode,
                  const char *const StrFile,
                  const uint16_t    Line);

#endif
