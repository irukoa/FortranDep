#include "src/DEP/Dependency.h"

void FDEP_FreeDependency(FDEP_Dependency **Dependency) {

  if (!(Dependency) || !(*Dependency)) {
    return;
  }

  free((*Dependency)->Name);
  (*Dependency)->Name = NULL;
  free(*Dependency);
  *Dependency = NULL;
}

FDEP_Dependency *FDEP_NewDependency(const char *const  Name,
                                    const FDEP_ObjType Type,
                                    FDEP_ErrorCode    *FailByCaller) {
  FDEP_ErrorCode   ErrorCode       = NO_ERROR;
  bool             ParentAllocated = false;
  FDEP_Dependency *Dependency      = NULL;

  Dependency = (FDEP_Dependency *)FDEP_ApiMalloc(sizeof(FDEP_Dependency));
  if (!Dependency) {
    ErrorCode = ERROR_ALLOC;
    goto error_handler;
  }

  ParentAllocated  = true;
  Dependency->Name = NULL;
  Dependency->Name = (char *)FDEP_ApiMalloc(sizeof(char) * (strlen(Name) + 1));
  if (!(Dependency->Name)) {
    ErrorCode = ERROR_ALLOC;
    goto error_handler;
  }
  (void)strcpy(Dependency->Name, Name);
  Dependency->Type = Type;

  return Dependency;
error_handler:
  if (ParentAllocated)
    free(Dependency->Name);
  free(Dependency);
  if (FailByCaller) {
    *FailByCaller = ErrorCode;
    return NULL;
  }
  FDEP_API_ERROR(ErrorCode);
  return NULL;
}

void FDEP_FreeTarget(FDEP_Target **Target) {
  size_t i;

  if (!Target || !(*Target)) {
    return;
  }

  free((*Target)->Name);
  (*Target)->Name = NULL;
  for (i = 0; i < (*Target)->DependencyCount; i++) {
    FDEP_FreeDependency(&((*Target)->DependencyList[i]));
  }
  free((*Target)->DependencyList);
  (*Target)->DependencyList = NULL;
  free((*Target));
  (*Target) = NULL;
}

FDEP_Target *FDEP_NewTarget(const char *const  Name,
                            const FDEP_ObjType Type,
                            FDEP_ErrorCode    *FailByCaller) {
  FDEP_ErrorCode ErrorCode       = NO_ERROR;
  bool           ParentAllocated = false;
  FDEP_Target   *Target          = NULL;

  Target = (FDEP_Target *)FDEP_ApiMalloc(sizeof(FDEP_Target));
  if (!Target) {
    ErrorCode = ERROR_ALLOC;
    goto error_handler;
  }

  ParentAllocated = true;
  Target->Name    = NULL;
  Target->Name    = (char *)FDEP_ApiMalloc(sizeof(char) * (strlen(Name) + 1));
  if (!(Target->Name)) {
    ErrorCode = ERROR_ALLOC;
    goto error_handler;
  }
  (void)strcpy(Target->Name, Name);
  Target->Type            = Type;
  Target->DependencyCount = 0;
  Target->DependencyList  = NULL;
  return Target;
error_handler:
  if (ParentAllocated)
    free(Target->Name);
  free(Target);
  if (FailByCaller) {
    *FailByCaller = ErrorCode;
    return NULL;
  }
  FDEP_API_ERROR(ErrorCode);
  return NULL;
}

bool FDEP_AddDependencyToTarget(const char *const  Name,
                                const FDEP_ObjType Type,
                                FDEP_Target      **Target,
                                FDEP_ErrorCode    *FailByCaller) {
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  size_t         NewDependencyCount;
  void          *TmpDependencyList = NULL;
  size_t         i;

  if (!(Target) || !(*Target)) {
    ErrorCode = ERROR_INPUT;
    goto error_handler;
  }

  if ((0 == strcmp(Name, (*Target)->Name)) && (Type == (*Target)->Type)) {
    return false;
  }

  for (i = 0; i < (*Target)->DependencyCount; i++) {
    if ((0 == strcmp(Name, (*Target)->DependencyList[i]->Name)) &&
        (Type == (*Target)->DependencyList[i]->Type)) {
      return false;
    }
  }

  NewDependencyCount = (*Target)->DependencyCount + 1;
  TmpDependencyList =
      FDEP_ApiRealloc((void *)(*Target)->DependencyList,
                      NewDependencyCount * sizeof(FDEP_Dependency *));
  if (!TmpDependencyList) {
    ErrorCode = ERROR_ALLOC;
    goto error_handler;
  }
  (*Target)->DependencyList  = (FDEP_Dependency **)TmpDependencyList;
  (*Target)->DependencyCount = NewDependencyCount;
  (*Target)->DependencyList[(*Target)->DependencyCount - 1] =
      FDEP_NewDependency(Name, Type, &ErrorCode);
  if (ErrorCode != NO_ERROR) {
    goto error_handler;
  }
  return true;
error_handler:
  FDEP_FreeTarget(Target);
  if (FailByCaller) {
    *FailByCaller = ErrorCode;
    return false;
  }
  FDEP_API_ERROR(ErrorCode);
  return false;
}

void FDEP_FreeTargetList(FDEP_Target ***TargetList,
                         const size_t   TargetCount) {
  size_t i;

  if (!TargetList || !(*TargetList)) {
    return;
  }

  for (i = 0; i < TargetCount; i++) {
    FDEP_FreeTarget(&((*TargetList)[i]));
  }
  free(*TargetList);
  *TargetList = NULL;
}
