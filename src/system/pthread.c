/**
 * pthread.c - Port of pthreads to PROS for V5
 *
 * NOTE: Currently commented out until it can be verified
 *
 * Contains the various methods needed to enable pthreads support
 * based on the espressif/esp-idf repository for use with libcxxrt.
 *
 * Espressif IoT Development Framework is licensed under the Apache 2.0 license
 *
 * Copyright (c) 2017, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
#ifndef _POSIX_THREADS
#define _POSIX_THREADS
#endif

#ifndef _UNIX98_THREAD_MUTEX_ATTRIBUTES
#define _UNIX98_THREAD_MUTEX_ATTRIBUTES
#endif

#include "rtos/FreeRTOS.h"
#include "rtos/list.h"
#include "rtos/semphr.h"
#include "rtos/task.h"
#include "system/system.h"
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#define RTOS_PTHREAD_STATE_EXITED (1 << 1)
#define RTOS_PTHREAD_STATE_RUN (1 << 2)

struct rtos_pthread {
        ListItem_t list_item;
        task_t join_handle;
        uint16_t state;
        uint8_t detached;
};

struct rtos_pthread_task_arg {
        task_fn_t fn;
        void* arg;
};

struct rtos_pthread_mutex {
        ListItem_t list_item;
        sem_t sem;
        int type;
};

static sem_t threads_mut = NULL;
static StaticSemaphore_t threads_mut_buf;

static List_t threads_list;

// static int pthread_mutex_lock(rtos_pthread_mutex* mut, uint32_t tmo);

void rtos_pthread_init() {
        vListInitialise(&threads_list);
        threads_mut = xSemaphoreCreateMutexStatic(&threads_mut_buf);
}

static void* rtos_pthread_find_item(void* (*item_check_fn)(ListItem_t*, void*), void* check_arg) {
        ListItem_t const* list_end = listGET_END_MARKER(&threads_list);
        ListItem_t* list_item = listGET_HEAD_ENTRY(&threads_list);
        while (list_item != list_end) {
                void* val = item_check_fn(list_item, check_arg);
                if (val) {
                        return val;
                }
                list_item = listGET_NEXT(list_item);
        }
        return NULL;
}

static void* rtos_pthread_get_handle_by_desc(ListItem_t* item, void* arg) {
        struct rtos_pthread* pthread = (struct rtos_pthread*)listGET_LIST_ITEM_OWNER(item);
        if (pthread == arg) {
                return (void*)listGET_LIST_ITEM_VALUE(item);
        }
        return NULL;
}

static task_t rtos_pthread_find_handle(pthread_t thread) {
        return (task_t)rtos_pthread_find_item(rtos_pthread_get_handle_by_desc, (void*)thread);
}

static void* rtos_pthread_get_desc_by_handle(ListItem_t* item, void* arg) {
        task_t task_handle = arg;
        task_t comp_handle = (task_t)(listGET_LIST_ITEM_VALUE(item));
        if (task_handle == comp_handle) {
                return (struct rtos_pthread*)listGET_LIST_ITEM_OWNER(item);
        }
        return NULL;
}

static struct rtos_pthread* rtos_pthread_find(task_t task_handle) {
        return (struct rtos_pthread*)rtos_pthread_find_item(rtos_pthread_get_desc_by_handle, (void*)task_handle);
}

static void rtos_pthread_delete(struct rtos_pthread* pthread) {
        uxListRemove(&pthread->list_item);
        vPortFree(pthread);
}

static void pthread_task_fn(void* _arg) {
        struct rtos_pthread_task_arg* arg = (struct rtos_pthread_task_arg*)_arg;

        task_notify_wait(0, 0, NULL, portMAX_DELAY);

        arg->fn(arg->arg);

        vPortFree(arg);

        if (sem_wait(threads_mut, portMAX_DELAY) != pdTRUE) {
                return;  // failed to lock threads list
                         // TODO: error out
        }

        struct rtos_pthread* pthread = rtos_pthread_find(xTaskGetCurrentTaskHandle());
        if (pthread->detached) {
                rtos_pthread_delete(pthread);
        } else {
                if (pthread->join_handle) {
                        task_notify_send(pthread->join_handle, 0, E_NOTIFY_ACTION_NONE, NULL);
                } else {
                        pthread->state = RTOS_PTHREAD_STATE_EXITED;
                }
        }
        sem_post(threads_mut);

        task_delete(NULL);
}

int pthread_create(pthread_t* thread, pthread_attr_t const* attr, void* (*start_routine)(void*), void* arg) {
        if (attr) {
                return ENOSYS;
        }
        struct rtos_pthread_task_arg* task_arg = (struct rtos_pthread_task_arg*)pvPortMalloc(sizeof(task_arg));
        if (task_arg == NULL) {
                return ENOMEM;
        }
        memset(task_arg, 0, sizeof(*task_arg));

        struct rtos_pthread* pthread = (struct rtos_pthread*)pvPortMalloc(sizeof(*pthread));
        if (pthread == NULL) {
                vPortFree(task_arg);
                return ENOMEM;
        }
        memset(pthread, 0, sizeof(*pthread));

        task_arg->fn = (task_fn_t)start_routine;
        task_arg->arg = arg;
        task_t task =
            task_create(&pthread_task_fn, task_arg, TASK_STACK_DEPTH_DEFAULT, TASK_PRIORITY_DEFAULT, "pthread");
        if (task == NULL) {
                vPortFree(pthread);
                vPortFree(task_arg);
                return EAGAIN;
        }

        vListInitialiseItem(&pthread->list_item);
        listSET_LIST_ITEM_OWNER(&pthread->list_item, pthread);
        listSET_LIST_ITEM_VALUE(&pthread->list_item, (uint32_t)task);

        if (sem_wait(threads_mut, portMAX_DELAY) != pdTRUE) {
                vPortFree(pthread);
                vPortFree(task_arg);
                task_delete(task);
                return EAGAIN;
        }

        vListInsertEnd(&threads_list, &pthread->list_item);

        sem_post(threads_mut);

        task_notify_send(task, 0, E_NOTIFY_ACTION_NONE, NULL);

        *thread = (pthread_t)pthread;

        return 0;
}

int pthread_join(pthread_t thread, void** retval) {
        struct rtos_pthread* pthread = (struct rtos_pthread*)thread;
        int ret = 0;
        bool wait = false;

        if (sem_wait(threads_mut, portMAX_DELAY) != pdTRUE) {
                ret = EAGAIN;
        }
        task_t task = rtos_pthread_find_handle(thread);
        if (!task) {
                ret = ESRCH;
        } else if (pthread->join_handle) {
                ret = EINVAL;
        } else if (task == xTaskGetCurrentTaskHandle()) {
                ret = EDEADLK;
        } else {
                struct rtos_pthread* cur_pthread = rtos_pthread_find(xTaskGetCurrentTaskHandle());
                if (cur_pthread && cur_pthread->join_handle == task) {
                        ret = EDEADLK;
                } else {
                        if (pthread->state &= RTOS_PTHREAD_STATE_RUN) {
                                pthread->join_handle = xTaskGetCurrentTaskHandle();
                                wait = true;
                        } else {
                                rtos_pthread_delete(pthread);
                        }
                }
        }
        sem_post(threads_mut);

        if (ret == 0 && wait) {
                task_notify_wait(0, 0, NULL, portMAX_DELAY);
                if (sem_wait(threads_mut, portMAX_DELAY) != pdTRUE) {
                        errno = ENOMSG;
                        return ENOMSG;
                }
                rtos_pthread_delete(pthread);
                sem_post(threads_mut);
        }

        if (retval) {
                *retval = 0;
        }

        return ret;
}

int pthread_detach(pthread_t thread) {
        struct rtos_pthread* pthread = (struct rtos_pthread*)thread;
        int ret = 0;

        if (sem_wait(threads_mut, portMAX_DELAY) != pdTRUE) {
                errno = EAGAIN;
                errno = ENOMSG;
                return EAGAIN;
        }

        task_t task = rtos_pthread_find_handle(thread);
        if (!task) {
                errno = ESRCH;
                ret = ESRCH;
        } else {
                pthread->detached = true;
        }
        sem_post(threads_mut);
        return ret;
}

int phread_cancel(pthread_t pthread) {
        errno = ENOSYS;
        return ENOSYS;
}

pthread_t pthread_self() {
        if (!sem_wait(threads_mut, portMAX_DELAY)) {
                errno = EAGAIN;
                return EAGAIN;
        }
        struct rtos_pthread* pthread = rtos_pthread_find(xTaskGetCurrentTaskHandle());
        sem_post(threads_mut);
        if (!pthread) {
                errno = EINVAL;
        }
        return (pthread_t)pthread;
}

int pthread_equal(pthread_t t1, pthread_t t2) {
        return t1 == t2 ? 1 : 0;
}

int sched_yield() {
        task_delay(0);
        return 0;
}

// int pthread_once(pthread_once_t* once_ctrl, void (*init_routine)(void)) {
//   if(!once_ctrl || !init_routine || !once_ctrl->is_initialized) {
//     errno = EINVAL;
//     return EINVAL;
//   }
//
//   rtos_sched_stop();
//
// }

static int mutexattr_check(const pthread_mutexattr_t* attr) {
        if (attr->type < PTHREAD_MUTEX_NORMAL || attr->type > PTHREAD_MUTEX_RECURSIVE) {
                return EINVAL;
        }
        return 0;
}

int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr) {
        int type = PTHREAD_MUTEX_NORMAL;

        if (!mutex) {
                return EINVAL;
        }

        if (attr) {
                if (!attr->is_initialized) {
                        return EINVAL;
                }
                int res = mutexattr_check(attr);
                if (res) {
                        return res;
                }
                type = attr->type;
        }

        struct rtos_pthread_mutex* mux = (struct rtos_pthread_mutex*)pvPortMalloc(sizeof(struct rtos_pthread_mutex));
        if (!mux) {
                return ENOMEM;
        }
        mux->type = type;

        if (mux->type == PTHREAD_MUTEX_RECURSIVE) {
                mux->sem = mutex_recursive_create();
        } else {
                mux->sem = mutex_create();
        }
        if (!mux->sem) {
                vPortFree(mux);
                return EAGAIN;
        }

        *mutex = (pthread_mutex_t)mux;  // pointer value fit into pthread_mutex_t (uint32_t)

        return 0;
}

static int pthread_mutex_lock_internal(struct rtos_pthread_mutex* mux, uint32_t tmo) {
        if (mux->type == PTHREAD_MUTEX_RECURSIVE) {
                if (mutex_recursive_take(mux->sem, tmo) != pdTRUE) {
                        return EBUSY;
                }
        } else {
                if (sem_wait(mux->sem, tmo) != pdTRUE) {
                        return EBUSY;
                }
        }

        return 0;
}

int pthread_mutex_destroy(pthread_mutex_t* mutex) {
        if (!mutex) {
                return EINVAL;
        }
        struct rtos_pthread_mutex* mux = (struct rtos_pthread_mutex*)*mutex;

        // check if mux is busy
        int res = pthread_mutex_lock_internal(mux, 0);
        if (res == EBUSY) {
                return EBUSY;
        }

        sem_delete(mux->sem);
        vPortFree(mux);

        return 0;
}

static int pthread_mutex_init_if_static(pthread_mutex_t* mutex) {
        int res = 0;
        if (*mutex == PTHREAD_MUTEX_INITIALIZER) {
                portENTER_CRITICAL();
                if (*mutex == PTHREAD_MUTEX_INITIALIZER) {
                        res = pthread_mutex_init(mutex, NULL);
                }
                portEXIT_CRITICAL();
        }
        return res;
}

int pthread_mutex_lock(pthread_mutex_t* mutex) {
        if (!mutex) {
                return EINVAL;
        }
        int res = pthread_mutex_init_if_static(mutex);
        if (res != 0) {
                return res;
        }
        return pthread_mutex_lock_internal((struct rtos_pthread_mutex*)*mutex, portMAX_DELAY);
}

int pthread_mutex_trylock(pthread_mutex_t* mutex) {
        if (!mutex) {
                return EINVAL;
        }
        int res = pthread_mutex_init_if_static(mutex);
        if (res != 0) {
                return res;
        }
        return pthread_mutex_lock_internal((struct rtos_pthread_mutex*)*mutex, 0);
}

int pthread_mutex_unlock(pthread_mutex_t* mutex) {
        struct rtos_pthread_mutex* mux;

        if (!mutex) {
                return EINVAL;
        }
        mux = (struct rtos_pthread_mutex*)*mutex;

        if (mux->type == PTHREAD_MUTEX_RECURSIVE) {
                mutex_recursive_give(mux->sem);
        } else {
                sem_post(mux->sem);
        }
        return 0;
}

int pthread_mutexattr_init(pthread_mutexattr_t* attr) {
        if (!attr) {
                return EINVAL;
        }
        attr->type = PTHREAD_MUTEX_NORMAL;
        attr->is_initialized = 1;
        return 0;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t* attr) {
        if (!attr) {
                return EINVAL;
        }
        attr->is_initialized = 0;
        return 0;
}

int pthread_mutexattr_gettype(const pthread_mutexattr_t* attr, int* type) {
        return ENOSYS;
}

int pthread_mutexattr_settype(pthread_mutexattr_t* attr, int type) {
        if (!attr) {
                return EINVAL;
        }
        pthread_mutexattr_t tmp_attr = {.type = type };
        int res = mutexattr_check(&tmp_attr);
        if (!res) {
                attr->type = type;
        }
        return res;
}

int pthread_cond_wait(pthread_cond_t* a, pthread_mutex_t* b) {
        return 0;
}

int pthread_cond_signal(pthread_cond_t* a) {
        return 0;
}
*/