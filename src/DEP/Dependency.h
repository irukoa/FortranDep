#ifndef _DEPENDENCY_H
#define _DEPENDENCY_H

#include "src/API/API.h"

/**
 * @brief Identifier for object types.
 *
 * @details
 * Represents the type of a dependency or target. The meaning of specific
 * values is defined by the caller.
 */
typedef int FDEP_ObjType;

/**
 * @brief Represents a single dependency.
 *
 * @details
 * A dependency consists of a name and an associated object type.
 *
 * @note
 * - Name is dynamically allocated.
 * - Instances are created using FDEP_NewDependency.
 *
 * @see FDEP_NewDependency
 * @see FDEP_FreeDependency
 */
typedef struct _FDEP_Dependency {
  char        *Name; /**< Null-terminated dependency name. */
  FDEP_ObjType Type; /**< Dependency type identifier. */
} FDEP_Dependency;

/**
 * @brief Frees a dependency object.
 *
 * @param[in,out] Dependency
 *     Pointer to the dependency pointer to be freed.
 *
 * @note
 * - Safe to call with NULL.
 * - After the call, the pointer becomes invalid.
 *
 * @see FDEP_NewDependency
 */
void FDEP_FreeDependency(FDEP_Dependency **Dependency);

/**
 * @brief Allocates and initializes a new dependency.
 *
 * @param[in] Name
 *     Null-terminated string representing the dependency name.
 *
 * @param[in] Type
 *     Object type associated with the dependency.
 *
 * @param[out] FailByCaller
 *     Optional pointer to receive an error code. If NULL, errors are handled
 *     internally via FDEP_API_ERROR.
 *
 * @return
 * Pointer to a newly allocated FDEP_Dependency on success, or NULL on failure.
 *
 * @note
 * - The caller assumes ownership of the returned object.
 *
 * @error
 * On failure:
 * - If FailByCaller is non-NULL, the error code is written to it.
 * - Otherwise, FDEP_API_ERROR is invoked.
 * - Any allocated memory is freed before returning.
 */
FDEP_Dependency *FDEP_NewDependency(const char *const  Name,
                                    const FDEP_ObjType Type,
                                    FDEP_ErrorCode    *FailByCaller);

/**
 * @brief Represents a target with associated dependencies.
 *
 * @details
 * A target consists of a name, a type, and a dynamically managed list of
 * dependencies.
 *
 * @note
 * - DependencyList is an array of pointers to FDEP_Dependency.
 * - DependencyCount reflects the number of valid dependencies.
 * - Instances are created using FDEP_NewTarget.
 *
 * @see FDEP_NewTarget
 * @see FDEP_AddDependencyToTarget
 * @see FDEP_FreeTarget
 */
typedef struct _FDEP_Target {
  char             *Name;            /**< Null-terminated target name. */
  FDEP_ObjType      Type;            /**< Target type identifier. */
  size_t            DependencyCount; /**< Number of dependencies. */
  FDEP_Dependency **DependencyList;  /**< Array of dependency pointers. */
} FDEP_Target;

/**
 * @brief Frees a target and all associated dependencies.
 *
 * @param[in,out] Target
 *     Pointer to the target pointer to be freed.
 *
 * @note
 * - Frees:
 *   - All dependencies in DependencyList
 *   - The DependencyList array
 *   - The target itself
 *
 * @see FDEP_NewTarget
 * @see FDEP_AddDependencyToTarget
 */
void FDEP_FreeTarget(FDEP_Target **Target);

/**
 * @brief Allocates and initializes a new target without dependencies.
 *
 * @details
 * Initializes a target with:
 * - DependencyCount = 0
 * - DependencyList = NULL
 *
 * @param[in] Name
 *     Null-terminated string representing the target name.
 *
 * @param[in] Type
 *     Object type associated with the target.
 *
 * @param[out] FailByCaller
 *     Optional pointer to receive an error code. If NULL, errors are handled
 *     internally via FDEP_API_ERROR.
 *
 * @return
 * Pointer to a newly allocated FDEP_Target on success, or NULL on failure.
 *
 * @note
 * - The caller assumes ownership of the returned object.
 *
 * @error
 * On failure:
 * - If FailByCaller is non-NULL, the error code is written to it.
 * - Otherwise, FDEP_API_ERROR is invoked.
 * - Any allocated memory is freed before returning.
 */
FDEP_Target *FDEP_NewTarget(const char *const  Name,
                            const FDEP_ObjType Type,
                            FDEP_ErrorCode    *FailByCaller);

/**
 * @brief Adds a dependency to a target.
 *
 * @details
 * Allocates a new dependency and appends it to the target's DependencyList.
 *
 * @param[in] Name
 *     Null-terminated dependency name.
 *
 * @param[in] Type
 *     Dependency type.
 *
 * @param[in,out] Target
 *     Pointer to the target pointer to which the dependency is added.
 *
 * @param[out] FailByCaller
 *     Optional pointer to receive an error code. If NULL, errors are handled
 *     internally via FDEP_API_ERROR.
 *
 * @return
 * true if the dependency was successfully added, false otherwise.
 *
 * @note
 * - On success, DependencyCount is incremented.
 *
 * @error
 * On failure:
 * - If FailByCaller is non-NULL, the error code is written to it.
 * - Otherwise, FDEP_API_ERROR is invoked.
 * - Any intermediate allocations are freed.
 */
bool FDEP_AddDependencyToTarget(const char *const  Name,
                                const FDEP_ObjType Type,
                                FDEP_Target      **Target,
                                FDEP_ErrorCode    *FailByCaller);

/**
 * @brief Frees a list of targets.
 *
 * @param[in,out] TargetList
 *     Pointer to an array of target pointers to be freed.
 *
 * @param[in] TargetCount
 *     Number of targets in the list.
 *
 * @note
 * - Safe to call with TargetCount == 0.
 * - Frees each target using FDEP_FreeTarget.
 * - Frees the TargetList array itself.
 */
void FDEP_FreeTargetList(FDEP_Target ***TargetList,
                         const size_t   TargetCount);

#endif
