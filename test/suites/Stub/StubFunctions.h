#ifndef _STUBFUNCTIONS
#define _STUBFUNCTIONS

#include "src/API/API.h"
#include "test/common/fff.h"

DECLARE_FAKE_VOID_FUNC(FDEP_ApiError_Mock,
                       const FDEP_ErrorCode,
                       const char *const,
                       const uint16_t)

#endif
