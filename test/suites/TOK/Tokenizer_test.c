#include "test/suites/Stub/StandardContext.h"

#include "src/FORTRAN/FortranSyntax.h"
#include "src/TOK/Tokenizer.h"

static FILE *PutInFakeStream(const char *const Contents) {
  FILE *Stream = fmemopen((void *)Contents, strlen(Contents), "r");
  ASSERT_X(Stream);
  return Stream;
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeNominal) {
  (void)ContextData;
  bool              Ran        = false;
  FDEP_ErrorCode    ErrorCode  = NO_ERROR;
  const char        String[]   = "my:String";
  const char *const Delimiters = ":";
  char            **TokenList;
  size_t            TokenCount;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran        = true;
    TokenCount = FDEP_Tokenize(String, Delimiters, &TokenList, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TokenCount == 2);
  ASSERT(0 == strcmp(TokenList[0], "my"));
  ASSERT(0 == strcmp(TokenList[1], "String"));
  FDEP_FreeTokenList(&TokenList, TokenCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamFunctional1) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement           *StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a&\n"
                                         "  &&\n"
                                         "  &a\n"
                                         "end &\n"
                                         "module a&\n"
                                         " &a\n";
  Data->FakeStream                     = PutInFakeStream(Contents);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(2 == StatementCount);
  ASSERT(0 == strcmp(StatementList[0].TokenList[1], "aa"));
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamFunctional2) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement           *StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a&\n"
                                         "  &&\n"
                                         "  &a\n"
                                         "contains\n"
                                         "  subroutine &\n"
                                         "    s(&\n"
                                         "    )\n"
                                         "    print*, \"singleline&\" ; end ; end module aa; "
                                         "module b; end module b\n";
  Data->FakeStream                     = PutInFakeStream(Contents);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(8 == StatementCount);
  ASSERT(0 == strcmp(StatementList[7].TokenList[2], "b"));
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamFunctional3) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement           *StatementList;
  size_t                    StatementCount;
  const char                Contents[] =
      "module a&\n"
      "  &&\n"
      "  &a\n"
      "contains\n"
      "  subroutine &\n"
      "    s(&\n"
      "    )\n"
      "    print*, \"multiline&\n"
      "        &string!\" ; end ; end module aa; module bb; end module bb\n";
  Data->FakeStream = PutInFakeStream(Contents);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(8 == StatementCount);
  ASSERT(0 == strcmp(StatementList[7].TokenList[2], "bb"));
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamFunctional4) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement           *StatementList;
  size_t                    StatementCount;
  const char                Contents[] =
      "module a&\n"
      "  &&\n"
      "  &a\n"
      "contains\n"
      "  subroutine &\n"
      "    s(&\n"
      "    )\n"
      "    print*, \"multiline&\n"
      "        &string!\" ; end ; end module aa; & !mycomment\n "
      "module bbb; end module bbb\n";
  Data->FakeStream = PutInFakeStream(Contents);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(8 == StatementCount);
  ASSERT(0 == strcmp(StatementList[7].TokenList[2], "bbb"));
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_SUITE(TokenizerSuite,
           TestFDEP_TokenizeNominal,
           TestFDEP_TokenizeStreamFunctional1,
           TestFDEP_TokenizeStreamFunctional2,
           TestFDEP_TokenizeStreamFunctional3,
           TestFDEP_TokenizeStreamFunctional4);
