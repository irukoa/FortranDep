#ifndef _STANDARDMOCKS
#define _STANDARDMOCKS

#include "src/API/API.h"
#include "test/common/TSD.h"

void  FDEP_ResetMallocFailCfg(void);
void  FDEP_SetMallocFailCfg(const size_t FailAt);
void *FDEP_ApiMalloc_CanFail(const size_t Sz);

void  FDEP_ResetReallocFailCfg(void);
void  FDEP_SetReallocFailCfg(const size_t FailAt);
void *FDEP_ApiRealloc_CanFail(void        *Ptr,
                              const size_t Sz);

void FDEP_ResetFerrorFailCfg(void);
void FDEP_SetFerrorFailCfg(const size_t FailAt);
int  FDEP_ApiFerror_CanFail(FILE *stream);

int FDEP_ApiFprintf_DoNothing(FILE *const       Stream,
                              const char *const Format,
                              ...);

void FDEP_ApiError_JMP(const FDEP_ErrorCode ErrCode,
                       const char *const    StrFile,
                       const uint16_t       Line);

#endif
