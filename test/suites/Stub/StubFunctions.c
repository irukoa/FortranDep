#include "src/API/API.h"
#include "test/common/fff.h"

DEFINE_FFF_GLOBALS

FAKE_VOID_FUNC(FDEP_ApiError_Mock,
               const FDEP_ErrorCode,
               const char *const,
               const uint16_t)
