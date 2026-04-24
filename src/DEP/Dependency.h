#ifndef _DEPENDENCY_H
#define _DEPENDENCY_H

#include "src/API/API.h"

typedef int FDEP_ObjType;

typedef struct _FDEP_Dependency {
  char        *Name;
  FDEP_ObjType Type;
} FDEP_Dependency;

INTDEC(void,
       FDEP_FreeDependency,
       FDEP_Dependency **Dependency); // Reference to pointer of allocated
                                      // FDEP_Dependency object.

INTDEC(FDEP_Dependency *, // Pointer to allocated FDEP_Dependency object.
       FDEP_NewDependency,
       const char *const  Name,
       const FDEP_ObjType Type,
       FDEP_ErrorCode    *FailByCaller);

typedef struct _FDEP_Target {
  char             *Name;
  FDEP_ObjType      Type;
  size_t            DependencyCount;
  FDEP_Dependency **DependencyList; // Array of FDEP_Dependency pointers.
} FDEP_Target;

/* Frees a target previously allocated by FDEP_NewTarget. */
void FDEP_FreeTarget(FDEP_Target **Target); // Reference to pointer of allocated
                                            // FDEP_Target object.

/* Returns an allocated pointer to a target type without dependencies
 (DependencyCount = 0, DependencyList = NULL).
 On failure:
 if FailByCaller is NULL, calls FDEP_API_ERROR;
 otherwise, writes the error code in the value of FailByCaller and returns NULL.
 Any memory allocated by the function is freed.
*/
FDEP_Target *FDEP_NewTarget( // Pointer to allocated FDEP_Target object.
    const char *const  Name,
    const FDEP_ObjType Type,
    FDEP_ErrorCode    *FailByCaller);

/* Adds a new dependency to a target allocated by FDEP_NewTarget. If the
 dependency was added, returns true and false otherwise.
 On failure:
 if FailByCaller is NULL, calls FDEP_API_ERROR;
 otherwise, writes the error code in the value of FailByCaller and returns
 false.
 Any memory allocated by the function is freed.
*/
bool FDEP_AddDependencyToTarget(
    const char *const  Name,
    const FDEP_ObjType Type,
    FDEP_Target *
        *Target, // Reference to pointer of allocated FDEP_Target object.
    FDEP_ErrorCode *FailByCaller);

void FDEP_FreeTargetList(
    FDEP_Target ***TargetList, // Reference to array of pointers to allocated
                               // FDEP_Target objects.
    const size_t TargetCount);

#endif
