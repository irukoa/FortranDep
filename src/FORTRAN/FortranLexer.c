#include "src/FORTRAN/FortranLexer.h"

FDEP_FortranToken FDEP_GetToken(const char *const String) {
  FDEP_FortranToken Token;

  Token = FDEP_TK_UNKNOWN;
  if (strcmp(String, "module") == 0) {
    Token = FDEP_TK_MODULE;
  } else if (strcmp(String, "submodule") == 0) {
    Token = FDEP_TK_SUBMODULE;
  } else if (strcmp(String, "use") == 0) {
    Token = FDEP_TK_USE;
  } else if (strcmp(String, "intrinsic") == 0) {
    Token = FDEP_TK_INTRINSIC;
  } else if (strcmp(String, "non_intrinsic") == 0) {
    Token = FDEP_TK_NON_INTRINSIC;
  } else if (strcmp(String, "procedure") == 0) {
    Token = FDEP_TK_PROCEDURE;
  } else if (strcmp(String, "function") == 0) {
    Token = FDEP_TK_FUNCTION;
  } else if (strcmp(String, "subroutine") == 0) {
    Token = FDEP_TK_SUBROUTINE;
  } else if (strcmp(String, "end") == 0) {
    Token = FDEP_TK_END;
  }
  return Token;
}

bool FDEP_StatementContainsDefinedModule(const FDEP_Statement *const Statement,
                                         size_t *IndexModule,
                                         size_t *IndexName) {
  size_t            i;
  FDEP_FortranToken Token, PrecedingToken, FollowingToken;

  if (!Statement || !(Statement->TokenList) || (Statement->TokenCount == 0)) {
    return false;
  }

  for (i = 0; i < Statement->TokenCount; i++) {
    Token = FDEP_GetToken(Statement->TokenList[i]);
    if (Token == FDEP_TK_MODULE) {
      // Check that the token is not preceded by "end".
      if (i > 0) {
        PrecedingToken = FDEP_GetToken(Statement->TokenList[i - 1]);
        if (PrecedingToken == FDEP_TK_END) {
          return false;
        }
      }
      // Check that the token is not followed by "procedure", "function", or
      // "subroutine".
      if (i < Statement->TokenCount - 1) {
        FollowingToken = FDEP_GetToken(Statement->TokenList[i + 1]);
        if ((FollowingToken == FDEP_TK_PROCEDURE) ||
            (FollowingToken == FDEP_TK_FUNCTION) ||
            (FollowingToken == FDEP_TK_SUBROUTINE)) {
          return false;
        }
      }
      // If we arrived here, the module name follows the module token.
      *IndexModule = i;
      *IndexName   = i + 1;
      return true;
    }
  }
  return false;
}

bool FDEP_StatementContainsDefinedSubModule(
    const FDEP_Statement *const Statement,
    size_t                     *IndexSubModule,
    size_t                     *IndexName) {
  size_t            i;
  FDEP_FortranToken Token, PrecedingToken;

  if (!Statement || !(Statement->TokenList) || (Statement->TokenCount == 0)) {
    return false;
  }

  for (i = 0; i < Statement->TokenCount; i++) {
    Token = FDEP_GetToken(Statement->TokenList[i]);
    if (Token == FDEP_TK_SUBMODULE) {
      // Check that the token is not preceded by "end".
      if (i > 0) {
        PrecedingToken = FDEP_GetToken(Statement->TokenList[i - 1]);
        if (PrecedingToken == FDEP_TK_END) {
          return false;
        }
      }
      // If we arrived here, the submodule name is the last token.
      *IndexSubModule = i;
      *IndexName      = Statement->TokenCount - 1;
      return true;
    }
  }
  return false;
}

bool FDEP_StatementContainsUsedModule(const FDEP_Statement *const Statement,
                                      size_t                     *IndexUse,
                                      size_t                     *IndexName) {
  size_t            i;
  FDEP_FortranToken Token, FollowingToken;

  if (!Statement || !(Statement->TokenList) || (Statement->TokenCount == 0)) {
    return false;
  }

  for (i = 0; i < Statement->TokenCount; i++) {
    Token = FDEP_GetToken(Statement->TokenList[i]);
    if (Token == FDEP_TK_USE) {
      // Check if the token is followed by "intrinsic" or "non_intrinsic".
      if (i < Statement->TokenCount - 1) {
        FollowingToken = FDEP_GetToken(Statement->TokenList[i + 1]);
        if (FollowingToken == FDEP_TK_INTRINSIC) {
          return false;
        }
        if (FollowingToken == FDEP_TK_NON_INTRINSIC) {
          *IndexUse  = i;
          *IndexName = i + 2;
          return true;
        } else {
          *IndexUse  = i;
          *IndexName = i + 1;
          return true;
        }
      }
    }
  }
  return false;
}
