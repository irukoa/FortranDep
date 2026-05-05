#ifndef _FORTRAN_DEPENDENCY_LOGIC_H
#define _FORTRAN_DEPENDENCY_LOGIC_H

#include "src/API/API.h"
#include "src/DEP/Dependency.h"
#include "src/FORTRAN/FortranLexer.h"
#include "src/TOK/Tokenizer.h"

/**
 * @brief Enumeration of Fortran object types used in dependency analysis.
 *
 * @details
 * Represents the semantic classification of entities discovered during
 * analysis of Fortran source code.
 *
 * These values are typically mapped to FDEP_ObjType when constructing
 * dependencies and targets.
 */
typedef enum {
  FDEP_OBJ_OBJECT,   /**< Compiled object file. */
  FDEP_OBJ_SOURCE,   /**< Source file. */
  FDEP_OBJ_MODULE,   /**< Fortran module. */
  FDEP_OBJ_SUBMODULE /**< Fortran submodule. */
} FDEP_FortranObjType;

/**
 * @brief Builds a dependency tree from a list of tokenized statements.
 *
 * @details
 * Analyzes the provided StatementList and constructs a list of targets
 * representing Fortran entities (e.g., modules, submodules, source files)
 * and their dependencies.
 *
 * The function interprets statements using the Fortran lexer utilities
 * and populates a dynamically allocated array of FDEP_Target objects.
 *
 * @param[out] TargetList
 *     Pointer to an array of FDEP_Target pointers. Memory is allocated
 *     by this function on success. The caller assumes ownership.
 *
 * @param[in] StatementList
 *     Array of pointers to tokenized statements.
 *
 * @param[in] StatementCount
 *     Number of elements in StatementList.
 *
 * @param[in] StrictMode
 *     If TRUE, the object file depends on module files.
 *
 * @param[out] FailByCaller
 *     Optional pointer to receive an error code. If NULL, errors are handled
 *     internally via FDEP_API_ERROR.
 *
 * @return
 * Number of targets created. Returns 0 if no targets are produced or if
 * an error occurs.
 *
 * @note
 * - No allocation is performed if the return value is 0.
 * - Each target may contain multiple dependencies derived from "use"
 *   statements and other constructs.
 *
 * @warning
 * - StatementList must contain valid, initialized FDEP_Statement objects.
 *
 * @error
 * On failure:
 * - If FailByCaller is non-NULL, the error code is written to it.
 * - Otherwise, FDEP_API_ERROR is invoked.
 * - Any allocated memory is freed before returning.
 *
 * @see FDEP_Target
 * @see FDEP_Dependency
 * @see FDEP_Statement
 */
size_t FDEP_StatementListIntoDependencyTree(
    FDEP_Target                     ***TargetList,
    const FDEP_Statement *const *const StatementList,
    const size_t                       StatementCount,
    const bool                         StrictMode,
    FDEP_ErrorCode                    *FailByCaller);

#endif
