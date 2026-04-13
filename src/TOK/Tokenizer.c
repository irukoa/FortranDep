#include "src/TOK/Tokenizer.h"
#include <stdbool.h>
#include <string.h>

void FDEP_FreeTokenList(char      ***TokenList,
                        const size_t TokenCount) {
  size_t i;

  if (!TokenList || !(*TokenList)) {
    return;
  }
  for (i = 0; i < TokenCount; i++) {
    free((*TokenList)[i]);
  }
  free(*TokenList);
  *TokenList = NULL;
}

size_t FDEP_Tokenize(const char *const String,
                     const char *const Delimiters,
                     char           ***TokenList,
                     FDEP_ErrorCode   *FailByCaller) {
  FDEP_ErrorCode ErrorCode = NO_ERROR;

  char  *StringCpy;
  char  *Token;
  size_t TokenCount;
  void  *TmpTokenList;
  char  *SaveStrCpy;
  // Variables for memory cleanup on failure.
  size_t AllocatedTokenCount;
  size_t i;

  if (!String || !Delimiters || !TokenList) {
    ErrorCode = ERROR_INPUT;
    goto error_handler;
  }
  // Allocate space and copy string.
  StringCpy = NULL;
  StringCpy = (char *)FDEP_ApiMalloc(sizeof(char) * (strlen(String) + 1));
  if (!StringCpy) {
    ErrorCode = ERROR_ALLOC;
    goto error_handler;
  }
  (void)strcpy(StringCpy, String);
  // Initialization.
  *TokenList          = NULL;
  Token               = NULL;
  TokenCount          = 0;
  Token               = strtok_r(StringCpy, Delimiters, &SaveStrCpy);
  AllocatedTokenCount = 0;
  // Main loop.
  while (Token) {
    TokenCount++;
    // Resize TokenList.
    TmpTokenList = NULL;
    TmpTokenList =
        FDEP_ApiRealloc((void *)(*TokenList), sizeof(char *) * TokenCount);
    if (!TmpTokenList) {
      ErrorCode = ERROR_ALLOC;
      goto error_handler;
    }
    *TokenList = (char **)TmpTokenList;
    // Allocate space for the #TokenCount token.
    (*TokenList)[TokenCount - 1] =
        (char *)FDEP_ApiMalloc(sizeof(char) * (strlen(Token) + 1));
    if (!((*TokenList)[TokenCount - 1])) {
      ErrorCode = ERROR_ALLOC;
      goto error_handler;
    }
    AllocatedTokenCount++;
    // Copy the token.
    (void)strcpy((*TokenList)[TokenCount - 1], Token);
    // Get the next token.
    Token = strtok_r(NULL, Delimiters, &SaveStrCpy);
  }
  free(StringCpy);
  return TokenCount;
error_handler:
  free(StringCpy);
  if (*TokenList) {
    for (i = 0; i < AllocatedTokenCount; i++) {
      free((*TokenList)[i]);
    }
    free(*TokenList);
    *TokenList = NULL;
  }
  if (FailByCaller) {
    *FailByCaller = ErrorCode;
    return 0;
  }
  FDEP_API_ERROR(ErrorCode);
  return 0;
}

static void FDEP_FreeStatement(FDEP_Statement *Statement) {
  size_t i;

  if (!Statement || !(Statement->TokenList)) {
    return;
  }
  if (Statement->TokenList) {
    for (i = 0; i < Statement->TokenCount; i++) {
      free(Statement->TokenList[i]);
    }
    free(Statement->TokenList);
  }
  Statement->TokenList  = NULL;
  Statement->TokenCount = 0;
}

static void FDEP_FreePartialStatement(FDEP_Statement *Statement,
                                      const size_t    BuiltTokens) {
  size_t i;

  if (!Statement || !(Statement->TokenList))
    return;
  for (i = 0; i < BuiltTokens; i++) {
    free(Statement->TokenList[i]);
  }
  free(Statement->TokenList);
  Statement->TokenList  = NULL;
  Statement->TokenCount = 0;
}

void FDEP_FreeStatementList(FDEP_Statement **StatementList,
                            size_t           StatementCount) {
  size_t i;

  if (!StatementList || !(*StatementList))
    return;
  for (i = 0; i < StatementCount; i++) {
    FDEP_FreeStatement(&(*StatementList)[i]);
  }
  free(*StatementList);
  *StatementList = NULL;
}

size_t FDEP_TokenizeStream(FDEP_Statement  **StatementList,
                           const char *const Delimiters,
                           const char        ContinuationMarker,
                           const char        SeparatorMarker,
                           FILE             *Stream,
                           void (*StringPreprocess)(char *String)) {
  FDEP_ErrorCode ErrorCode = NO_ERROR;

  // Statement proccessing.
  char  *String;
  size_t Cap;
  char  *ContPos;
  char  *NextLine;
  size_t NCap;
  void  *TmpString;
  char  *Substring;
  char  *SaveSubstring;
  char   SeparatorMarkerString[2];
  // Statement tokenizing.
  void           *TmpStatementList;
  char          **TokenBuffer;
  size_t          FoundTokens;
  FDEP_Statement *CurrentStatement;
  size_t          StatementCount;
  size_t          i;
  // Variables for memory cleanup on failure.
  size_t AllocatedStatementCount;
  size_t AllocatedTokenCount;
  bool   HasCurrentStatement;

  if (!StatementList || !Delimiters || !Stream) {
    ErrorCode = ERROR_INPUT;
    goto error_handler;
  }
  // Initialization.
  StatementCount           = 0;
  *StatementList           = NULL;
  AllocatedStatementCount  = 0;
  AllocatedTokenCount      = 0;
  HasCurrentStatement      = false;
  SeparatorMarkerString[0] = SeparatorMarker;
  SeparatorMarkerString[1] = '\0';
  // Main loop.
  String    = NULL;
  Cap       = 0;
  NextLine  = NULL;
  NCap      = 0;
  Substring = NULL;
  while ((getline(&String, &Cap, Stream)) != -1) {
    if (StringPreprocess) {
      StringPreprocess(String);
    }
    // Account for line continuation.
    ContPos = NULL;
    while ((ContPos = strchr(String, ContinuationMarker))) {
      // Replace continuation marker with null character.
      *ContPos = '\0';
      // Get the next line.
      if (getline(&NextLine, &NCap, Stream) == -1) {
        if (FDEP_ApiFerror(Stream)) {
          ErrorCode = ERROR_STREAM;
          goto error_handler;
        }
        if (feof(Stream)) {
          break;
        }
      }
      if (StringPreprocess) {
        StringPreprocess(NextLine);
      }
      // Allocate space for the next line.
      Cap       = strlen(String) + strlen(NextLine) + 2;
      TmpString = NULL;
      TmpString = FDEP_ApiRealloc((void *)String, sizeof(char) * Cap);
      if (!TmpString) {
        ErrorCode = ERROR_ALLOC;
        goto error_handler;
      }
      String = (char *)TmpString;
      // Insert newline before '\0' and concatenate.
      (void)strcat(String, "\n");
      (void)strcat(String, NextLine);
    }
    // Splits String along statement separator markers.
    Substring = strtok_r(String, SeparatorMarkerString, &SaveSubstring);
    while (Substring) {
      // Seach for tokens in statement. Store in TokenBuffer.
      TokenBuffer = NULL;
      FoundTokens =
          FDEP_Tokenize(Substring, Delimiters, &TokenBuffer, &ErrorCode);
      if (ErrorCode != NO_ERROR) {
        // FDEP_Tokenize set an error.
        goto error_handler;
      }
      if (FoundTokens > 0) {
        StatementCount++;
        // Expand *StatementList.
        TmpStatementList = NULL;
        TmpStatementList = (FDEP_Statement *)FDEP_ApiRealloc(
            (void *)(*StatementList), sizeof(FDEP_Statement) * StatementCount);
        if (!TmpStatementList) {
          ErrorCode = ERROR_ALLOC;
          goto error_handler;
        }
        *StatementList = TmpStatementList;
        // Cast CurrentStatement to *StatementList element.
        CurrentStatement =
            (FDEP_Statement *)(&(*StatementList)[StatementCount - 1]);
        CurrentStatement->TokenList  = NULL;
        CurrentStatement->TokenCount = 0;
        HasCurrentStatement          = true;
        AllocatedTokenCount          = 0;
        // Allocate CurrentStatement.
        CurrentStatement->TokenList = NULL;
        CurrentStatement->TokenList =
            (char **)FDEP_ApiMalloc(sizeof(char *) * FoundTokens);
        if (!(CurrentStatement->TokenList)) {
          ErrorCode = ERROR_ALLOC;
          goto error_handler;
        }
        // Allocate CurrentStatement->TokenList[i].
        for (i = 0; i < FoundTokens; i++) {
          CurrentStatement->TokenList[i] = NULL;
          CurrentStatement->TokenList[i] = (char *)FDEP_ApiMalloc(
              sizeof(char) * (strlen(TokenBuffer[i]) + 1));
          if (!(CurrentStatement->TokenList[i])) {
            ErrorCode = ERROR_ALLOC;
            goto error_handler;
          }
          // Copy tokens.
          (void)strcpy(CurrentStatement->TokenList[i], TokenBuffer[i]);
          AllocatedTokenCount++;
        }
        CurrentStatement->TokenCount = FoundTokens;
        AllocatedStatementCount++;
        HasCurrentStatement = false;
      }
      Substring = strtok_r(NULL, SeparatorMarkerString, &SaveSubstring);
      // Free buffer.
      FDEP_FreeTokenList(&TokenBuffer, FoundTokens);
      TokenBuffer = NULL;
    }
  }
  if (FDEP_ApiFerror(Stream)) {
    ErrorCode = ERROR_STREAM;
    goto error_handler;
  }
  // Free locals.
  free(NextLine);
  free(String);
  return StatementCount;
error_handler:
  // Free locals.
  free(NextLine);
  free(String);
  // Free TokenBuffer.
  if (TokenBuffer) {
    FDEP_FreeTokenList(&TokenBuffer, FoundTokens);
  }
  // Free partially built current statement.
  if (HasCurrentStatement && *StatementList) {
    FDEP_FreePartialStatement(&(*StatementList)[AllocatedStatementCount],
                              AllocatedTokenCount);
  }
  // Free completed statements.
  FDEP_FreeStatementList(StatementList, AllocatedStatementCount);
  FDEP_API_ERROR(ErrorCode);
  return 0;
}
