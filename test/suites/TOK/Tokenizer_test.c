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
  FDEP_Statement          **StatementList;
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
  ASSERT(0 == strcmp(StatementList[0]->TokenList[1], "aa"));
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamFunctional2) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
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
  ASSERT(0 == strcmp(StatementList[7]->TokenList[2], "b"));
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamFunctional3) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
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
  ASSERT(0 == strcmp(StatementList[7]->TokenList[2], "bb"));
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamFunctional4) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
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
      "module bbb; end module bbb&\n";
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
  ASSERT(0 == strcmp(StatementList[7]->TokenList[2], "bbb"));
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_SUITE(TokenizerSuite,
           TestFDEP_TokenizeNominal,
           TestFDEP_TokenizeStreamFunctional1,
           TestFDEP_TokenizeStreamFunctional2,
           TestFDEP_TokenizeStreamFunctional3,
           TestFDEP_TokenizeStreamFunctional4);

TEST_F(FDEP_Standard,
       TestFDEP_FreeTokenListNull) {
  (void)ContextData;
  char **TokenList = NULL;
  FDEP_FreeTokenList(NULL, 0);
  FDEP_FreeTokenList(&TokenList, 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeError1) {
  (void)ContextData;
  bool              Ran        = false;
  FDEP_ErrorCode    ErrorCode  = NO_ERROR;
  const char        String[]   = "my:String";
  const char *const Delimiters = ":";
  char            **TokenList;
  size_t            TokenCount;
  // Input errors.
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran        = true;
    TokenCount = FDEP_Tokenize(NULL, Delimiters, &TokenList, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_INPUT);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(TokenCount == 0);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran        = true;
    TokenCount = FDEP_Tokenize(String, NULL, &TokenList, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_INPUT);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(TokenCount == 0);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran        = true;
    TokenCount = FDEP_Tokenize(String, Delimiters, NULL, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_INPUT);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(TokenCount == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeError1e) {
  (void)ContextData;
  bool              Ran        = false;
  const char        String[]   = "my:String";
  const char *const Delimiters = ":";
  char            **TokenList;
  // Input errors.
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    (void)FDEP_Tokenize(NULL, Delimiters, &TokenList, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 1);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[0] == ERROR_INPUT);
  Ran = false;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    (void)FDEP_Tokenize(String, NULL, &TokenList, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 2);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[1] == ERROR_INPUT);
  Ran = false;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    (void)FDEP_Tokenize(String, Delimiters, NULL, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 3);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[2] == ERROR_INPUT);
  Ran = false;
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeError2) {
  (void)ContextData;
  bool              Ran        = false;
  FDEP_ErrorCode    ErrorCode  = NO_ERROR;
  const char        String[]   = "my:String";
  const char *const Delimiters = ":";
  char            **TokenList;
  size_t            TokenCount;
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran        = true;
    TokenCount = FDEP_Tokenize(String, Delimiters, &TokenList, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(TokenCount == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeError2e) {
  (void)ContextData;
  bool              Ran        = false;
  const char        String[]   = "my:String";
  const char *const Delimiters = ":";
  char            **TokenList;
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    (void)FDEP_Tokenize(String, Delimiters, &TokenList, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 1);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[0] == ERROR_ALLOC);
  Ran = false;
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeError3) {
  (void)ContextData;
  bool              Ran        = false;
  FDEP_ErrorCode    ErrorCode  = NO_ERROR;
  const char        String[]   = "my:String";
  const char *const Delimiters = ":";
  char            **TokenList;
  size_t            TokenCount;
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran        = true;
    TokenCount = FDEP_Tokenize(String, Delimiters, &TokenList, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(TokenCount == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeError4) {
  (void)ContextData;
  bool              Ran        = false;
  FDEP_ErrorCode    ErrorCode  = NO_ERROR;
  const char        String[]   = "my:String";
  const char *const Delimiters = ":";
  char            **TokenList;
  size_t            TokenCount;
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(3);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran        = true;
    TokenCount = FDEP_Tokenize(String, Delimiters, &TokenList, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(TokenCount == 0);
}

size_t FDEP_Tokenize_Mock(const char *const String,
                          const char *const Delimiters,
                          char           ***TokenList,
                          FDEP_ErrorCode   *FailByCaller) {
  (void)String;
  (void)Delimiters;
  (void)FailByCaller;
  *TokenList = (char **)malloc(sizeof(char *) * 1);
  ASSERT_X(*TokenList);
  (*TokenList)[0] = (char *)malloc(sizeof(char) * 2);
  ASSERT_X((*TokenList)[0]);
  ((*TokenList)[0])[0] = 'a';
  ((*TokenList)[0])[1] = '\0';
  return 1;
}

size_t FDEP_Tokenize_MockError(const char *const String,
                               const char *const Delimiters,
                               char           ***TokenList,
                               FDEP_ErrorCode   *FailByCaller) {
  (void)String;
  (void)Delimiters;
  (void)FailByCaller;
  *TokenList    = NULL;
  *FailByCaller = ERROR_ALLOC;
  return 0;
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError1) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a\n"
                                         "integer :: &\n "
                                         "i &\n"
                                         "= &\n"
                                         "5\n "
                                         "end module a&\n ";
  Data->FakeStream                     = PutInFakeStream(Contents);
  // Input errors.
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        NULL, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_INPUT);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, NULL, FDEP_FORTRAN_CONTINUATION, FDEP_FORTRAN_SEPARATOR,
        Data->FakeStream, FDEP_FortranPreprocess, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_INPUT);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, NULL, FDEP_FortranPreprocess, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_INPUT);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError1e) {
  FDEP_StandardContextData *Data = (FDEP_StandardContextData *)ContextData;
  bool                      Ran  = false;
  FDEP_Statement          **StatementList;
  const char                Contents[] = "module a\n"
                                         "integer :: &\n "
                                         "i &\n"
                                         "= &\n"
                                         "5\n "
                                         "end module a&\n ";
  Data->FakeStream                     = PutInFakeStream(Contents);
  // Input errors.
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    (void)FDEP_TokenizeStream(NULL, FDEP_FORTRAN_DELIMITERS,
                              FDEP_FORTRAN_CONTINUATION, FDEP_FORTRAN_SEPARATOR,
                              Data->FakeStream, FDEP_FortranPreprocess, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 1);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[0] == ERROR_INPUT);
  Ran = false;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    (void)FDEP_TokenizeStream(&StatementList, NULL, FDEP_FORTRAN_CONTINUATION,
                              FDEP_FORTRAN_SEPARATOR, Data->FakeStream,
                              FDEP_FortranPreprocess, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 2);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[1] == ERROR_INPUT);
  Ran = false;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    (void)FDEP_TokenizeStream(&StatementList, FDEP_FORTRAN_DELIMITERS,
                              FDEP_FORTRAN_CONTINUATION, FDEP_FORTRAN_SEPARATOR,
                              NULL, FDEP_FortranPreprocess, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 3);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[2] == ERROR_INPUT);
  Ran = false;
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError2) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a\n"
                                         "integer :: &\n "
                                         "i &\n"
                                         "= &\n"
                                         "5\n "
                                         "end module a&\n ";
  Data->FakeStream                     = PutInFakeStream(Contents);
  FDEP_ResetFerrorFailCfg();
  FDEP_SetFerrorFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_STREAM);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError2e) {
  FDEP_StandardContextData *Data = (FDEP_StandardContextData *)ContextData;
  bool                      Ran  = false;
  FDEP_Statement          **StatementList;
  const char                Contents[] = "module a\n"
                                         "integer :: &\n "
                                         "i &\n"
                                         "= &\n"
                                         "5\n "
                                         "end module a&\n ";
  Data->FakeStream                     = PutInFakeStream(Contents);
  FDEP_ResetFerrorFailCfg();
  FDEP_SetFerrorFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    (void)FDEP_TokenizeStream(&StatementList, FDEP_FORTRAN_DELIMITERS,
                              FDEP_FORTRAN_CONTINUATION, FDEP_FORTRAN_SEPARATOR,
                              Data->FakeStream, FDEP_FortranPreprocess, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 1);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[0] == ERROR_STREAM);
  Ran = false;
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError3) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a&\n";
  Data->FakeStream                     = PutInFakeStream(Contents);
  FDEP_ResetFerrorFailCfg();
  FDEP_SetFerrorFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_STREAM);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError4) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a\n"
                                         "integer :: &\n "
                                         "i &\n"
                                         "= &\n"
                                         "5\n "
                                         "end module a&\n ";
  Data->FakeStream                     = PutInFakeStream(Contents);
  FDEP_Tokenize                        = FDEP_Tokenize_Mock;
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
  RESET_FN(FDEP_Tokenize);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError5) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a&\n"
                                         ";integer :: &\n "
                                         "i &\n"
                                         "= &\n"
                                         "5\n "
                                         "end module a&\n ";
  Data->FakeStream                     = PutInFakeStream(Contents);
  FDEP_Tokenize                        = FDEP_Tokenize_Mock;
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
  RESET_FN(FDEP_Tokenize);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError6) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a\n"
                                         "integer :: &\n "
                                         "i &\n"
                                         "= &\n"
                                         "5\n "
                                         "end module a&\n ";
  Data->FakeStream                     = PutInFakeStream(Contents);
  FDEP_Tokenize                        = FDEP_Tokenize_Mock;
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
  RESET_FN(FDEP_Tokenize);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError7) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a\n"
                                         "integer :: &\n "
                                         "i &\n"
                                         "= &\n"
                                         "5\n "
                                         "end module a&\n ";
  Data->FakeStream                     = PutInFakeStream(Contents);
  FDEP_Tokenize                        = FDEP_Tokenize_Mock;
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(2);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
  RESET_FN(FDEP_Tokenize);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError8) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a\n"
                                         "integer :: &\n "
                                         "i &\n"
                                         "= &\n"
                                         "5\n "
                                         "end module a&\n ";
  Data->FakeStream                     = PutInFakeStream(Contents);
  FDEP_Tokenize                        = FDEP_Tokenize_Mock;
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(3);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
  RESET_FN(FDEP_Tokenize);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError9) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a\n"
                                         "integer :: &\n "
                                         "i &\n"
                                         "= &\n"
                                         "5\n "
                                         "end module a&\n ";
  Data->FakeStream                     = PutInFakeStream(Contents);
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(4 + 3);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamError10) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  const char                Contents[] = "module a\n"
                                         "integer :: &\n "
                                         "i &\n"
                                         "= &\n"
                                         "5\n "
                                         "end module a&\n ";
  Data->FakeStream                     = PutInFakeStream(Contents);
  FDEP_Tokenize                        = FDEP_Tokenize_MockError;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  Ran       = false;
  ErrorCode = NO_ERROR;
  ASSERT(StatementCount == 0);
  RESET_FN(FDEP_Tokenize);
}

TEST_SUITE(TokenizerErrorSuite,
           TestFDEP_FreeTokenListNull,
           TestFDEP_TokenizeError1,
           TestFDEP_TokenizeError1e,
           TestFDEP_TokenizeError2,
           TestFDEP_TokenizeError2e,
           TestFDEP_TokenizeError3,
           TestFDEP_TokenizeError4,
           TestFDEP_TokenizeStreamError1,
           TestFDEP_TokenizeStreamError1e,
           TestFDEP_TokenizeStreamError2,
           TestFDEP_TokenizeStreamError2e,
           TestFDEP_TokenizeStreamError3,
           TestFDEP_TokenizeStreamError4,
           TestFDEP_TokenizeStreamError5,
           TestFDEP_TokenizeStreamError6,
           TestFDEP_TokenizeStreamError7,
           TestFDEP_TokenizeStreamError8,
           TestFDEP_TokenizeStreamError9,
           TestFDEP_TokenizeStreamError10);
