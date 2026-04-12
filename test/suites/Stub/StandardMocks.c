#include "StandardMocks.h"
#include <stdbool.h>

typedef struct _MallocFakeFailCgf {
  size_t CallCount;
  size_t FailAt;
  bool   SetToFail;
} MallocFakeFailCgf;
static MallocFakeFailCgf MallocFakeCfg = {.CallCount = 0,
                                          .FailAt    = 0,
                                          .SetToFail = false};

void ResetMallocFailCfg(void) {
  MallocFakeCfg.CallCount = 0;
  MallocFakeCfg.FailAt    = 0;
  MallocFakeCfg.SetToFail = false;
}
void SetMallocFailCfg(const size_t FailAt) {
  MallocFakeCfg.CallCount = 0;
  MallocFakeCfg.FailAt    = FailAt;
  MallocFakeCfg.SetToFail = true;
}
void *ApiMalloc_CanFail(const size_t Sz) {
  MallocFakeCfg.CallCount++;
  if ((MallocFakeCfg.SetToFail) &&
      (MallocFakeCfg.CallCount == MallocFakeCfg.FailAt)) {
    ResetMallocFailCfg();
    return NULL;
  } else {
    if (Sz == 0) {
      API_ERROR(ERROR_ALLOC);
    }
    return (void *)malloc(Sz);
  }
}

int ApiFprintf_DoNothing(FILE *const       Stream,
                         const char *const Format,
                         ...) {
  (void)Stream;
  (void)Format;
  return 0;
}

void ApiError_JMP(const ErrorCode   ErrCode,
                  const char *const StrFile,
                  const uint16_t    Line) {
  (void)ErrCode;
  (void)StrFile;
  (void)Line;
  (void)longjmp(TSD_GlobJumpRef, 0xFFFF);
  return;
}
