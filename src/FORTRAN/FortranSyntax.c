#include "src/FORTRAN/FortranSyntax.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static bool IsString    = false;
static char StringOwner = '\0';

void FDEP_FortranPreprocess(char *String) {
  char  *Position;
  size_t Len, i;

  // Replace string literals by blanks.
  for (i = 0; i < strlen(String); i++) {
    if ((String[i] == FDEP_FORTRAN_STRING1) ||
        (String[i] == FDEP_FORTRAN_STRING2)) {
      if ((!IsString) && (StringOwner == '\0')) {
        IsString    = true;
        StringOwner = String[i];
        String[i]   = ' ';
        continue;
      }
      if ((IsString) && (StringOwner == String[i])) {
        IsString    = false;
        StringOwner = '\0';
        String[i]   = ' ';
        continue;
      }
    }
    if (IsString) {
      String[i] = ' ';
      if (i == strlen(String) - 1) {
        String[i] = '&';
      }
    }
  }
  // Strip comments.
  if ((Position = strchr(String, FDEP_FORTRAN_COMMENT))) {
    *Position = '\0';
  }
  // Remove newline.
  if ((Position = strchr(String, '\n'))) {
    *Position = '\0';
  }
  // Lowercase
  Len = strlen(String);
  for (i = 0; i < Len; i++) {
    String[i] = (char)tolower((unsigned char)String[i]);
  }
}
