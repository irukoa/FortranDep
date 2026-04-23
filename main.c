#include "src/API/API.h"
#include "src/FORTRAN/FortranSyntax.h"
#include "src/TOK/Tokenizer.h"

int main(const int         argc,
         const char *const argv[]) {
  FDEP_Statement *StatementList = NULL;
  size_t          StatementCount;
  size_t          i, j, Counter;

  (void)argc;
  (void)argv;

  StatementCount = FDEP_TokenizeStream(
      &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
      FDEP_FORTRAN_SEPARATOR, stdin, FDEP_FortranPreprocess, NULL);
  fprintf(stdout, "-STC: %zu\n", StatementCount);
  Counter = 0;
  for (i = 0; i < StatementCount; i++) {
    fprintf(stdout, "-ST: %zu\n", i + 1);
    for (j = 0; j < StatementList[i].TokenCount; j++) {
      Counter++;
      fprintf(stdout, "%zu: %zu.%zu: %s\n", Counter, i + 1, j + 1,
              StatementList[i].TokenList[j]);
    }
  }
  FDEP_FreeStatementList(&StatementList, StatementCount);
  fprintf(stdout, "#OBJTARGET#: #FILEDEP#\n");
  return EXIT_SUCCESS;
}
