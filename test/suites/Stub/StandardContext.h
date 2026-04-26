/* Include in test files. Defines the "standard" context FDEP_Standard. Defines
 * a fake stream.*/
#include "src/API/API.h"
#include "test/common/TSD.h"
#include "test/suites/Stub/StandardMocks.h"
#include "test/suites/Stub/StubFunctions.h"

typedef struct _FDEP_StandardContextData {
  FILE *FakeStream;
} FDEP_StandardContextData;
CONTEXT_FN(FDEP_StandardSetup) {
  FDEP_StandardContextData *Data = (FDEP_StandardContextData *)ContextData;
  Data->FakeStream               = NULL;
  // Fake FDEP_ApiMalloc.
  FDEP_ApiMalloc = FDEP_ApiMalloc_CanFail;
  // Fake FDEP_ApiRealloc.
  FDEP_ApiRealloc = FDEP_ApiRealloc_CanFail;
  // Fake FDEP_ApiFerror.
  FDEP_ApiFerror = FDEP_ApiFerror_CanFail;
  // Fake FDEP_ApiFprintf.
  FDEP_ApiFprintf = FDEP_ApiFprintf_DoNothing;
  // Fake FDEP_ApiError.
  RESET_FAKE(FDEP_ApiError_Mock);
  FDEP_ApiError_Mock_fake.custom_fake = FDEP_ApiError_JMP;
  FDEP_ApiError                       = FDEP_ApiError_Mock;
}
CONTEXT_FN(FDEP_StandardTeardown) {
  FDEP_StandardContextData *Data = (FDEP_StandardContextData *)ContextData;
  if (Data->FakeStream) {
    fclose(Data->FakeStream);
    Data->FakeStream = NULL;
  }
  FDEP_ResetMallocFailCfg();
  FDEP_ResetReallocFailCfg();
  FDEP_ResetFerrorFailCfg();
  // Rewind FDEP_ApiMalloc.
  RESET_FN(FDEP_ApiMalloc);
  // Rewind FDEP_ApiRealloc.
  RESET_FN(FDEP_ApiRealloc);
  // Rewind FDEP_ApiFerror.
  RESET_FN(FDEP_ApiFerror);
  // Rewind FDEP_ApiFprintf.
  RESET_FN(FDEP_ApiFprintf);
  // Rewind FDEP_ApiError.
  RESET_FN(FDEP_ApiError);
}
CONTEXT(FDEP_Standard,
        FDEP_StandardSetup,
        FDEP_StandardTeardown,
        sizeof(FDEP_StandardContextData));
