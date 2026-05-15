#ifndef _LEXER_H
#define _LEXER_H

#include "src/API/API.h"
#include "src/TOK/Tokenizer.h"

/**
 * @brief Enumeration of recognized Fortran tokens.
 *
 * @details
 * Represents keyword categories identified from tokenized strings.
 * These values are produced by FDEP_GetToken and used in higher-level
 * statement analysis.
 */
typedef enum {
  FDEP_TK_MODULE,        /**< "module" keyword */
  FDEP_TK_SUBMODULE,     /**< "submodule" keyword */
  FDEP_TK_USE,           /**< "use" keyword */
  FDEP_TK_INTRINSIC,     /**< "intrinsic" keyword */
  FDEP_TK_NON_INTRINSIC, /**< "non_intrinsic" keyword */
  FDEP_TK_PROCEDURE,     /**< "procedure" keyword */
  FDEP_TK_FUNCTION,      /**< "function" keyword */
  FDEP_TK_SUBROUTINE,    /**< "subroutine" keyword */
  FDEP_TK_END,           /**< "end" keyword */
  FDEP_TK_ENDMODULE,     /**< "endmodule" keyword */
  FDEP_TK_ENDSUBMODULE,  /**< "endsubmodule" keyword */
  FDEP_TK_PURE,          /**< "pure" keyword */
  FDEP_TK_IMPURE,        /**< "impure" keyword */
  FDEP_TK_ELEMENTAL,     /**< "elemental" keyword */
  FDEP_TK_RECURSIVE,     /**< "recursive" keyword */
  FDEP_TK_NONRECURSIVE,  /**< "non_recursive" keyword */
  FDEP_TK_INTEGER,       /**< "integer" keyword */
  FDEP_TK_REAL,          /**< "real" keyword */
  FDEP_TK_DOUBLE,        /**< "double" keyword */
  FDEP_TK_COMPLEX,       /**< "complex" keyword */
  FDEP_TK_LOGICAL,       /**< "logical" keyword */
  FDEP_TK_CHARACTER,     /**< "character" keyword */
  FDEP_TK_TYPE,          /**< "type" keyword */
  FDEP_TK_CLASS,         /**< "class" keyword */
  FDEP_TK_UNKNOWN        /**< Unrecognized token */
} FDEP_FortranToken;

/**
 * @brief Classifies a string as a Fortran token.
 *
 * @param[in] String
 *     Null-terminated string to classify.
 *
 * @return
 * Corresponding FDEP_FortranToken value. Returns FDEP_TK_UNKNOWN if the
 * string does not match any known token.
 *
 * @note
 * - Matching is typically case-insensitive when used with
 *   FDEP_FortranPreprocess.
 */
FDEP_FortranToken FDEP_GetToken(const char *const String);

/**
 * @brief Checks whether a statement defines a module.
 *
 * @details
 * Determines whether the given statement represents a module definition.
 * If successful, outputs the positions of the relevant tokens.
 *
 * @param[in] Statement
 *     Tokenized statement to analyze.
 *
 * @param[out] IndexModule
 *     Receives the index of the "module" keyword within the statement.
 *
 * @param[out] IndexName
 *     Receives the index of the module name token.
 *
 * @return
 * true if the statement defines a module, false otherwise.
 *
 * @note
 * - Output parameters are only valid if the function returns true.
 *
 * @warning
 * - Statement must be a valid, initialized FDEP_Statement.
 */
bool FDEP_StatementContainsDefinedModule(const FDEP_Statement *const Statement,
                                         size_t *IndexModule,
                                         size_t *IndexName);

/**
 * @brief Checks whether a statement defines a submodule.
 *
 * @details
 * Determines whether the given statement represents a submodule definition.
 * If successful, outputs the positions of the relevant tokens.
 *
 * @param[in] Statement
 *     Tokenized statement to analyze.
 *
 * @param[out] IndexSubModule
 *     Receives the index of the "submodule" keyword within the statement.
 *
 * @param[out] IndexName
 *     Receives the index of the submodule name token.
 *
 * @return
 * true if the statement defines a submodule, false otherwise.
 *
 * @note
 * - Output parameters are only valid if the function returns true.
 *
 * @warning
 * - Statement must be a valid, initialized FDEP_Statement.
 */
bool FDEP_StatementContainsDefinedSubModule(
    const FDEP_Statement *const Statement,
    size_t                     *IndexSubModule,
    size_t                     *IndexName);

/**
 * @brief Checks whether a statement contains a non-intrinsic module usage.
 *
 * @details
 * Determines whether the statement includes a "use" statement referring to a
 * non-intrinsic module.
 *
 * If successful, outputs the positions of the relevant tokens.
 *
 * @param[in] Statement
 *     Tokenized statement to analyze.
 *
 * @param[out] IndexUse
 *     Receives the index of the "use" keyword within the statement.
 *
 * @param[out] IndexName
 *     Receives the index of the referenced module name.
 *
 * @return
 * true if a non-intrinsic module usage is found, false otherwise.
 *
 * @note
 * - Output parameters are only valid if the function returns true.
 *
 * @warning
 * - Statement must be a valid, initialized FDEP_Statement.
 */
bool FDEP_StatementContainsUsedModule(const FDEP_Statement *const Statement,
                                      size_t                     *IndexUse,
                                      size_t                     *IndexName);

/**
 * @brief Checks whether a statement ends a program unit.
 *
 * @details
 * Determines whether the statement ends a module or submodule.
 *
 * @param[in] Statement
 *     Tokenized statement to analyze.
 *
 * @return
 * true if module or submodule ending is found, false otherwise.
 *
 * @note
 * - Cannot determine whether the plain 'end' statement ends a program unit.
 * Will return false in that case.
 *
 * @warning
 * - Statement must be a valid, initialized FDEP_Statement.
 */
bool FDEP_StatementEndsProgramUnit(const FDEP_Statement *const Statement);

#endif
