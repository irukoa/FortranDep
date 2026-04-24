#include "src/API/API.h"
#include "src/FORTRAN/FortranDependencyLogic.h"
#include "src/FORTRAN/FortranSyntax.h"
#include "src/TOK/Tokenizer.h"

int main(const int         argc,
         const char *const argv[]) {
  FDEP_Statement **StatementList = NULL;
  size_t           StatementCount;
  FDEP_Target    **TargetList = NULL;
  size_t           TargetCount;
  size_t           i, j;

  (void)argc;
  (void)argv;
  StatementCount = FDEP_TokenizeStream(
      &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
      FDEP_FORTRAN_SEPARATOR, stdin, FDEP_FortranPreprocess, NULL);
  TargetCount = FDEP_StatementListIntoDependencyTree(
      &TargetList, (const FDEP_Statement *const *const)StatementList,
      StatementCount, NULL);
  for (i = 0; i < TargetCount; i++) {
    switch (TargetList[i]->Type) {
    case FDEP_OBJ_OBJECT:
      (void)FDEP_ApiFprintf(stdout, "%s:", FDEP_ObjectName);
      break;
    case FDEP_OBJ_MODULE:
      (void)FDEP_ApiFprintf(stdout, "%s.%s:", TargetList[i]->Name,
                            FDEP_ModuleSuffix);
      break;
    case FDEP_OBJ_SUBMODULE:
      (void)FDEP_ApiFprintf(stdout, "%s.%s:", TargetList[i]->Name,
                            FDEP_SubModuleSuffix);
      break;
    default:
      continue;
    }
    if (TargetList[i]->DependencyCount == 0) {
      (void)FDEP_ApiFprintf(stdout, "\n");
    }
    for (j = 0; j < TargetList[i]->DependencyCount; j++) {
      switch (TargetList[i]->DependencyList[j]->Type) {
      case FDEP_OBJ_SOURCE:
        (void)FDEP_ApiFprintf(stdout, "  %s", FDEP_SourceName);
        break;
      case FDEP_OBJ_MODULE:
        (void)FDEP_ApiFprintf(stdout, "  %s.%s",
                              TargetList[i]->DependencyList[j]->Name,
                              FDEP_ModuleSuffix);
        break;
      case FDEP_OBJ_SUBMODULE:
        (void)FDEP_ApiFprintf(stdout, "  %s.%s",
                              TargetList[i]->DependencyList[j]->Name,
                              FDEP_SubModuleSuffix);
        break;
      default:
        continue;
      }
      if (j != TargetList[i]->DependencyCount - 1) {
        (void)FDEP_ApiFprintf(stdout, "  \\\n");
      } else {
        (void)FDEP_ApiFprintf(stdout, "\n");
      }
    }
  }
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  return EXIT_SUCCESS;
}
