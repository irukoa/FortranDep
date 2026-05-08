#include "src/FORTRAN/FortranDependencyLogic.h"
#include "src/FORTRAN/FortranSyntax.h"

size_t FDEP_StatementListIntoDependencyTree(
    FDEP_Target                     ***TargetList,
    const FDEP_Statement *const *const StatementList,
    const size_t                       StatementCount,
    FDEP_ErrorCode                    *FailByCaller) {
  FDEP_ErrorCode      ErrorCode   = NO_ERROR;
  size_t              TargetCount = 0;
  void               *TmpTargetList;
  size_t              i;
  bool                DefinesModule, DefinesSubmodule, UsesModule;
  size_t              IndexKeyword, IndexName;
  char               *String = NULL;
  size_t              AncestorCount;
  FDEP_FortranObjType CurrentOwner = FDEP_OBJ_OBJECT;

  if (!TargetList || !StatementList) {
    ErrorCode = ERROR_INPUT;
    if (FailByCaller) {
      *FailByCaller = ErrorCode;
      return 0;
    }
    FDEP_API_ERROR(ErrorCode);
  }

  // Initialization.
  *TargetList = NULL;

  // The first target is always the object file.
  TargetCount   = 1;
  TmpTargetList = NULL;
  TmpTargetList = FDEP_ApiRealloc((void *)*TargetList, sizeof(FDEP_Target *));
  if (!TmpTargetList) {
    ErrorCode = ERROR_ALLOC;
    goto error_handler;
  }
  *TargetList = (FDEP_Target **)TmpTargetList;

  (*TargetList)[0] = FDEP_NewTarget(FDEP_OBJECT_NAME,
                                    (FDEP_ObjType)FDEP_OBJ_OBJECT, &ErrorCode);
  if (ErrorCode != NO_ERROR) {
    goto error_handler;
  }

  // The first dependency is always the source file.
  (void)FDEP_AddDependencyToTarget(FDEP_SOURCE_NAME,
                                   (FDEP_ObjType)FDEP_OBJ_SOURCE, false,
                                   &((*TargetList)[0]), &ErrorCode);
  if (ErrorCode != NO_ERROR) {
    goto error_handler;
  }

  for (i = 0; i < StatementCount; i++) {

    if (FDEP_StatementEndsProgramUnit(StatementList[i])) {
      CurrentOwner = FDEP_OBJ_OBJECT;
    }

    DefinesModule = FDEP_StatementContainsDefinedModule(
        StatementList[i], &IndexKeyword, &IndexName);
    if ((DefinesModule) && (IndexName < StatementList[i]->TokenCount)) {
      // New module target.
      CurrentOwner  = FDEP_OBJ_MODULE;
      TmpTargetList = NULL;
      TmpTargetList = (FDEP_Target **)FDEP_ApiRealloc(
          (void *)(*TargetList), sizeof(FDEP_Target *) * (TargetCount + 1));
      if (!TmpTargetList) {
        ErrorCode = ERROR_ALLOC;
        goto error_handler;
      }
      *TargetList = (FDEP_Target **)TmpTargetList;
      TargetCount++;
      (*TargetList)[TargetCount - 1] =
          FDEP_NewTarget(StatementList[i]->TokenList[IndexName],
                         (FDEP_ObjType)FDEP_OBJ_MODULE, &ErrorCode);
      if (ErrorCode != NO_ERROR) {
        goto error_handler;
      }

      // The module target depends on the source file.
      (void)FDEP_AddDependencyToTarget(
          FDEP_SOURCE_NAME, (FDEP_ObjType)FDEP_OBJ_SOURCE, false,
          &((*TargetList)[TargetCount - 1]), &ErrorCode);
      if (ErrorCode != NO_ERROR) {
        goto error_handler;
      }
      // The object target depends on the module file.
      (void)FDEP_AddDependencyToTarget(StatementList[i]->TokenList[IndexName],
                                       (FDEP_ObjType)FDEP_OBJ_MODULE, false,
                                       &((*TargetList)[0]), &ErrorCode);
      if (ErrorCode != NO_ERROR) {
        goto error_handler;
      }
      continue;
    }

    DefinesSubmodule = FDEP_StatementContainsDefinedSubModule(
        StatementList[i], &IndexKeyword, &IndexName);
    if ((DefinesSubmodule) && (IndexName < StatementList[i]->TokenCount) &&
        (IndexKeyword < IndexName - 1)) {
      // New submodule target.
      CurrentOwner  = FDEP_OBJ_SUBMODULE;
      TmpTargetList = NULL;
      TmpTargetList = (FDEP_Target **)FDEP_ApiRealloc(
          (void *)(*TargetList), sizeof(FDEP_Target *) * (TargetCount + 1));
      if (!TmpTargetList) {
        ErrorCode = ERROR_ALLOC;
        goto error_handler;
      }
      *TargetList = (FDEP_Target **)TmpTargetList;
      String      = NULL;
      String      = (char *)FDEP_ApiMalloc(
          sizeof(char) *
          (strlen(StatementList[i]->TokenList[IndexKeyword + 1]) +
           strlen(StatementList[i]->TokenList[IndexName]) + 2));
      if (!String) {
        ErrorCode = ERROR_ALLOC;
        goto error_handler;
      }
      TargetCount++;
      (void)strcpy(String, StatementList[i]->TokenList[IndexKeyword + 1]);
      (void)strcat(String, "@");
      (void)strcat(String, StatementList[i]->TokenList[IndexName]);
      (*TargetList)[TargetCount - 1] =
          FDEP_NewTarget(String, (FDEP_ObjType)FDEP_OBJ_SUBMODULE, &ErrorCode);
      if (ErrorCode != NO_ERROR) {
        goto error_handler;
      }
      free(String);
      String = NULL;

      // The submodule target depends on the source file.
      (void)FDEP_AddDependencyToTarget(
          FDEP_SOURCE_NAME, (FDEP_ObjType)FDEP_OBJ_SOURCE, false,
          &((*TargetList)[TargetCount - 1]), &ErrorCode);
      if (ErrorCode != NO_ERROR) {
        goto error_handler;
      }
      // The submodule target depends on the ancestor (sub)module file.
      AncestorCount = IndexName - IndexKeyword - 1;
      if (AncestorCount > 1) {
        String = (char *)FDEP_ApiMalloc(
            sizeof(char) *
            (strlen(StatementList[i]->TokenList[IndexKeyword + 1]) +
             strlen(StatementList[i]->TokenList[IndexName - 1]) + 2));
        if (!String) {
          ErrorCode = ERROR_ALLOC;
          goto error_handler;
        }
        (void)strcpy(String, StatementList[i]->TokenList[IndexKeyword + 1]);
        (void)strcat(String, "@");
        (void)strcat(String, StatementList[i]->TokenList[IndexName - 1]);
      } else {
        String = (char *)FDEP_ApiMalloc(
            sizeof(char) *
            (strlen(StatementList[i]->TokenList[IndexKeyword + 1]) + 1));
        if (!String) {
          ErrorCode = ERROR_ALLOC;
          goto error_handler;
        }
        (void)strcpy(String, StatementList[i]->TokenList[IndexKeyword + 1]);
      }
      (void)FDEP_AddDependencyToTarget(String, (FDEP_ObjType)FDEP_OBJ_SUBMODULE,
                                       false, &((*TargetList)[TargetCount - 1]),
                                       &ErrorCode);
      if (ErrorCode != NO_ERROR) {
        goto error_handler;
      }
      // The object target depends on the ancestor (sub)module file.
      (void)FDEP_AddDependencyToTarget(String, (FDEP_ObjType)FDEP_OBJ_SUBMODULE,
                                       false, &((*TargetList)[0]), &ErrorCode);
      if (ErrorCode != NO_ERROR) {
        goto error_handler;
      }
      free(String);
      String = NULL;
      // The object target depends on the submodule file itself.
      (void)FDEP_AddDependencyToTarget((*TargetList)[TargetCount - 1]->Name,
                                       (FDEP_ObjType)FDEP_OBJ_SUBMODULE, false,
                                       &((*TargetList)[0]), &ErrorCode);
      if (ErrorCode != NO_ERROR) {
        goto error_handler;
      }
      continue;
    }

    UsesModule = FDEP_StatementContainsUsedModule(StatementList[i],
                                                  &IndexKeyword, &IndexName);
    if ((UsesModule) && (IndexName < StatementList[i]->TokenCount)) {

      // Current owner and object target get a module dependency.
      if (CurrentOwner != FDEP_OBJ_OBJECT) {
        (void)FDEP_AddDependencyToTarget(StatementList[i]->TokenList[IndexName],
                                         (FDEP_ObjType)FDEP_OBJ_MODULE, false,
                                         &((*TargetList)[TargetCount - 1]),
                                         &ErrorCode);
        if (ErrorCode != NO_ERROR) {
          goto error_handler;
        }
      }
      (void)FDEP_AddDependencyToTarget(StatementList[i]->TokenList[IndexName],
                                       (FDEP_ObjType)FDEP_OBJ_MODULE, false,
                                       &((*TargetList)[0]), &ErrorCode);
      if (ErrorCode != NO_ERROR) {
        goto error_handler;
      }
      continue;
    }
  }

  return TargetCount;
error_handler:
  free(String);
  FDEP_FreeTargetList(TargetList, TargetCount);
  if (FailByCaller) {
    *FailByCaller = ErrorCode;
    return 0;
  }
  FDEP_API_ERROR(ErrorCode);
  abort();
}

void FDEP_VerifyCompilationUnits(FDEP_Target ***TargetList,
                                 const size_t   TargetCount) {
  size_t i, j, k;
  bool   ModuleTarget, ModuleDependency;
  bool   SubModuleTarget, SubModuleDependency;

  if (!TargetList || !(*TargetList)) {
    return;
  }

  // For each dependency.
  for (i = 0; i < TargetCount; i++) {
    for (j = 0; j < (*TargetList)[i]->DependencyCount; j++) {
      // For each target.
      for (k = 0; k < TargetCount; k++) {
        if (0 == strcmp((*TargetList)[k]->Name,
                        (*TargetList)[i]->DependencyList[j]->Name)) {
          ModuleTarget =
              ((*TargetList)[k]->Type == (FDEP_ObjType)FDEP_OBJ_MODULE);
          ModuleDependency = ((*TargetList)[i]->DependencyList[j]->Type ==
                              (FDEP_ObjType)FDEP_OBJ_MODULE);
          SubModuleTarget =
              ((*TargetList)[k]->Type == (FDEP_ObjType)FDEP_OBJ_SUBMODULE);
          SubModuleDependency = ((*TargetList)[i]->DependencyList[j]->Type ==
                                 (FDEP_ObjType)FDEP_OBJ_SUBMODULE);

          if (ModuleTarget && ModuleDependency) {
            (*TargetList)[i]->DependencyList[j]->InCompilationUnit = true;
            break;
          }

          if (ModuleTarget && SubModuleDependency) {
            (*TargetList)[i]->DependencyList[j]->InCompilationUnit = true;
            break;
          }

          if (SubModuleTarget && SubModuleDependency) {
            (*TargetList)[i]->DependencyList[j]->InCompilationUnit = true;
            break;
          }
        }
      }
    }
  }
}
