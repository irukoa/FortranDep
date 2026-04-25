#ifndef _FORTRAN_SYNTAX_H
#define _FORTRAN_SYNTAX_H

#include "src/API/API.h"

#define FDEP_FORTRAN_DELIMITERS   " \t\n(),:"
#define FDEP_FORTRAN_CONTINUATION '&'
#define FDEP_FORTRAN_SEPARATOR    ';'
#define FDEP_FORTRAN_COMMENT      '!'
#define FDEP_FORTRAN_STRING1      '\''
#define FDEP_FORTRAN_STRING2      '"'

#define FDEP_OBJECT_NAME      "#OBJECT#"
#define FDEP_SOURCE_NAME      "#SOURCEFILE#"
#define FDEP_MODULE_SUFFIX    "mod"
#define FDEP_SUBMODULE_SUFFIX "smod"

/* Preprocesses String according to Fortran format:
 - strips everything past the '!' token, and
 - makes everything lowercase.*/
void FDEP_FortranPreprocess(char *String);

#endif
