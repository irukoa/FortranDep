#include "src/API/API.h"
#include "test/common/fff.h"

DEFINE_FFF_GLOBALS

FAKE_VOID_FUNC(ApiError_Mock,
               const ErrorCode,
               const char *const,
               const uint16_t)
