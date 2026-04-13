#include "test/suites/Stub/StandardContext.h"

#include "src/FORTRAN/FortranLexer.h"
#include "src/FORTRAN/FortranSyntax.h"
#include "src/TOK/Tokenizer.h"

TEST_F(FDEP_Standard,
       TestFDEP_StatementContainsDefinedModuleNominal) {
  (void)ContextData;
  bool Ran = false;
  // String simulates a Fortran statement, so no ";" "&" will appear in the
  // String.
  const char     String[] = "module a";
  FDEP_Statement Statement;
  bool           IsModule;
  size_t         IndexModule, IndexName;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran                  = true;
    Statement.TokenCount = FDEP_Tokenize(String, FDEP_FORTRAN_DELIMITERS,
                                         &(Statement.TokenList), NULL);
  }
  ASSERT_X(Ran);
  Ran = false;
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
  IsModule =
      FDEP_StatementContainsDefinedModule(&Statement, &IndexModule, &IndexName);
  ASSERT(IsModule);
  ASSERT(IndexModule == 0);
  ASSERT(IndexName == 1);
  FDEP_FreeTokenList(&(Statement.TokenList), Statement.TokenCount);
}

TEST_SUITE(FortranLexerSuite,
           TestFDEP_StatementContainsDefinedModuleNominal);
