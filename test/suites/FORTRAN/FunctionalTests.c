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
       Functional1) {
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "a"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "b"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "a"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "b"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional2) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "submodule(a) b\n"
                                         "  use k\n"
                                         "end submodule b\n";
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 4);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "a"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "a@b"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[3]->Name, "k"));
  ASSERT(TargetList[0]->DependencyList[3]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "a@b"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(TargetList[1]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "a"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[2]->Name, "k"));
  ASSERT(TargetList[1]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional3) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "submodule(a:b) c\n"
                                         "  use k\n"
                                         "end submodule c\n";
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 4);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "a@b"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "a@c"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[3]->Name, "k"));
  ASSERT(TargetList[0]->DependencyList[3]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "a@c"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(TargetList[1]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "a@b"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[2]->Name, "k"));
  ASSERT(TargetList[1]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional4) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "module a\n"
                                         "  use k\n"
                                         "end module a\n"
                                         "module b\n"
                                         "  use a\n"
                                         "  use j\n"
                                         "end module b\n";
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 5);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "a"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "k"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[3]->Name, "b"));
  ASSERT(TargetList[0]->DependencyList[3]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[4]->Name, "j"));
  ASSERT(TargetList[0]->DependencyList[4]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "a"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "k"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[2]->Name, "b"));
  ASSERT(TargetList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[2]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[2]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[2]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[2]->DependencyList[1]->Name, "a"));
  ASSERT(TargetList[2]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[2]->DependencyList[2]->Name, "j"));
  ASSERT(TargetList[2]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional5) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "module c; use x; end module c\n";
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "c"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "x"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "c"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "x"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional6) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "module &\n"
                                         "  d\n"
                                         "  use &\n"
                                         "  & y\n"
                                         "end module d\n";
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "d"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "y"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "d"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "y"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional7) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] =
      "module e\n"
      "  use string_mod ! comment with ; & ! \"\n"
      "  character(len=20) :: str = \"this is ; a string &\n"
      "  & with tricky chars\"\n"
      "  use z\n"
      "end module e\n";
  Data->FakeStream = PutInFakeStream(Contents);
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 4);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "e"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "string_mod"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[3]->Name, "z"));
  ASSERT(TargetList[0]->DependencyList[3]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "e"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "string_mod"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[2]->Name, "z"));
  ASSERT(TargetList[1]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional8) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "MoDuLe mIxEd_cAsE\n"
                                         "  UsE mOdUlE_B\n"
                                         "EnD mOdUlE mIxEd_cAsE\n";
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "mixed_case"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "module_b"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "mixed_case"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "module_b"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional9) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char Contents[] = "module f & ! this is a comment after continuation\n"
                          "  & \n"
                          "  use y\n"
                          "end module f\n";
  Data->FakeStream      = PutInFakeStream(Contents);
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "f"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "y"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "f"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "y"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional10) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char Contents[] = "module g ; ; ; use a ; ;; ; end module g\n";
  Data->FakeStream      = PutInFakeStream(Contents);
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "g"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "a"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "g"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "a"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional11) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "module h\n"
                                         "  character(len=*) :: str = \"&\"\n"
                                         "  use b\n"
                                         "end module h\n";
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "h"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "b"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "h"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "b"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional12) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "character(len=:) :: str =\"aa&\n"
                                         "&bb\"; module b; use k; end &\n"
                                         "module b\n";
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "b"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "k"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "b"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "k"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional13) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "submodule &\n"
                                         "  ( &\n"
                                         "  a: &\n"
                                         "  b &\n"
                                         "  ) &\n"
                                         "  c\n"
                                         "  use k\n"
                                         "end submodule c\n";
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 4);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "a@b"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "a@c"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[3]->Name, "k"));
  ASSERT(TargetList[0]->DependencyList[3]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "a@c"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(TargetList[1]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "a@b"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[2]->Name, "k"));
  ASSERT(TargetList[1]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional14) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] =
      "module str_test\n"
      "  character(len=*) :: s1 = \"This is a 'string' ; use fake\"\n"
      "  character(len=*) :: s2 = 'This is a \"string\" ; use fake2'\n"
      "  use real_module\n"
      "end module str_test\n";
  Data->FakeStream = PutInFakeStream(Contents);
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "str_test"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "real_module"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "str_test"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "real_module"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional15) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "! empty line comment\n"
                                         "; ; ; ;\n"
                                         "module empty_test ; ; ! some comment\n"
                                         "  \n"
                                         "   use something\n"
                                         "end module empty_test\n";
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "empty_test"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "something"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "empty_test"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "something"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional16) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "module a ! \"\n"
                                         "str :: \"my&\n"
                                         "&lo!ng&\n"
                                         "string\" !\n"
                                         "   use k\n"
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
        StatementCount, true, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "a"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[2]->Name, "k"));
  ASSERT(TargetList[0]->DependencyList[2]->Type == FDEP_OBJ_MODULE);
  ASSERT(0 == strcmp(TargetList[1]->Name, "a"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "k"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);
  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional17) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] =
      "module a; end; submodule (a) b; end; submodule (c) d; end\n";
  Data->FakeStream = PutInFakeStream(Contents);
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
        StatementCount, false, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 4);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "c"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_SUBMODULE);

  ASSERT(0 == strcmp(TargetList[1]->Name, "a"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 1);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);

  ASSERT(0 == strcmp(TargetList[2]->Name, "a@b"));
  ASSERT(TargetList[2]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(TargetList[2]->DependencyCount == 1);
  ASSERT(0 == strcmp(TargetList[2]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[2]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);

  ASSERT(0 == strcmp(TargetList[3]->Name, "c@d"));
  ASSERT(TargetList[3]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(TargetList[3]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[3]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[3]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[3]->DependencyList[1]->Name, "c"));
  ASSERT(TargetList[3]->DependencyList[1]->Type == FDEP_OBJ_SUBMODULE);

  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional18) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char Contents[] = "submodule (a) b; end; submodule (a:b) c; end\n";
  Data->FakeStream      = PutInFakeStream(Contents);
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
        StatementCount, false, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 3);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "a"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_SUBMODULE);

  ASSERT(0 == strcmp(TargetList[1]->Name, "a@b"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "a"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_SUBMODULE);

  ASSERT(0 == strcmp(TargetList[2]->Name, "a@c"));
  ASSERT(TargetList[2]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(TargetList[2]->DependencyCount == 1);
  ASSERT(0 == strcmp(TargetList[2]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[2]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);

  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_F(FDEP_Standard,
       Functional19) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "      module a\n"
                                         "  use b\n"
                                         "  ![...]\n"
                                         "end\n"
                                         "\n"
                                         "submodule(a) c\n"
                                         "  use d\n"
                                         "  ![...]\n"
                                         "end\n"
                                         "\n"
                                         "submodule(e) f\n"
                                         "  use a\n"
                                         "  ![...]\n"
                                         "end\n";
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
        StatementCount, false, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == NO_ERROR);
  Ran = false;
  ASSERT(TargetCount == 4);
  ASSERT(0 == strcmp(TargetList[0]->Name, FDEP_OBJECT_NAME));
  ASSERT(TargetList[0]->Type == FDEP_OBJ_OBJECT);
  ASSERT(TargetList[0]->DependencyCount == 4);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[0]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[0]->DependencyList[1]->Name, "b"));
  ASSERT(TargetList[0]->DependencyList[1]->Type == FDEP_OBJ_MODULE);

  ASSERT(0 == strcmp(TargetList[1]->Name, "a"));
  ASSERT(TargetList[1]->Type == FDEP_OBJ_MODULE);
  ASSERT(TargetList[1]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[1]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[1]->DependencyList[1]->Name, "b"));
  ASSERT(TargetList[1]->DependencyList[1]->Type == FDEP_OBJ_MODULE);

  ASSERT(0 == strcmp(TargetList[2]->Name, "a@c"));
  ASSERT(TargetList[2]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(TargetList[2]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[2]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[2]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[2]->DependencyList[1]->Name, "d"));
  ASSERT(TargetList[2]->DependencyList[1]->Type == FDEP_OBJ_MODULE);

  ASSERT(0 == strcmp(TargetList[3]->Name, "e@f"));
  ASSERT(TargetList[3]->Type == FDEP_OBJ_SUBMODULE);
  ASSERT(TargetList[3]->DependencyCount == 2);
  ASSERT(0 == strcmp(TargetList[3]->DependencyList[0]->Name, FDEP_SOURCE_NAME));
  ASSERT(TargetList[3]->DependencyList[0]->Type == FDEP_OBJ_SOURCE);
  ASSERT(0 == strcmp(TargetList[3]->DependencyList[1]->Name, "e"));
  ASSERT(TargetList[3]->DependencyList[1]->Type == FDEP_OBJ_SUBMODULE);

  FDEP_FreeTargetList(&TargetList, TargetCount);
  FDEP_FreeStatementList(&StatementList, StatementCount);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 0);
}

TEST_SUITE(FunctionalSuite,
           Functional1,
           Functional2,
           Functional3,
           Functional4,
           Functional5,
           Functional6,
           Functional7,
           Functional8,
           Functional9,
           Functional10,
           Functional11,
           Functional12,
           Functional13,
           Functional14,
           Functional15,
           Functional16,
           Functional17,
           Functional18,
           Functional19);
