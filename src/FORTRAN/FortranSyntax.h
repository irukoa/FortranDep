#ifndef _FORTRAN_SYNTAX_H
#define _FORTRAN_SYNTAX_H

#define FDEP_FORTRAN_DELIMITERS   " \t\n(),:"
#define FDEP_FORTRAN_CONTINUATION '&'
#define FDEP_FORTRAN_SEPARATOR    ';'
#define FDEP_FORTRAN_COMMENT      '!'
#define FDEP_FORTRAN_STRING1      '\''
#define FDEP_FORTRAN_STRING2      '"'

/* Preprocesses String according to Fortran format:
 - strips everything past the '!' token, and
 - makes everything lowercase.*/
void FDEP_FortranPreprocess(char *String);

#endif
