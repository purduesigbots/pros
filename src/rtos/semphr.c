#include "FreeRTOS.h"
#include "semphr.h"

/**
 * semphr. h
 * <pre>sem_wait(
 *                   sem_t sem,
 *                   uint32_t xBlockTime
 *               )</pre>
 *
 * <i>Macro</i> to obtain a semaphore.  The semaphore must have previously been
 * created with a call to sem_binary_create(), mutex_create() or
 * sem_create().
 *
 * @param sem A handle to the semaphore being taken - obtained when
 * the semaphore was created.
 *
 * @param xBlockTime The time in ticks to wait for the semaphore to become
 * available.  The macro portTICK_PERIOD_MS can be used to convert this to a
 * real time.  A block time of zero can be used to poll the semaphore.  A block
 * time of portMAX_DELAY can be used to block indefinitely (provided
 * INCLUDE_vTaskSuspend is set to 1 in FreeRTOSConfig.h).
 *
 * @return pdTRUE if the semaphore was obtained.  pdFALSE
 * if xBlockTime expired without the semaphore becoming available.
 *
 * Example usage:
 <pre>
 sem_t sem = NULL;

 // A task that creates a semaphore.
 void vATask( void * pvParameters )
 {
    // Create the semaphore to guard a shared resource.
    sem = sem_binary_create();
 }

 // A task that uses the semaphore.
 void vAnotherTask( void * pvParameters )
 {
    // ... Do other things.

    if( sem != NULL )
    {
        // See if we can obtain the semaphore.  If the semaphore is not available
        // wait 10 ticks to see if it becomes free.
        if( sem_wait( sem, ( uint32_t ) 10 ) == pdTRUE )
        {
            // We were able to obtain the semaphore and can now access the
            // shared resource.

            // ...

            // We have finished accessing the shared resource.  Release the
            // semaphore.
            sem_post( sem );
        }
        else
        {
            // We could not obtain the semaphore and can therefore not access
            // the shared resource safely.
        }
    }
 }
 </pre>
 * \defgroup sem_wait sem_wait
 * \ingroup Semaphores
 */
uint8_t sem_wait(sem_t sem, uint32_t timeout) {
	return xQueueSemaphoreTake( ( sem ), ( timeout ) );
}

/**
 * semphr. h
 * <pre>sem_post( sem_t sem )</pre>
 *
 * <i>Macro</i> to release a semaphore.  The semaphore must have previously been
 * created with a call to sem_binary_create(), mutex_create() or
 * sem_create(). and obtained using sSemaphoreTake().
 *
 * This macro must not be used from an ISR.  See semGiveFromISR () for
 * an alternative which can be used from an ISR.
 *
 * This macro must also not be used on semaphores created using
 * mutex_recursive_create().
 *
 * @param sem A handle to the semaphore being released.  This is the
 * handle returned when the semaphore was created.
 *
 * @return pdTRUE if the semaphore was released.  pdFALSE if an error occurred.
 * Semaphores are implemented using queues.  An error can occur if there is
 * no space on the queue to post a message - indicating that the
 * semaphore was not first obtained correctly.
 *
 * Example usage:
 <pre>
 sem_t sem = NULL;

 void vATask( void * pvParameters )
 {
    // Create the semaphore to guard a shared resource.
    sem = vSemaphoreCreateBinary();

    if( sem != NULL )
    {
        if( sem_post( sem ) != pdTRUE )
        {
            // We would expect this call to fail because we cannot give
            // a semaphore without first "taking" it!
        }

        // Obtain the semaphore - don't block if the semaphore is not
        // immediately available.
        if( sem_wait( sem, ( uint32_t ) 0 ) )
        {
            // We now have the semaphore and can access the shared resource.

            // ...

            // We have finished accessing the shared resource so can free the
            // semaphore.
            if( sem_post( sem ) != pdTRUE )
            {
                // We would not expect this call to fail because we must have
                // obtained the semaphore to get here.
            }
        }
    }
 }
 </pre>
 * \defgroup sem_post sem_post
 * \ingroup Semaphores
 */
uint8_t sem_post(sem_t sem) {
	return xQueueGenericSend((queue_t)(sem), NULL, semGIVE_BLOCK_TIME, queueSEND_TO_BACK);
}

/**
 * semphr. h
 * <pre>sem_t mutex_create( void )</pre>
 *
 * Creates a new mutex type semaphore instance, and returns a handle by which
 * the new mutex can be referenced.
 *
 * Internally, within the FreeRTOS implementation, mutex semaphores use a block
 * of memory, in which the mutex structure is stored.  If a mutex is created
 * using mutex_create() then the required memory is automatically
 * dynamically allocated inside the mutex_create() function.  (see
 * http://www.freertos.org/a00111.html).  If a mutex is created using
 * mutex_create_static() then the application writer must provided the
 * memory.  mutex_create_static() therefore allows a mutex to be created
 * without using any dynamic memory allocation.
 *
 * Mutexes created using this function can be accessed using the sem_wait()
 * and sem_post() macros.  The mutex_recursive_take() and
 * mutex_recursive_give() macros must not be used.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See sem_binary_create() for an alternative implementation that can be
 * used for pure synchronisation (where one task or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @return If the mutex was successfully created then a handle to the created
 * semaphore is returned.  If there was not enough heap to allocate the mutex
 * data structures then NULL is returned.
 *
 * Example usage:
 <pre>
 sem_t sem;

 void vATask( void * pvParameters )
 {
    // Semaphore cannot be used before a call to mutex_create().
    // This is a macro so pass the variable in directly.
    sem = mutex_create();

    if( sem != NULL )
    {
        // The semaphore was created successfully.
        // The semaphore can now be used.
    }
 }
 </pre>
 * \defgroup mutex_create mutex_create
 * \ingroup Semaphores
 */
mutex_t mutex_create(void) {
	return (mutex_t)xQueueCreateMutex(queueQUEUE_TYPE_MUTEX);
}

uint8_t mutex_give(mutex_t mutex) {
	return xQueueGenericSend((queue_t)(mutex), NULL, semGIVE_BLOCK_TIME, queueSEND_TO_BACK);
}

uint8_t mutex_take(mutex_t mutex, uint32_t timeout) {
	return xQueueSemaphoreTake( ( mutex ), ( timeout ) );
}

void mutex_delete(mutex_t mutex){
  sem_delete((sem_t)(mutex));
}

/**
 * semphr. h
 * <pre>sem_t sem_binary_create( void )</pre>
 *
 * Creates a new binary semaphore instance, and returns a handle by which the
 * new semaphore can be referenced.
 *
 * In many usage scenarios it is faster and more memory efficient to use a
 * direct to task notification in place of a binary semaphore!
 * http://www.freertos.org/RTOS-task-notifications.html
 *
 * Internally, within the FreeRTOS implementation, binary semaphores use a block
 * of memory, in which the semaphore structure is stored.  If a binary semaphore
 * is created using sem_binary_create() then the required memory is
 * automatically dynamically allocated inside the sem_binary_create()
 * function.  (see http://www.freertos.org/a00111.html).  If a binary semaphore
 * is created using semCreateBinaryStatic() then the application writer
 * must provide the memory.  semCreateBinaryStatic() therefore allows a
 * binary semaphore to be created without using any dynamic memory allocation.
 *
 * The old vSemaphoreCreateBinary() macro is now deprecated in favour of this
 * sem_binary_create() function.  Note that binary semaphores created using
 * the vSemaphoreCreateBinary() macro are created in a state such that the
 * first call to 'take' the semaphore would pass, whereas binary semaphores
 * created using sem_binary_create() are created in a state such that the
 * the semaphore must first be 'given' before it can be 'taken'.
 *
 * This type of semaphore can be used for pure synchronisation between tasks or
 * between an interrupt and a task.  The semaphore need not be given back once
 * obtained, so one task/interrupt can continuously 'give' the semaphore while
 * another continuously 'takes' the semaphore.  For this reason this type of
 * semaphore does not use a priority inheritance mechanism.  For an alternative
 * that does use priority inheritance see mutex_create().
 *
 * @return Handle to the created semaphore, or NULL if the memory required to
 * hold the semaphore's data structures could not be allocated.
 *
 * Example usage:
 <pre>
 sem_t sem = NULL;

 void vATask( void * pvParameters )
 {
    // Semaphore cannot be used before a call to sem_binary_create().
    // This is a macro so pass the variable in directly.
    sem = sem_binary_create();

    if( sem != NULL )
    {
        // The semaphore was created successfully.
        // The semaphore can now be used.
    }
 }
 </pre>
 * \defgroup sem_binary_create sem_binary_create
 * \ingroup Semaphores
 */
sem_t sem_binary_create(void) {
	return xQueueGenericCreate((uint32_t)1, semSEMAPHORE_QUEUE_ITEM_LENGTH, queueQUEUE_TYPE_BINARY_SEMAPHORE);
}

/**
 * semphr. h
 * <pre>sem_t mutex_recursive_create( void )</pre>
 *
 * Creates a new recursive mutex type semaphore instance, and returns a handle
 * by which the new recursive mutex can be referenced.
 *
 * Internally, within the FreeRTOS implementation, recursive mutexs use a block
 * of memory, in which the mutex structure is stored.  If a recursive mutex is
 * created using mutex_recursive_create() then the required memory is
 * automatically dynamically allocated inside the
 * mutex_recursive_create() function.  (see
 * http://www.freertos.org/a00111.html).  If a recursive mutex is created using
 * semCreateRecursiveMutexStatic() then the application writer must
 * provide the memory that will get used by the mutex.
 * semCreateRecursiveMutexStatic() therefore allows a recursive mutex to
 * be created without using any dynamic memory allocation.
 *
 * Mutexes created using this macro can be accessed using the
 * mutex_recursive_take() and mutex_recursive_give() macros.  The
 * sem_wait() and sem_post() macros must not be used.
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * mutex_recursive_give() for each successful 'take' request.  For example,
 * if a task successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task until it has also  'given' the mutex back
 * exactly five times.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See sem_binary_create() for an alternative implementation that can be
 * used for pure synchronisation (where one task or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @return sem Handle to the created mutex semaphore.  Should be of type
 * sem_t.
 *
 * Example usage:
 <pre>
 sem_t sem;

 void vATask( void * pvParameters )
 {
    // Semaphore cannot be used before a call to mutex_create().
    // This is a macro so pass the variable in directly.
    sem = mutex_recursive_create();

    if( sem != NULL )
    {
        // The semaphore was created successfully.
        // The semaphore can now be used.
    }
 }
 </pre>
 * \defgroup mutex_recursive_create mutex_recursive_create
 * \ingroup Semaphores
 */
mutex_t mutex_recursive_create(void) {
	return xQueueCreateMutex(queueQUEUE_TYPE_RECURSIVE_MUTEX);
}

/**
 * semphr. h
 * <pre>mutex_recursive_give( sem_t xMutex )</pre>
 *
 * <i>Macro</i> to recursively release, or 'give', a mutex type semaphore.
 * The mutex must have previously been created using a call to
 * mutex_recursive_create();
 *
 * configUSE_RECURSIVE_MUTEXES must be set to 1 in FreeRTOSConfig.h for this
 * macro to be available.
 *
 * This macro must not be used on mutexes created using mutex_create().
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * mutex_recursive_give() for each successful 'take' request.  For example,
 * if a task successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task until it has also  'given' the mutex back
 * exactly five times.
 *
 * @param xMutex A handle to the mutex being released, or 'given'.  This is the
 * handle returned by mutex_create();
 *
 * @return pdTRUE if the semaphore was given.
 *
 * Example usage:
 <pre>
 sem_t xMutex = NULL;

 // A task that creates a mutex.
 void vATask( void * pvParameters )
 {
    // Create the mutex to guard a shared resource.
    xMutex = mutex_recursive_create();
 }

 // A task that uses the mutex.
 void vAnotherTask( void * pvParameters )
 {
    // ... Do other things.

    if( xMutex != NULL )
    {
        // See if we can obtain the mutex.  If the mutex is not available
        // wait 10 ticks to see if it becomes free.
        if( mutex_recursive_take( xMutex, ( uint32_t ) 10 ) == pdTRUE )
        {
            // We were able to obtain the mutex and can now access the
            // shared resource.

            // ...
            // For some reason due to the nature of the code further calls to
                        // mutex_recursive_take() are made on the same mutex.  In real
                        // code these would not be just sequential calls as this would make
                        // no sense.  Instead the calls are likely to be buried inside
                        // a more complex call structure.
            mutex_recursive_take( xMutex, ( uint32_t ) 10 );
            mutex_recursive_take( xMutex, ( uint32_t ) 10 );

            // The mutex has now been 'taken' three times, so will not be
                        // available to another task until it has also been given back
                        // three times.  Again it is unlikely that real code would have
                        // these calls sequentially, it would be more likely that the calls
                        // to mutex_recursive_give() would be called as a call stack
                        // unwound.  This is just for demonstrative purposes.
            mutex_recursive_give( xMutex );
                        mutex_recursive_give( xMutex );
                        mutex_recursive_give( xMutex );

                        // Now the mutex can be taken by other tasks.
        }
        else
        {
            // We could not obtain the mutex and can therefore not access
            // the shared resource safely.
        }
    }
 }
 </pre>
 * \defgroup mutex_recursive_give mutex_recursive_give
 * \ingroup Semaphores
 */
uint8_t mutex_recursive_give(mutex_t mutex) {
	return xQueueGiveMutexRecursive((mutex));
}

/**
 * semphr. h
 * mutex_recursive_take(
 *                          sem_t xMutex,
 *                          uint32_t xBlockTime
 *                        )
 *
 * <i>Macro</i> to recursively obtain, or 'take', a mutex type semaphore.
 * The mutex must have previously been created using a call to
 * mutex_recursive_create();
 *
 * configUSE_RECURSIVE_MUTEXES must be set to 1 in FreeRTOSConfig.h for this
 * macro to be available.
 *
 * This macro must not be used on mutexes created using mutex_create().
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * mutex_recursive_give() for each successful 'take' request.  For example,
 * if a task successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task until it has also  'given' the mutex back
 * exactly five times.
 *
 * @param xMutex A handle to the mutex being obtained.  This is the
 * handle returned by mutex_recursive_create();
 *
 * @param xBlockTime The time in ticks to wait for the semaphore to become
 * available.  The macro portTICK_PERIOD_MS can be used to convert this to a
 * real time.  A block time of zero can be used to poll the semaphore.  If
 * the task already owns the semaphore then mutex_recursive_take() will
 * return immediately no matter what the value of xBlockTime.
 *
 * @return pdTRUE if the semaphore was obtained.  pdFALSE if xBlockTime
 * expired without the semaphore becoming available.
 *
 * Example usage:
 <pre>
 sem_t xMutex = NULL;

 // A task that creates a mutex.
 void vATask( void * pvParameters )
 {
    // Create the mutex to guard a shared resource.
    xMutex = mutex_recursive_create();
 }

 // A task that uses the mutex.
 void vAnotherTask( void * pvParameters )
 {
    // ... Do other things.

    if( xMutex != NULL )
    {
        // See if we can obtain the mutex.  If the mutex is not available
        // wait 10 ticks to see if it becomes free.
        if( mutex_recursive_take( sem, ( uint32_t ) 10 ) == pdTRUE )
        {
            // We were able to obtain the mutex and can now access the
            // shared resource.

            // ...
            // For some reason due to the nature of the code further calls to
                        // mutex_recursive_take() are made on the same mutex.  In real
                        // code these would not be just sequential calls as this would make
                        // no sense.  Instead the calls are likely to be buried inside
                        // a more complex call structure.
            mutex_recursive_take( xMutex, ( uint32_t ) 10 );
            mutex_recursive_take( xMutex, ( uint32_t ) 10 );

            // The mutex has now been 'taken' three times, so will not be
                        // available to another task until it has also been given back
                        // three times.  Again it is unlikely that real code would have
                        // these calls sequentially, but instead buried in a more complex
                        // call structure.  This is just for illustrative purposes.
            mutex_recursive_give( xMutex );
                        mutex_recursive_give( xMutex );
                        mutex_recursive_give( xMutex );

                        // Now the mutex can be taken by other tasks.
        }
        else
        {
            // We could not obtain the mutex and can therefore not access
            // the shared resource safely.
        }
    }
 }
 </pre>
 * \defgroup mutex_recursive_take mutex_recursive_take
 * \ingroup Semaphores
 */
uint8_t mutex_recursive_take(mutex_t mutex, uint32_t timeout) {
	return xQueueTakeMutexRecursive((mutex), (timeout));
}

sem_t sem_create(uint32_t max_count, uint32_t init_count) {
	return xQueueCreateCountingSemaphore((max_count), (init_count));
}

void sem_delete(sem_t sem) {
	queue_delete((queue_t)(sem));
}

task_t mutex_get_owner(mutex_t mutex) {
	return xQueueGetMutexHolder((mutex));
}

uint32_t sem_get_count(sem_t sem) {
	return queue_get_waiting((queue_t)(sem));
}

mutex_t mutex_create_static(static_sem_s_t* pxMutexBuffer) {
	return xQueueCreateMutexStatic(queueQUEUE_TYPE_MUTEX, (pxMutexBuffer));
}

sem_t sem_create_static(uint32_t max_count, uint32_t init_count, static_sem_s_t* psemBuffer) {
	return xQueueCreateCountingSemaphoreStatic((max_count), (init_count), (psemBuffer));
}
