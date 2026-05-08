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
  bool             LaxMode = false;
  size_t           ActualDependencyCount, RollingDependencyCount;

  if (argc > 1) {
    if (strcmp(argv[1], "-l") == 0) {
      LaxMode = true;
    }
  }

  StatementCount = FDEP_TokenizeStream(
      &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
      FDEP_FORTRAN_SEPARATOR, stdin, FDEP_FortranPreprocess, NULL);

  TargetCount = FDEP_StatementListIntoDependencyTree(
      &TargetList, (const FDEP_Statement *const *const)StatementList,
      StatementCount, NULL);

  if (!LaxMode) {
    FDEP_VerifyCompilationUnits(&TargetList, TargetCount);
  }

  for (i = 0; i < TargetCount; i++) {

    switch (TargetList[i]->Type) {
    case FDEP_OBJ_OBJECT:
      (void)FDEP_ApiFprintf(stdout, "%s:", FDEP_OBJECT_NAME);
      break;
    case FDEP_OBJ_MODULE:
      (void)FDEP_ApiFprintf(stdout, "%s.%s:", TargetList[i]->Name,
                            FDEP_MODULE_SUFFIX);
      break;
    case FDEP_OBJ_SUBMODULE:
      (void)FDEP_ApiFprintf(stdout, "%s.%s:", TargetList[i]->Name,
                            FDEP_SUBMODULE_SUFFIX);
      break;
    default:
      continue;
    }

    if (TargetList[i]->DependencyCount == 0) {
      (void)FDEP_ApiFprintf(stdout, "\n");
    }

    ActualDependencyCount = 0;
    for (j = 0; j < TargetList[i]->DependencyCount; j++) {
      if (!TargetList[i]->DependencyList[j]->InCompilationUnit) {
        ActualDependencyCount++;
      }
    }

    RollingDependencyCount = 0;
    for (j = 0; j < TargetList[i]->DependencyCount; j++) {
      if (!TargetList[i]->DependencyList[j]->InCompilationUnit) {

        switch (TargetList[i]->DependencyList[j]->Type) {
        case FDEP_OBJ_SOURCE:
          (void)FDEP_ApiFprintf(stdout, "  %s", FDEP_SOURCE_NAME);
          break;
        case FDEP_OBJ_MODULE:
          (void)FDEP_ApiFprintf(stdout, "  %s.%s",
                                TargetList[i]->DependencyList[j]->Name,
                                FDEP_MODULE_SUFFIX);
          break;
        case FDEP_OBJ_SUBMODULE:
          (void)FDEP_ApiFprintf(stdout, "  %s.%s",
                                TargetList[i]->DependencyList[j]->Name,
                                FDEP_SUBMODULE_SUFFIX);
          break;
        default:
          continue;
        }
        RollingDependencyCount++;

        if (RollingDependencyCount != ActualDependencyCount) {
          (void)FDEP_ApiFprintf(stdout, "  \\\n");
        } else {
          (void)FDEP_ApiFprintf(stdout, "\n");
          break;
        }
      }
    }
  }

  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);

  return EXIT_SUCCESS;
}
