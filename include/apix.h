#ifndef _PROS_API_EXTENDED_H_
#define _PROS_API_EXTENDED_H_

#include "api.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**                             RTOS FACILITIES                              **/
/**                                                                          **/
/**                                                                          **/
/** See https://pros.cs.purdue.edu/v5/extended/multitasking to learn more.   **/
/******************************************************************************/

/**
 * Unblocks a task in the Blocked state (e.g. waiting for a delay, on a semaphore, etc.)
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#abort_delay for details.
 */
bool_t task_abort_delay(task_t task);

/**
 * Creates a binary semaphore.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#binary_semaphores for details.
 *
 * @return
 *          A newly created semaphore.
 */
sem_t sem_binary_create();

/**
 * Creates a recursive mutex which can be locked recursively by the owner.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#recursive_mutexes for details.
 *
 * @return
 *          A newly created recursive mutex.
 */
mutex_t mutex_recursive_create();

/**
 * Takes a recursive mutex
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#recursive_mutexes for details.
 *
 * @param mutex
 *          A mutex handle created by mutex_recursive_create
 * @param wait_time
 *          Amount of time to wait before timing out
 * @return
 *          1 if the mutex was obtained, 0 otherwise
 */
bool_t mutex_take_recursive(mutex_t mutex, uint32_t wait_time);

#endif /* _PROS_API_EXTENDED_H_ */