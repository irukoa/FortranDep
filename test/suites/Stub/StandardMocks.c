#include "StandardMocks.h"
#include <stdbool.h>

// Malloc.
typedef struct _FDEP_MallocFakeFailCgf {
  size_t CallCount;
  size_t FailAt;
  bool   SetToFail;
} FDEP_MallocFakeFailCgf;
static FDEP_MallocFakeFailCgf FDEP_MallocFakeCfg = {.CallCount = 0,
                                                    .FailAt    = 0,
                                                    .SetToFail = false};

void FDEP_ResetMallocFailCfg(void) {
  FDEP_MallocFakeCfg.CallCount = 0;
  FDEP_MallocFakeCfg.FailAt    = 0;
  FDEP_MallocFakeCfg.SetToFail = false;
}
void FDEP_SetMallocFailCfg(const size_t FailAt) {
  FDEP_MallocFakeCfg.CallCount = 0;
  FDEP_MallocFakeCfg.FailAt    = FailAt;
  FDEP_MallocFakeCfg.SetToFail = true;
}
void *FDEP_ApiMalloc_CanFail(const size_t Sz) {
  FDEP_MallocFakeCfg.CallCount++;
  if ((FDEP_MallocFakeCfg.SetToFail) &&
      (FDEP_MallocFakeCfg.CallCount == FDEP_MallocFakeCfg.FailAt)) {
    FDEP_ResetMallocFailCfg();
    return NULL;
  } else {
    if (Sz == 0) {
      FDEP_API_ERROR(ERROR_ALLOC);
    }
    return (void *)malloc(Sz);
  }
}

// Realloc.
typedef struct _FDEP_ReallocFakeFailCgf {
  size_t CallCount;
  size_t FailAt;
  bool   SetToFail;
} FDEP_ReallocFakeFailCgf;
static FDEP_ReallocFakeFailCgf FDEP_ReallocFakeCfg = {.CallCount = 0,
                                                      .FailAt    = 0,
                                                      .SetToFail = false};

void FDEP_ResetReallocFailCfg(void) {
  FDEP_ReallocFakeCfg.CallCount = 0;
  FDEP_ReallocFakeCfg.FailAt    = 0;
  FDEP_ReallocFakeCfg.SetToFail = false;
}
void FDEP_SetReallocFailCfg(const size_t FailAt) {
  FDEP_ReallocFakeCfg.CallCount = 0;
  FDEP_ReallocFakeCfg.FailAt    = FailAt;
  FDEP_ReallocFakeCfg.SetToFail = true;
}
void *FDEP_ApiRealloc_CanFail(void        *Ptr,
                              const size_t Sz) {
  FDEP_ReallocFakeCfg.CallCount++;
  if ((FDEP_ReallocFakeCfg.SetToFail) &&
      (FDEP_ReallocFakeCfg.CallCount == FDEP_ReallocFakeCfg.FailAt)) {
    FDEP_ResetReallocFailCfg();
    return NULL;
  } else {
    if (Sz == 0) {
      FDEP_API_ERROR(ERROR_ALLOC);
    }
    return (void *)realloc(Ptr, Sz);
  }
}

// Ferror.
typedef struct _FDEP_FerrorFakeFailCgf {
  size_t CallCount;
  size_t FailAt;
  bool   SetToFail;
} FDEP_FerrorFakeFailCgf;
static FDEP_FerrorFakeFailCgf FDEP_FerrorFakeCfg = {.CallCount = 0,
                                                    .FailAt    = 0,
                                                    .SetToFail = false};

void FDEP_ResetFerrorFailCfg(void) {
  FDEP_FerrorFakeCfg.CallCount = 0;
  FDEP_FerrorFakeCfg.FailAt    = 0;
  FDEP_FerrorFakeCfg.SetToFail = false;
}
void FDEP_SetFerrorFailCfg(const size_t FailAt) {
  FDEP_FerrorFakeCfg.CallCount = 0;
  FDEP_FerrorFakeCfg.FailAt    = FailAt;
  FDEP_FerrorFakeCfg.SetToFail = true;
}
int FDEP_ApiFerror_CanFail(FILE *Stream) {
  FDEP_FerrorFakeCfg.CallCount++;
  if ((FDEP_FerrorFakeCfg.SetToFail) &&
      (FDEP_FerrorFakeCfg.CallCount == FDEP_FerrorFakeCfg.FailAt)) {
    FDEP_ResetFerrorFailCfg();
    return -1;
  } else {
    return ferror(Stream);
  }
}

int FDEP_ApiFprintf_DoNothing(FILE *const       Stream,
                              const char *const Format,
                              ...) {
  (void)Stream;
  (void)Format;
  return 0;
}

void FDEP_ApiError_JMP(const FDEP_ErrorCode ErrCode,
                       const char *const    StrFile,
                       const uint16_t       Line) {
  (void)ErrCode;
  (void)StrFile;
  (void)Line;
  (void)longjmp(TSD_GlobJumpRef, 0xFFFF);
  return;
}
