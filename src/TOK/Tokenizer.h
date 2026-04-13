#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include "src/API/API.h"

/* Frees a token list previously allocated by FDEP_Tokenize. It's safe to pass
 * TokenCount = 0.*/
void FDEP_FreeTokenList(char ***TokenList,
                        size_t  TokenCount);

/* Tokenizes String according to Delimiters. The function allocates memory for
 * TokenList and returns the number of elements. On failure, if FailByCaller is
 * null, a call to FDEP_API_ERROR is performed. Else, the error code is written
 * in FailByCaller. If no tokens are found, return value is 0 and no allocation
 * is performed.*/
size_t FDEP_Tokenize(const char *const String,
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
                            size_t           StatementCount);

/* Transforms the input from Stream into statements according to Delimiters. Any
 * continuation markers are replaced by newline characters \n. Statements are
 * split along separation markers. The function allocates memory for
 * StatementList and returns the number of elements. On failure, a call to
 * FDEP_API_ERROR is performed. If no statements are found, return value is 0
 * and no allocation is performed. A string preproccesing rule may be optionally
 * applied by passing StringPreprocess. Passing NULL will disable
 * preprocessing.*/
size_t FDEP_TokenizeStream(FDEP_Statement  **StatementList,
                           const char *const Delimiters,
                           const char        ContinuationMarker,
                           const char        SeparatorMarker,
                           FILE             *Stream,
                           void (*StringPreprocess)(char *String));

#endif
