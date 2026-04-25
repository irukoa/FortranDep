#Header.
MKDIR := mkdir -p
CP    := cp
MV    := mv
RM    := rm -rf

#Compiler, flags, definitions and linker options.
FC        := gfortran
DEPSCRIPT := ./tools/GetFDependencies.sh
CFLAGS :=

#Rules
.PHONY: main clean
.DEFAULT_GOAL := main
ifeq ($(MAKECMDGOALS), )
override MAKECMDGOALS := main
endif

#Environment.
OBJ := obj
BIN := bin
EXE := $(BIN)/ExampleProgram.x

#Dependency graph.
#	-Source code.
SRC  := src
SRCS := $(wildcard $(SRC)/*.F90)
#	-Include directories.
INC := -I.
# -Objects.
OBJS := $(patsubst $(SRC)/%.F90,$(OBJ)/%.o,$(SRCS))

TRASH := trash

# -Dependency inclusion.
DEPS := $(patsubst $(OBJ)/%.o,$(OBJ)/%.d,$(OBJS))
ifneq ($(filter-out clean, $(MAKECMDGOALS)), )
-include $(DEPS)
-include $(OBJ)/main.d
endif

$(OBJ)/%.o: $(SRC)/%.F90 | $(OBJ)
		$(FC) $(CFLAGS) -J$(TRASH) -I$(OBJ) $(INC) -c $< -o $@
$(OBJ)/%.mod: $(SRC)/%.F90 | $(OBJ)
		$(RM) $@
		-$(FC) $(CFLAGS) -fsyntax-only -J$(OBJ) $(INC) -c $<
$(OBJ)/%.smod: $(SRC)/%.F90 | $(OBJ)
		$(RM) $@
		-$(FC) $(CFLAGS) -fsyntax-only -J$(OBJ) $(INC) -c $<
$(OBJ)/%.d: $(SRC)/%.F90 | $(OBJ)
		$(DEPSCRIPT) $< $(OBJ) > $@ || $(RM) $@

main: $(EXE)
$(OBJ)/main.o: main.F90 | $(OBJ)
		$(FC) $(CFLAGS) -J$(TRASH) -I$(OBJ) $(INC) -c $< -o $@
$(OBJ)/main.d: main.F90 | $(OBJ)
		$(DEPSCRIPT) $< $(OBJ) > $@ || $(RM) $@
$(EXE): $(OBJ)/main.o $(OBJS) | $(BIN)
		$(FC) $(CFLAGS) $(INC) -J$(TRASH) -I$(OBJ) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ) $(BIN):
		$(MKDIR) $@

clean::
		$(RM) $(OBJ) $(BIN)
