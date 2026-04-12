#Header.
MKDIR := mkdir -p
CP    := cp
MV    := mv
RM    := rm -rf

#Profile.
PROFILE := debug

#Compiler, flags, definitions and linker options.
CC        := gcc
DEPSCRIPT := ./tools/GetDependencies.sh
ifeq ($(PROFILE), debug)
CFLAGS := -g -Wall -O0 -Wextra -pedantic -std=gnu11 --coverage
DEFS   := -DTESTABLE
else
CFLAGS := -Werror -O3 -std=gnu11
DEFS   := -DNDEBUG
endif
LDFLAGS :=
LDLIBS  :=

#Rules
.PHONY: main clean
.DEFAULT_GOAL := main
ifeq ($(MAKECMDGOALS), )
override MAKECMDGOALS := main
endif

#Environment.
OBJ := obj
BIN := bin
EXE := $(BIN)/FortranDepCore.x

#Dependency graph.
#	-Source code.
SRCS :=
#	-Include directories.
INC := -I./
# -Objects.
OBJS :=
# -Graph creation is delegated to modules in registry.
-include ModuleRegistry.mkreg
# -Dependency inclusion.
DEPS := $(patsubst $(OBJ)/%.o,$(OBJ)/%.d,$(OBJS))
ifneq ($(filter-out clean, $(MAKECMDGOALS)), )
-include $(DEPS)
endif

#Rule for the executable.
main: $(EXE)
$(OBJ)/main.o: main.c | $(OBJ)
		$(CC) $(CFLAGS) $(INC) $(DEFS) -c $< -o $@
$(OBJ)/main.d: main.c | $(OBJ)
		$(DEPSCRIPT) $< $(OBJ) > $@ || $(RM) $@
ifneq ($(filter-out clean, $(MAKECMDGOALS)), )
-include $(OBJ)/main.d
endif
$(EXE): $(OBJ)/main.o $(OBJS) | $(BIN)
		$(CC) $(CFLAGS) $(INC) $(DEFS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

ifeq ($(PROFILE), debug)
#Testing profile.
TESTGOALS := test runtest clean
ifneq ($(filter $(TESTGOALS), $(MAKECMDGOALS)), )
-include test/Test.mk
endif

#Static analysis profile.
STAGOALS := sta clean
ifeq ($(filter-out $(STAGOALS), $(MAKECMDGOALS)), )
-include STA.mk
endif
endif

$(OBJ) $(BIN):
		$(MKDIR) $@

clean::
		$(RM) $(OBJ) $(BIN)
