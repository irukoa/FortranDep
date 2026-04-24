#include "test/suites/Stub/StandardContext.h"

#include "src/FORTRAN/FortranDependencyLogic.h"
#include "src/FORTRAN/FortranSyntax.h"
#include "src/TOK/Tokenizer.h"

static FILE *PutInFakeStream(const char *const Contents) {
  FILE *Stream = fmemopen((void *)Contents, strlen(Contents), "r");
  ASSERT_X(Stream);
  return Stream;
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeFunctional1) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "module a\n"
                                         "  use b\n"
                                         "end module a\n";
  Data->FakeStream                     = PutInFakeStream(Contents);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran            = true;
    StatementCount = FDEP_TokenizeStream(
        &StatementList, FDEP_FORTRAN_DELIMITERS, FDEP_FORTRAN_CONTINUATION,
        FDEP_FORTRAN_SEPARATOR, Data->FakeStream, FDEP_FortranPreprocess,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_ObjectName));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SourceName));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "a"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "b"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "a"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SourceName));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "b"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_SUITE(FortranDependencyLogicSuite,
           TestFDEP_StatementListIntoDependencyTreeFunctional1);
