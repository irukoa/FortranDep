DIR      := test/suites/Stub
TESTSRCS += $(wildcard $(DIR)/*.c)
TESTOBJS += $(patsubst $(DIR)/%.c,$(TESTOBJ)/%.o,$(wildcard $(DIR)/*.c))

$(TESTOBJ)/%.o: $(DIR)/%.c | $(TESTOBJ)
		$(CC) $(CFLAGS) $(TESTINC) $(INC) $(DEFS) -c $< -o $@

$(TESTOBJ)/%.d: $(DIR)/%.c | $(TESTOBJ)
		$(DEPSCRIPT) $< $(TESTOBJ) > $@ || $(RM) $@

DIR  :=
