#include "test/suites/Stub/StandardContext.h"

#include "src/DEP/Dependency.h"

TEST_F(FDEP_Standard,
       TestFDEP_DependencyNominal) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  FDEP_Target   *Target    = NULL;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran    = true;
    Target = FDEP_NewTarget("NewTarget", 0, &ErrorCode);
  }
  ASSERT_X(Ran);
  Ran = false;
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(0 == strcmp(Target->Name, "NewTarget"));
  ASSERT(0 == Target->Type);
  ASSERT(0 == Target->DependencyCount);
  ASSERT(NULL == Target->DependencyList);
  // Add a dependency.
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    FDEP_AddDependencyToTarget("NewDep1", 1, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  Ran = false;
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(1 == Target->DependencyCount);
  ASSERT(NULL != Target->DependencyList);
  ASSERT(0 == strcmp(Target->DependencyList[0]->Name, "NewDep1"));
  ASSERT(1 == Target->DependencyList[0]->Type);
  // Add another dependency.
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    FDEP_AddDependencyToTarget("NewDep2", 2, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  Ran = false;
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(2 == Target->DependencyCount);
  ASSERT(0 == strcmp(Target->DependencyList[1]->Name, "NewDep2"));
  ASSERT(2 == Target->DependencyList[1]->Type);
  // Add an already present dependency.
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    FDEP_AddDependencyToTarget("NewDep1", 1, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  Ran = false;
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(2 == Target->DependencyCount);
  // Add a dependency with the same characteristics as the target.
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    FDEP_AddDependencyToTarget("NewTarget", 0, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  Ran = false;
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(2 == Target->DependencyCount);
  FDEP_FreeTarget(&Target);
}

TEST_SUITE(DependencySuite,
           TestFDEP_DependencyNominal);
