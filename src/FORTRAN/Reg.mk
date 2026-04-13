DIR  := src/FORTRAN
SRCS += $(wildcard $(DIR)/*.c)
OBJS += $(patsubst $(DIR)/%.c,$(OBJ)/%.o,$(wildcard $(DIR)/*.c))

$(OBJ)/%.o: $(DIR)/%.c | $(OBJ)
		$(CC) $(CFLAGS) $(INC) $(DEFS) -c $< -o $@

$(OBJ)/%.d: $(DIR)/%.c | $(OBJ)
		$(DEPSCRIPT) $< $(OBJ) > $@ || $(RM) $@

DIR  :=
