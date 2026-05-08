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
    FDEP_AddDependencyToTarget("NewDep1", 1, false, &Target, &ErrorCode);
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
    FDEP_AddDependencyToTarget("NewDep2", 2, false, &Target, &ErrorCode);
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
    FDEP_AddDependencyToTarget("NewDep1", 1, false, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  Ran = false;
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(2 == Target->DependencyCount);
  // Add a dependency with the same characteristics as the target.
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    FDEP_AddDependencyToTarget("NewTarget", 0, false, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  Ran = false;
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(2 == Target->DependencyCount);
  FDEP_FreeTarget(&Target);
}

TEST_SUITE(DependencySuite,
           TestFDEP_DependencyNominal);

TEST_F(FDEP_Standard,
       TestFDEP_DependencyError) {
  (void)ContextData;
  bool             Ran        = false;
  FDEP_ErrorCode   ErrorCode  = NO_ERROR;
  FDEP_Dependency *Dependency = NULL;
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran        = true;
    Dependency = FDEP_NewDependency("a", 1, false, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  ASSERT(!Dependency);
  Ran = false;
  FDEP_SetMallocFailCfg(2);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran        = true;
    Dependency = FDEP_NewDependency("a", 1, false, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  ASSERT(!Dependency);
  Ran = false;
  FDEP_SetMallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran        = true;
    Dependency = FDEP_NewDependency("a", 1, false, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 1);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[0] == ERROR_ALLOC);
  Ran = false;
}

TEST_F(FDEP_Standard,
       TestFDEP_TargetError) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  FDEP_Target   *Target    = NULL;
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran    = true;
    Target = FDEP_NewTarget("a", 1, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  ASSERT(!Target);
  Ran = false;
  FDEP_SetMallocFailCfg(2);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran    = true;
    Target = FDEP_NewTarget("a", 1, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  ASSERT(!Target);
  Ran = false;
  FDEP_SetMallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran    = true;
    Target = FDEP_NewTarget("a", 1, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 1);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[0] == ERROR_ALLOC);
  Ran = false;
}

TEST_F(FDEP_Standard,
       TestFDEP_AddDependencyToTargetError1) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  FDEP_Target   *Target    = NULL;
  bool           DepAdded  = false;
  // Input errors.
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    (void)FDEP_AddDependencyToTarget("b", 2, false, NULL, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_INPUT);
  Ran = false;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    (void)FDEP_AddDependencyToTarget("b", 2, false, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_INPUT);
  ASSERT(!Target);
  Ran = false;
  FDEP_FreeTarget(NULL);
  FDEP_FreeTarget(&Target);
  // Create a target.
  ErrorCode = NO_ERROR;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran    = true;
    Target = FDEP_NewTarget("a", 1, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(Target);
  // Add legitimate dependency.
  Ran = false;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran      = true;
    DepAdded = FDEP_AddDependencyToTarget("b", 2, false, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(DepAdded);
  // Do not add dependency (same as target).
  Ran = false;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran      = true;
    DepAdded = FDEP_AddDependencyToTarget("a", 1, false, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(!DepAdded);
  // Fail while adding new dependency.
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran      = true;
    DepAdded = FDEP_AddDependencyToTarget("c", 3, false, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  ASSERT(!DepAdded);
}

TEST_F(FDEP_Standard,
       TestFDEP_AddDependencyToTargetError2) {
  (void)ContextData;
  bool           Ran       = false;
  FDEP_ErrorCode ErrorCode = NO_ERROR;
  FDEP_Target   *Target    = NULL;
  bool           DepAdded  = false;
  // Create a target.
  ErrorCode = NO_ERROR;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran    = true;
    Target = FDEP_NewTarget("a", 1, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(Target);
  // Add legitimate dependency.
  Ran = false;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran      = true;
    DepAdded = FDEP_AddDependencyToTarget("b", 2, false, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  ASSERT(DepAdded);
  // Fail while adding new dependency.
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran      = true;
    DepAdded = FDEP_AddDependencyToTarget("c", 3, false, &Target, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  ASSERT(!DepAdded);
}

TEST_F(FDEP_Standard,
       TestFDEP_AddDependencyToTargetError3) {
  (void)ContextData;
  bool         Ran    = false;
  FDEP_Target *Target = NULL;
  // Create a target.
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran    = true;
    Target = FDEP_NewTarget("a", 1, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
  ASSERT(Target);
  // Fail while adding new dependency.
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran = true;
    (void)FDEP_AddDependencyToTarget("c", 3, false, &Target, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 1);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[0] == ERROR_ALLOC);
}

TEST_F(FDEP_Standard,
       TestFDEP_FreeTargetListNull) {
  (void)ContextData;
  FDEP_Target **TargetList = NULL;
  FDEP_FreeTargetList(NULL, 0);
  FDEP_FreeTargetList(&TargetList, 0);
}

TEST_SUITE(DependencyErrorSuite,
           TestFDEP_DependencyError,
           TestFDEP_TargetError,
           TestFDEP_AddDependencyToTargetError1,
           TestFDEP_AddDependencyToTargetError2,
           TestFDEP_AddDependencyToTargetError3,
           TestFDEP_FreeTargetListNull);
