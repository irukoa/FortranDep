#include "test/suites/Stub/StandardContext.h"

#include "src/FORTRAN/FortranLexer.h"
#include "src/FORTRAN/FortranSyntax.h"
#include "src/TOK/Tokenizer.h"

TEST_F(FDEP_Standard,
       TestFDEP_StatementContainsDefinedModuleNominal) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  // String simulates a Fortran statement, so no ";" "&" will appear in the
  // string.
  const char     String[] = "module a";
  FDEP_Statement Statement;
  bool           IsModule;
  size_t         IndexModule, IndexName;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
  IsModule =
      FDEP_StatementContainsDefinedModule(&Statement, &IndexModule, &IndexName);
  ASSERT(IsModule);
  ASSERT(IndexModule == 0);
  ASSERT(IndexName == 1);
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementContainsDefinedModuleNegative) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  FDEP_Statement Statement;
  bool           Result;
  size_t         IndexModule, IndexName;
  // "end module a" should return false.
  const char String1[] = "end module a";
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String1, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  Result =
      FDEP_StatementContainsDefinedModule(&Statement, &IndexModule, &IndexName);
  ASSERT(!Result);
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
  // "module procedure foo" should return false.
  const char String2[] = "module procedure foo";
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String2, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  Result =
      FDEP_StatementContainsDefinedModule(&Statement, &IndexModule, &IndexName);
  ASSERT(!Result);
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
  // "module function foo" should return false.
  const char String3[] = "module function foo";
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String3, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  Result =
      FDEP_StatementContainsDefinedModule(&Statement, &IndexModule, &IndexName);
  ASSERT(!Result);
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
  // "module subroutine foo" should return false.
  const char String4[] = "module subroutine foo";
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String4, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  Result =
      FDEP_StatementContainsDefinedModule(&Statement, &IndexModule, &IndexName);
  ASSERT(!Result);
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementContainsDefinedModuleNullGuard) {
  (void)ContextData;
  bool           Result;
  size_t         IndexModule, IndexName;
  FDEP_Statement EmptyStatement;
  // NULL statement.
  Result = FDEP_StatementContainsDefinedModule(NULL, &IndexModule, &IndexName);
  ASSERT(!Result);
  // Empty token list.
  EmptyStatement.TokenList  = NULL;
  EmptyStatement.TokenCount = 0;
  Result = FDEP_StatementContainsDefinedModule(&EmptyStatement, &IndexModule,
                                               &IndexName);
  ASSERT(!Result);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementContainsDefinedSubModuleNominal) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  const char     String[]  = "submodule(b) b1";
  FDEP_Statement Statement;
  bool           Result;
  size_t         IndexSubModule, IndexName;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran    = false;
  Result = FDEP_StatementContainsDefinedSubModule(&Statement, &IndexSubModule,
                                                  &IndexName);
  ASSERT(Result);
  ASSERT(IndexSubModule == 0);
  ASSERT(IndexName == Statement.TokenCount - 1);
  ASSERT(0 == strcmp(Statement.TokenList[IndexSubModule + 1], "b"));
  ASSERT(0 == strcmp(Statement.TokenList[IndexName], "b1"));
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementContainsDefinedSubModuleNested) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  const char     String[]  = "submodule(b:c) d";
  FDEP_Statement Statement;
  bool           Result;
  size_t         IndexSubModule, IndexName;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran    = false;
  Result = FDEP_StatementContainsDefinedSubModule(&Statement, &IndexSubModule,
                                                  &IndexName);
  ASSERT(Result);
  ASSERT(IndexSubModule == 0);
  ASSERT(IndexName == Statement.TokenCount - 1);
  ASSERT(0 == strcmp(Statement.TokenList[IndexName], "d"));
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementContainsDefinedSubModuleNegative) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  const char     String[]  = "end submodule b";
  FDEP_Statement Statement;
  bool           Result;
  size_t         IndexSubModule, IndexName;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran    = false;
  Result = FDEP_StatementContainsDefinedSubModule(&Statement, &IndexSubModule,
                                                  &IndexName);
  ASSERT(!Result);
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
  // NULL guard.
  Result =
      FDEP_StatementContainsDefinedSubModule(NULL, &IndexSubModule, &IndexName);
  ASSERT(!Result);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementContainsUsedModuleNominal) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  const char     String[]  = "use b";
  FDEP_Statement Statement;
  bool           Result;
  size_t         IndexUse, IndexName;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran    = false;
  Result = FDEP_StatementContainsUsedModule(&Statement, &IndexUse, &IndexName);
  ASSERT(Result);
  ASSERT(IndexUse == 0);
  ASSERT(IndexName == 1);
  ASSERT(0 == strcmp(Statement.TokenList[IndexName], "b"));
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementContainsUsedModuleNonIntrinsic) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  const char     String[]  = "use non_intrinsic b";
  FDEP_Statement Statement;
  bool           Result;
  size_t         IndexUse, IndexName;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran    = false;
  Result = FDEP_StatementContainsUsedModule(&Statement, &IndexUse, &IndexName);
  ASSERT(Result);
  ASSERT(IndexUse == 0);
  ASSERT(IndexName == 2);
  ASSERT(0 == strcmp(Statement.TokenList[IndexName], "b"));
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementContainsUsedModuleIntrinsic) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  const char     String[]  = "use intrinsic iso_c_binding";
  FDEP_Statement Statement;
  bool           Result;
  size_t         IndexUse, IndexName;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran    = false;
  Result = FDEP_StatementContainsUsedModule(&Statement, &IndexUse, &IndexName);
  ASSERT(!Result);
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementContainsUsedModuleNullGuard) {
  (void)ContextData;
  bool           Result;
  size_t         IndexUse, IndexName;
  FDEP_Statement EmptyStatement;
  // NULL statement.
  Result = FDEP_StatementContainsUsedModule(NULL, &IndexUse, &IndexName);
  ASSERT(!Result);
  // Empty token list.
  EmptyStatement.TokenList  = NULL;
  EmptyStatement.TokenCount = 0;
  Result =
      FDEP_StatementContainsUsedModule(&EmptyStatement, &IndexUse, &IndexName);
  ASSERT(!Result);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_SUITE(FortranLexerSuite,
           TestFDEP_StatementContainsDefinedModuleNominal,
           TestFDEP_StatementContainsDefinedModuleNegative,
           TestFDEP_StatementContainsDefinedModuleNullGuard,
           TestFDEP_StatementContainsDefinedSubModuleNominal,
           TestFDEP_StatementContainsDefinedSubModuleNested,
           TestFDEP_StatementContainsDefinedSubModuleNegative,
           TestFDEP_StatementContainsUsedModuleNominal,
           TestFDEP_StatementContainsUsedModuleNonIntrinsic,
           TestFDEP_StatementContainsUsedModuleIntrinsic,
           TestFDEP_StatementContainsUsedModuleNullGuard);
