#ifndef _STUBFUNCTIONS
#define _STUBFUNCTIONS

#include "src/API/API.h"
#include "test/common/fff.h"

DECLARE_FAKE_VOID_FUNC(ApiError_Mock,
                       const ErrorCode,
                       const char *const,
                       const uint16_t)

#endif
