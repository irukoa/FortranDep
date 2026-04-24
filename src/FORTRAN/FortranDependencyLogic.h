#ifndef _FORTRAN_DEPENDENCY_LOGIC_H
#define _FORTRAN_DEPENDENCY_LOGIC_H

#include "src/API/API.h"
#include "src/DEP/Dependency.h"
#include "src/FORTRAN/FortranLexer.h"
#include "src/TOK/Tokenizer.h"

typedef enum {
  FDEP_OBJ_OBJECT,
  FDEP_OBJ_SOURCE,
  FDEP_OBJ_MODULE,
  FDEP_OBJ_SUBMODULE
} FDEP_FortranObjType;

size_t FDEP_StatementListIntoDependencyTree(
    FDEP_Target ***TargetList, // Reference to array of pointers to allocated
                               // FDEP_Target objects.
    const FDEP_Statement *const
        *const   StatementList, // Array of allocated FDEP_Statement pointers.
    const size_t StatementCount,
    FDEP_ErrorCode *FailByCaller);

#endif
