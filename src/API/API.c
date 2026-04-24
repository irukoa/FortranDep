#include "src/API/API.h"
#include <errno.h>
#include <stdarg.h>

APIDEF(void *,
       FDEP_ApiMalloc,
       const size_t Sz) {
  if (Sz == 0) {
    FDEP_API_ERROR(ERROR_ALLOC);
  }
  return (void *)malloc(Sz);
}

APIDEF(void *,
       FDEP_ApiRealloc,
       void        *Ptr,
       const size_t Sz) {
  if (Sz == 0) {
    FDEP_API_ERROR(ERROR_ALLOC);
  }
  return (void *)realloc(Ptr, Sz);
}

APIDEF(int,
       FDEP_ApiFerror,
       FILE *Stream) {
  return ferror(Stream);
}

APIDEF(int,
       FDEP_ApiFprintf,
       FILE *const       Stream,
       const char *const Format,
       ...) {
  va_list args;
  int     result;

  va_start(args, Format);
  result = vfprintf(Stream, Format, args);
  va_end(args);
  return result;
}

// Error list.
typedef struct _FDEP_ErrorDict {
  const FDEP_ErrorCode ErrCode;
  const char *const    ErrString;
} FDEP_ErrorDict;
static const FDEP_ErrorDict FDEP_ErrorList[] = {
#include "APIConfig/ErrorList.cfg"
};
static const size_t FDEP_NumErrors =
    sizeof(FDEP_ErrorList) / sizeof(FDEP_ErrorList[0]);

APIDEF(void,
       FDEP_ApiError,
       const FDEP_ErrorCode ErrCode,
       const char *const    StrFile,
       const uint16_t       Line) {
#ifndef NDEBUG
  size_t i;
  for (i = 0; i < FDEP_NumErrors; i++) {
    if (FDEP_ErrorList[i].ErrCode == ErrCode) {
      fprintf(stderr, "%s:%u: ERROR: 0x%08X: ", StrFile, (uint32_t)Line,
              (uint32_t)ErrCode);
      fprintf(stderr, "%s.\n", FDEP_ErrorList[i].ErrString);
    }
  }
#else
  (void)ErrCode;
  (void)StrFile;
  (void)Line;
#endif
  if (errno != 0)
    fprintf(stderr, "Error: %d: %s\n", errno, strerror(errno));
  exit(EXIT_FAILURE);
}
