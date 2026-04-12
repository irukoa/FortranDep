################################################################################
# Static analysis part.
# Assumes that the source files are listed in the variable SRCS.
# Assumes that the variable INC holds all neccesary include locations.
################################################################################

.PHONY: $(STAGOALS)

#Static analysis: common infraestructure.
STADIR    := STA
STARPTS   := $(addsuffix .srp, $(addprefix $(STADIR)/, $(basename $(SRCS))))
STATOOL   := clang-tidy
STACHECKS := --checks='modernize*,clang-diagnostic-*,clang-analyzer-*'

sta: $(STARPTS)

#Static analysis: dependency graph inherited from variable SRCS.
$(STADIR)/%.srp: %.c | $(STADIR)
		mkdir -p $(dir $@)
		$(STATOOL) $(STACHECKS) $< -- $(INC) > $@

$(STADIR):
		$(MKDIR) $@

clean::
		$(RM) $(STADIR)
