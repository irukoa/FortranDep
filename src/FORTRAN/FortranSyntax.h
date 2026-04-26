#ifndef _FORTRAN_SYNTAX_H
#define _FORTRAN_SYNTAX_H

#include "src/API/API.h"

/**
 * @name Fortran lexical configuration
 * @brief Constants defining tokenization rules for Fortran source code.
 *
 * @details
 * These macros define the character set and markers used to tokenize
 * Fortran input streams with FDEP_TokenizeStream.
 *
 * @{
 */

#define FDEP_FORTRAN_DELIMITERS   " \t\n(),:" /**< Token delimiters. */
#define FDEP_FORTRAN_CONTINUATION '&'         /**< Line continuation marker. */
#define FDEP_FORTRAN_SEPARATOR    ';'         /**< Statement separator. */
#define FDEP_FORTRAN_COMMENT      '!'         /**< Comment indicator. */
#define FDEP_FORTRAN_STRING1      '\''        /**< Primary string delimiter. */
#define FDEP_FORTRAN_STRING2      '"' /**< Alternate string delimiter. */

/** @} */

/**
 * @name Fortran semantic identifiers
 * @brief Special identifiers used in dependency analysis.
 *
 * @{
 */

#define FDEP_OBJECT_NAME      "#OBJECT#" /**< Placeholder for object targets. */
#define FDEP_SOURCE_NAME      "#SOURCEFILE#" /**< Placeholder for source files. */
#define FDEP_MODULE_SUFFIX    "mod"          /**< Module file suffix. */
#define FDEP_SUBMODULE_SUFFIX "smod"         /**< Submodule file suffix. */

/** @} */

/**
 * @brief Preprocesses a string according to simplified Fortran rules.
 *
 * @details
 * Applies the following transformations in-place:
 * - Removes any content following the comment marker ('!').
 * - Converts all characters to lowercase.
 *
 * @param[in,out] String
 *     Null-terminated string to preprocess. The transformation is performed
 *     in-place.
 *
 * @warning
 * - The input string must be mutable.
 * - Behavior is undefined if String is NULL.
 *
 * @note
 * - This function is intended to be used as a StringPreprocess callback
 *   for FDEP_TokenizeStream.
 */
void FDEP_FortranPreprocess(char *String);

#endif
