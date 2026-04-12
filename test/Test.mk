################################################################################
# Testing profile part.
# Assumes that the source object files are listed in the variable OBJS.
# Assumes that all source object files lie in the directory OBJ.
# Assumes that the source object files have been compiled using
#   GCC's --coverage flag or other alternative which generates
#   .gcda-s and .gcno-s.
# Assumes that `__attribute__((constructor))` is available.
################################################################################

#Arguments to the test runner.
TARGS :=

.PHONY: $(TESTGOALS)

#Testing: common infraestructure.
TESTEXE := test/Run_Tests.x
TESTOBJ := test/obj
TESTREP := test/reports

TESTCOMMONSRC  := test/common
TESTCOMMONOBJ  := test/commonobj
TESTCOMMONSRCS := $(wildcard $(TESTCOMMONSRC)/*.c)
TESTCOMMONOBJS := $(patsubst $(TESTCOMMONSRC)/%.c,$(TESTCOMMONOBJ)/%.o,$(TESTCOMMONSRCS))

#Linker options.
LDTESTFLAGS :=
LDTESTLIBS  :=

#Common objects.
$(TESTCOMMONOBJ)/%.o: $(TESTCOMMONSRC)/%.c | $(TESTCOMMONOBJ)
		$(CC) $(CFLAGS) $(TESTINC) $(DEFS) -c $< -o $@
$(TESTCOMMONOBJ)/%.d: $(TESTCOMMONSRC)/%.c | $(TESTCOMMONOBJ)
		$(DEPSCRIPT) $< $(TESTCOMMONOBJ) > $@ || $(RM) $@
TESTCOMMONDEPS := $(patsubst $(TESTCOMMONOBJ)/%.o,$(TESTCOMMONOBJ)/%.d,$(TESTCOMMONOBJS))
ifneq ($(filter-out clean, $(MAKECMDGOALS)), )
-include $(TESTCOMMONDEPS)
endif

#Testing: dependency graph.
#	-Source code (unit tests).
TESTSRCS :=
#	-Include directories.
TESTINC := -I$(TESTCOMMONSRC)
# -Objects.
TESTOBJS :=
# -Graph creation is delegated to modules in registry.
-include test/ModuleRegistry.mkreg
# -Dependency inclusion.
TESTDEPS := $(patsubst $(TESTOBJ)/%.o,$(TESTOBJ)/%.d,$(TESTOBJS))
ifneq ($(filter-out clean, $(MAKECMDGOALS)), )
-include $(TESTDEPS)
endif

test: $(TESTEXE)
$(TESTEXE): $(OBJS) $(TESTCOMMONOBJS) $(TESTOBJS)
		$(CC) $(CFLAGS) $(TESTINC) $(DEFS) $^ -o $@ $(LDTESTFLAGS) $(LDTESTLIBS)

runtest: $(TESTEXE) | $(TESTREP)
		$(RM) $(TESTREP)/*
		$(RM) $(OBJ)/*.gcda
		./$(TESTEXE) $(TARGS) 2>&1 | tee $(TESTREP)/TestRun.log
		lcov --capture --directory $(OBJ) --output-file=test/coverage.info
		genhtml test/coverage.info --output-directory=$(TESTREP)
		$(MV) test/coverage.info $(TESTREP)
		valgrind --leak-check=full --show-leak-kinds=all ./$(TESTEXE) $(TARGS) 2>&1 | tee $(TESTREP)/ValgrindTestRun.log

$(TESTOBJ) $(TESTCOMMONOBJ) $(TESTREP):
		$(MKDIR) $@

clean::
		$(RM) $(TESTOBJ) $(TESTCOMMONOBJ)
		$(RM) $(TESTREP)
		$(RM) $(TESTEXE)
