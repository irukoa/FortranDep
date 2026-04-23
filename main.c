#include "src/API/API.h"
#include "src/FORTRAN/FortranDependencyLogic.h"
#include "src/FORTRAN/FortranSyntax.h"
#include "src/TOK/Tokenizer.h"

// TODO: MODIFY API TO PASS ALWAYS BY REFERENCE!

// Differentiate between gfortran and ifort in submodule dependency printing:
// if dependency does NOT contain ':', gfortran expects *.smod and ifort *.mod.

// static const char FDEP_ObjectName[]      = "#OBJECT#";
// static const char FDEP_SourceName[]      = "#SOURCEFILE#";
// static const char FDEP_ModuleSuffix[]    = "mod";
// static const char FDEP_SubModuleSuffix[] = "smod";

int main(const int         argc,
         const char *const argv[]) {
  FDEP_Statement *StatementList = NULL;
  size_t          StatementCount;
  size_t          i, j, Counter;

  FDEP_Target **TargetList = NULL;
  size_t        TargetCount;

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

  TargetCount = FDEP_StatementListIntoDependencyTree(
      &TargetList, (const FDEP_Statement *const *const)&StatementList,
      StatementCount, NULL);

  fprintf(stdout, "-TGC: %zu\n", TargetCount);
  for (i = 0; i < TargetCount; i++) {
    fprintf(stdout, "-TG: %zu: %s (%i)\n", i + 1, TargetList[i]->Name,
            TargetList[i]->Type);
    for (j = 0; j < TargetList[i]->DependencyCount; j++) {
      fprintf(stdout, "%zu.%zu: %s (%i)\n", i + 1, j + 1,
              TargetList[i]->DependencyList[j]->Name,
              TargetList[i]->DependencyList[j]->Type);
    }
  }

  FDEP_FreeStatementList(&StatementList, StatementCount);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  return EXIT_SUCCESS;
}
