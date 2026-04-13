#include "src/FORTRAN/FortranSyntax.h"
#include <ctype.h>
#include <stddef.h>
#include <string.h>

void FDEP_FortranPreprocess(char *String) {
  char  *CommentPosition;
  size_t Len, i;

  if ((CommentPosition = strchr(String, FDEP_FORTRAN_COMMENT))) {
    *CommentPosition = '\0';
  }
  Len = strlen(String);
  for (i = 0; i < Len; i++) {
    String[i] = (char)tolower((unsigned char)String[i]);
  }
}
