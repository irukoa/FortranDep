#include "API.h"
#include <errno.h>
#include <stdarg.h>
#include <string.h>

APIDEF(void *,
       ApiMalloc,
       const size_t Sz) {
  if (Sz == 0) {
    API_ERROR(ERROR_ALLOC);
  }
  return (void *)malloc(Sz);
}

APIDEF(int,
       ApiFprintf,
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
typedef struct _ErrorDict {
  const ErrorCode   ErrCode;
  const char *const ErrString;
} ErrorDict;
static const ErrorDict ErrorList[] = {
#include "APIConfig/ErrorList.cfg"
};
static const size_t NumErrors = sizeof(ErrorList) / sizeof(ErrorList[0]);

APIDEF(void,
       ApiError,
       const ErrorCode   ErrCode,
       const char *const StrFile,
       const uint16_t    Line) {
#ifndef NDEBUG
  size_t i;
  for (i = 0; i < NumErrors; i++) {
    if (ErrorList[i].ErrCode == ErrCode) {
      fprintf(stderr, "%s:%u: ERROR: 0x%08X: ", StrFile, (uint32_t)Line,
              (uint32_t)ErrCode);
      fprintf(stderr, "%s.\n", ErrorList[i].ErrString);
    }
  }
#else
  (void)ErrCode;
  (void)StrFile;
  (void)Line;
#endif
  fprintf(stderr, "Error: %d: %s\n", errno, strerror(errno));
  exit(EXIT_FAILURE);
}
