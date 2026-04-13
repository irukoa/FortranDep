#include "test/suites/Stub/StandardContext.h"

#include "src/FORTRAN/FortranSyntax.h"
#include "src/TOK/Tokenizer.h"

static FILE *PutInFakeStream(const char *const Contents) {
  FILE *Stream = fmemopen((void *)Contents, strlen(Contents), "r");
  ASSERT_X(Stream);
  return Stream;
}
static const char Contents[] = "module b;&&\n"
                               "  !acomment\n"
                               "end module b\n"
                               "module a\n"
                               "  use b\n"
                               "  use, intrinsic :: ISO_FORTRAN_ENV\n"
                               "  use, NON_intrinsic :: c\n"
                               "\n"
                               "  integer, parameter :: dp = kind(1.0d0)\n"
                               "\n"
                               "  interface\n"
                               "    module subroutine sub1()\n"
                               "    end subroutine\n"
                               "  end interface\n"
                               "\n"
                               "end module a\n"
                               "submodule (a) as\n"
                               "  integer, parameter :: a1 = 1\n"
                               "end submodule\n"
                               "submodule (as) ass\n"
                               "  integer, parameter :: as1 = 5\n"
                               "end submodule\n";

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeNominal) {
  (void)ContextData;
  bool              Ran        = false;
  const char        String[]   = "my:String";
  const char *const Delimiters = ":";
  char            **TokenList;
  size_t            TokenCount;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran        = true;
    TokenCount = FDEP_Tokenize(String, Delimiters, &TokenList, NULL);
  }
  ASSERT_X(Ran);
  Ran = false;
  ASSERT(TokenCount == 2);
  ASSERT(0 == strcmp(TokenList[0], "my"));
  ASSERT(0 == strcmp(TokenList[1], "String"));
  FDEP_FreeTokenList(&TokenList, TokenCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       TestFDEP_TokenizeStreamNominal) {
  FDEP_StandardContextData *Data = (FDEP_StandardContextData *)ContextData;
  bool                      Ran  = false;
  FDEP_Statement           *StatementList;
  size_t                    StatementCount;
  Data->FakeStream = PutInFakeStream(Contents);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess);
  }
  ASSERT_X(Ran);
  Ran = false;
  ASSERT(18 == StatementCount);
  ASSERT(0 == strcmp(StatementList[1].TokenList[0], "end"));
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_SUITE(TokenizerSuite,
           TestFDEP_TokenizeNominal,
           TestFDEP_TokenizeStreamNominal);
