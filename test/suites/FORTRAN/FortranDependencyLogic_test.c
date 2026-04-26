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
       TestFDEP_StatementListIntoDependencyTreeError1) {
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
  // Input errors.
  Ran = false;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        NULL, (const FDEP_Statement *const *const)StatementList, StatementCount,
        &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_INPUT);
  ASSERT(TargetCount == 0);
  Ran = false;
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(&TargetList, NULL,
                                                       StatementCount, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 1);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[0] == ERROR_INPUT);
  // First target error.
  Ran = false;
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_SetReallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, NULL);
  }
  ASSERT_X(Ran);
  ASSERT(FDEP_ApiError_Mock_fake.call_count == 2);
  ASSERT(FDEP_ApiError_Mock_fake.arg0_history[1] == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError2) {
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
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(1);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError3) {
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
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(2);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError4) {
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
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(3);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError5) {
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
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(5);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError6) {
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
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(4);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError7) {
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
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(5);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError8) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "  use b\n";
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
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(3);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError9) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "module a\n"
                                         "  use b\n";
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
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(7);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError10) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "submodule(a:b) c\n";
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
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(3);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError11) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "submodule(a:b) c\n";
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
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(5);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError12) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "submodule(a:b) c\n";
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
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(6);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError13) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "submodule(a:b) c\n";
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
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(4);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError14) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "submodule(a:b) c\n";
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
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(10);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError15) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "submodule(a) b\n";
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
  FDEP_ResetMallocFailCfg();
  FDEP_SetMallocFailCfg(10);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError16) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "submodule(a:b) c\n";
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
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(5);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_F(FDEP_Standard,
       TestFDEP_StatementListIntoDependencyTreeError17) {
  FDEP_StandardContextData *Data      = (FDEP_StandardContextData *)ContextData;
  bool                      Ran       = false;
  FDEP_ErrorCode            ErrorCode = NO_ERROR;
  FDEP_Statement          **StatementList;
  size_t                    StatementCount;
  FDEP_Target             **TargetList;
  size_t                    TargetCount;
  const char                Contents[] = "submodule(a:b) c\n";
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
  FDEP_ResetReallocFailCfg();
  FDEP_SetReallocFailCfg(7);
  if (setjmp(TSD_GlobJumpRef) == 0) {
    Ran         = true;
    TargetCount = FDEP_StatementListIntoDependencyTree(
        &TargetList, (const FDEP_Statement *const *const)StatementList,
        StatementCount, &ErrorCode);
  }
  ASSERT_X(Ran);
  ASSERT(TargetCount == 0);
  ASSERT(ErrorCode == ERROR_ALLOC);
  FDEP_FreeStatementList(&StatementList, StatementCount);
}

TEST_SUITE(FortranDependencyLogicErrorSuite,
           TestFDEP_StatementListIntoDependencyTreeError1,
           TestFDEP_StatementListIntoDependencyTreeError2,
           TestFDEP_StatementListIntoDependencyTreeError3,
           TestFDEP_StatementListIntoDependencyTreeError4,
           TestFDEP_StatementListIntoDependencyTreeError5,
           TestFDEP_StatementListIntoDependencyTreeError6,
           TestFDEP_StatementListIntoDependencyTreeError7,
           TestFDEP_StatementListIntoDependencyTreeError8,
           TestFDEP_StatementListIntoDependencyTreeError9,
           TestFDEP_StatementListIntoDependencyTreeError10,
           TestFDEP_StatementListIntoDependencyTreeError11,
           TestFDEP_StatementListIntoDependencyTreeError12,
           TestFDEP_StatementListIntoDependencyTreeError13,
           TestFDEP_StatementListIntoDependencyTreeError14,
           TestFDEP_StatementListIntoDependencyTreeError15,
           TestFDEP_StatementListIntoDependencyTreeError16,
           TestFDEP_StatementListIntoDependencyTreeError17);
