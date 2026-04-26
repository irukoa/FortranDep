#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include "src/API/API.h"

/* ============================================================================
 * INTERNAL API (not part of the public interface)
 * ----------------------------------------------------------------------------
 * These functions are for internal use only and may change or be removed
 * without notice. Do not rely on them outside this module.
 * ========================================================================== */

/** @cond INTERNAL */

/**
 * @brief Internal helper for freeing token lists.
 *
 * @internal
 * This function is not part of the public API.
 */
INTDEC(void,
       FDEP_FreeTokenList,
       char      ***TokenList, /**< Pointer to array of strings. */
       const size_t TokenCount);

/**
 * @brief Internal helper for splitting raw input into tokens.
 *
 * @internal
 * This function is not part of the public API.
 */
INTDEC(size_t,
       FDEP_Tokenize,
       const char *const String,
       const char *const Delimiters,
       char           ***TokenList, /**< Pointer to array of strings. */
       FDEP_ErrorCode   *FailByCaller);

/// @endcond

/**
 * @brief Represents a tokenized statement.
 *
 * @details
 * A statement consists of a dynamically allocated array of tokens (strings),
 * produced during stream tokenization.
 *
 * Each token is a null-terminated string. The TokenList array stores pointers
 * to these strings.
 *
 * @note
 * - Both the TokenList array and the individual tokens are dynamically
 * allocated.
 * - Instances of this struct are typically created by FDEP_TokenizeStream.
 *
 * @warning
 * - TokenCount represents the number of valid tokens stored in TokenList.
 * - The capacity of TokenList is equal to TokenCount unless otherwise
 * specified.
 *
 * @see FDEP_TokenizeStream
 * @see FDEP_FreeStatementList
 */
typedef struct _FDEP_Statement {
  char **TokenList;  /**< Array of token strings. */
  size_t TokenCount; /**< Number of tokens in TokenList. */
} FDEP_Statement;

/**
 * @brief Frees a list of statements and all associated memory.
 *
 * @details
 * Releases all memory allocated for a statement list created by
 * FDEP_TokenizeStream, including:
 * - Each FDEP_Statement structure
 * - Each token string within TokenList
 * - Each TokenList array
 * - The StatementList array itself
 *
 * @param[in,out] StatementList
 *     Pointer to the array of FDEP_Statement pointers to be freed.
 *     After this call, the pointer becomes invalid.
 *
 * @param[in] StatementCount
 *     Number of statements in the StatementList array.
 *
 * @note
 * - It is safe to call this function with StatementCount == 0.
 * - In this case, StatementList may be NULL or a valid pointer; no action is
 * taken.
 *
 * @warning
 * - Passing an invalid or partially initialized StatementList results in
 *   undefined behavior.
 *
 * @see FDEP_TokenizeStream
 */
void FDEP_FreeStatementList(FDEP_Statement ***StatementList,
                            const size_t      StatementCount);

/**
 * @brief Tokenizes an input stream into a list of statements.
 *
 * @details
 * Reads data from the given Stream and splits it into statements using
 * SeparatorMarker. Each statement is further processed using the set of
 * Delimiters.
 *
 * If a string ends with ContinuationMarker, it is concatenated with the
 * subsequent string before tokenization.
 *
 * An optional preprocessing step can be applied to each string prior to
 * tokenization by providing a StringPreprocess function.
 *
 * @param[out] StatementList
 *     Pointer to an array of FDEP_Statement pointers. Memory is allocated
 *     by this function on success. The caller is responsible for freeing it.
 *
 * @param[in] Delimiters
 *     Null-terminated string containing delimiter characters.
 *
 * @param[in] ContinuationMarker
 *     Character indicating that the current string continues onto the next.
 *
 * @param[in] SeparatorMarker
 *     Character used to separate statements in the input.
 *
 * @param[in] Stream
 *     Input stream to read from.
 *
 * @param[in] StringPreprocess
 *     Optional function applied to each string before tokenization.
 *     Pass NULL to disable preprocessing.
 *
 * @param[out] FailByCaller
 *     Optional pointer to receive an error code. If NULL, errors are handled
 *     internally via FDEP_API_ERROR.
 *
 * @return
 * Number of statements parsed. Returns 0 if no statements are found or if
 * an error occurs.
 *
 * @note
 * - Memory for StatementList is allocated only if the return value is greater
 * than 0.
 * - The caller assumes ownership of the allocated memory.
 *
 * @warning
 * StatementList must be a valid pointer. Behavior is undefined otherwise.
 *
 * @error
 * On failure:
 * - If FailByCaller is non-NULL, the error code is written to it.
 * - If FailByCaller is NULL, FDEP_API_ERROR is invoked.
 * - Any memory allocated during execution is freed before returning.
 */
size_t FDEP_TokenizeStream(FDEP_Statement ***StatementList,
                           const char *const Delimiters,
                           const char        ContinuationMarker,
                           const char        SeparatorMarker,
                           FILE             *Stream,
                           void (*StringPreprocess)(char *String),
                           FDEP_ErrorCode *FailByCaller);

#endif
