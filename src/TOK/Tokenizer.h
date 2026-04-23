#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include "src/API/API.h"

INTDEC(void,
       FDEP_FreeTokenList,
       char      ***TokenList,
       const size_t TokenCount);

INTDEC(size_t,
       FDEP_Tokenize,
       const char *const String,
       const char *const Delimiters,
       char           ***TokenList,
       FDEP_ErrorCode   *FailByCaller);

typedef struct _FDEP_Statement {
  char **TokenList;  // Array of characters.
  size_t TokenCount; // Array capacity.
} FDEP_Statement;

/* Frees a statement list previously allocated by FDEP_TokenizeStream. It's safe
 * to pass StatementCount = 0.*/
void FDEP_FreeStatementList(FDEP_Statement **StatementList,
                            const size_t     StatementCount);

/* Transforms the input from Stream into a statement list according to
 Delimiters. Any string which ends with a continuation marker is concatenated
 with the next. Statements are strings split along separation markers. The
 function allocates memory for StatementList and returns the number of elements.
 If no statements are found, return value is 0 and no allocation is performed. A
 string preproccesing rule may be optionally applied by passing
 StringPreprocess. Passing NULL will disable string preprocessing.
 On failure:
  if FailByCaller is NULL, calls FDEP_API_ERROR;
  otherwise, writes the error code in the value of FailByCaller and returns 0.
  Any memory allocated by the function is freed.
 */
size_t FDEP_TokenizeStream(FDEP_Statement  **StatementList,
                           const char *const Delimiters,
                           const char        ContinuationMarker,
                           const char        SeparatorMarker,
                           FILE             *Stream,
                           void (*StringPreprocess)(char *String),
                           FDEP_ErrorCode *FailByCaller);

#endif
