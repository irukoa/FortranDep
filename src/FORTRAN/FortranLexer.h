#ifndef _LEXER_H
#define _LEXER_H

#include "src/TOK/Tokenizer.h"
#include <stdbool.h>

typedef enum {
  FDEP_TK_MODULE,
  FDEP_TK_SUBMODULE,
  FDEP_TK_USE,
  FDEP_TK_INTRINSIC,
  FDEP_TK_NON_INTRINSIC,
  FDEP_TK_PROCEDURE,
  FDEP_TK_FUNCTION,
  FDEP_TK_SUBROUTINE,
  FDEP_TK_END,
  FDEP_TK_UNKNOWN
} FDEP_FortranToken;

/* Given a string, returns one of the enumerated tokens.*/
FDEP_FortranToken FDEP_GetToken(const char *const String);

/* Given Statement, returns true if it contains the definition of a module
 * and false otherwise. If a module is defined, replaces the value of
 * IndexModule with the index that contains the keyword `module`. Similarly,
 * replaces the value of IndexName by the statement token index that contains
 * the name of the module.*/
bool FDEP_StatementContainsDefinedModule(const FDEP_Statement *const Statement,
                                         size_t *IndexModule,
                                         size_t *IndexName);

/* Given Statement, returns true if it contains the definition of a submodule
 * and false otherwise. If a submodule is defined, replaces the value of
 * IndexSubModule with the index that contains the keyword `submodule`.
 * Similarly, replaces the value of IndexName by the statement token index that
 * contains the name of the submodule.*/
bool FDEP_StatementContainsDefinedSubModule(
    const FDEP_Statement *const Statement,
    size_t                     *IndexSubModule,
    size_t                     *IndexName);

/* Given Statement, returns true if it contains a non-intrinsic used module
 * and false otherwise. If a module is used, replaces the value of
 * IndexUse with the index that contains the keyword `use`. Similarly,
 * replaces the value of IndexName by the statement token index that contains
 * the name of the used module.*/
bool FDEP_StatementContainsUsedModule(const FDEP_Statement *const Statement,
                                      size_t                     *IndexUse,
                                      size_t                     *IndexName);

#endif
