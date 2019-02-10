#include "kapi.h"
#include "common/linkedlist.h"

// NOTE: can't just include task.h because of redefinition that goes on in kapi
//       include chain, so we just prototype what we need here
void *pvTaskGetThreadLocalStoragePointer( task_t xTaskToQuery, int32_t xIndex );
void vTaskSetThreadLocalStoragePointer( task_t xTaskToSet, int32_t xIndex, void *pvValue );

#include "rtos/tcb.h"

// This increments configNUM_THREAD_LOCAL_STORAGE_POINTERS by 2

#define SUBSCRIBERS_TLSP_IDX 0
#define SUBSCRIPTIONS_TLSP_IDX 1

static static_sem_s_t task_notify_when_deleting_mutex_buf;
static mutex_t task_notify_when_deleting_mutex;

struct notify_delete_action {
  task_t task_to_notify;
  uint32_t value;
  notify_action_e_t notify_action;
};

struct _find_task_args {
  task_t task;
  struct notify_delete_action* found_action;
};

static void _find_task_cb(ll_node_s_t* node, void* extra) {
  struct notify_delete_action* action = node->payload.data;
  struct _find_task_args* args = extra;

  if(action->task_to_notify == args->task) {
    args->found_action = action;
  }
}

static struct notify_delete_action* _find_task(linked_list_s_t* ll, task_t task) {
  struct _find_task_args args = {
    .task = task,
    .found_action = NULL
  };
  linked_list_foreach(ll, _find_task_cb, &args);

  return args.found_action;
}

void task_notify_when_deleting_init() {
  task_notify_when_deleting_mutex = mutex_create_static(&task_notify_when_deleting_mutex_buf);
}

void task_notify_when_deleting(task_t target_task, task_t task_to_notify,
                               uint32_t value, notify_action_e_t notify_action) {
  task_to_notify = (task_to_notify == NULL) ? pxCurrentTCB : task_to_notify;
  target_task = (target_task == NULL) ? pxCurrentTCB : target_task;

  // It doesn't make sense for a task to notify itself, and make sure that
  // neither task is NULL (implying that scheduler hasn't started yet)
  if (task_to_notify == target_task || !task_to_notify || !target_task) {
    return;
  }

  mutex_take(task_notify_when_deleting_mutex, TIMEOUT_MAX);

  // task_to_notify maintains a list of the tasks whose deletion it cares about.
  // This will allow us to unsubscribe from notification if/when task_to_notify
  // is deleted
  linked_list_s_t* subscriptions_ll = pvTaskGetThreadLocalStoragePointer(task_to_notify, SUBSCRIPTIONS_TLSP_IDX);
  if (subscriptions_ll == NULL) {
    subscriptions_ll = linked_list_init();
    vTaskSetThreadLocalStoragePointer(task_to_notify, SUBSCRIPTIONS_TLSP_IDX, subscriptions_ll);
  }
  if (subscriptions_ll != NULL) {
    // check whether task_to_notify is already subscribed to target_task. if so,
    // do nothing
    ll_node_s_t* it = subscriptions_ll->head;
    bool found = false;
    while (it != NULL && !found) {
      found = it->payload.data == target_task;
      it = it->next;
    }
    if (!found) {
      linked_list_prepend_data(subscriptions_ll, target_task);
    }
  }

  // similarly, target_task maintains a list of the tasks it needs to notify
  // when being deleted
  linked_list_s_t* target_ll = pvTaskGetThreadLocalStoragePointer(target_task, SUBSCRIBERS_TLSP_IDX);
  if (target_ll == NULL) {
    target_ll = linked_list_init();
    vTaskSetThreadLocalStoragePointer(target_task, SUBSCRIBERS_TLSP_IDX, target_ll);
  }

  if (target_ll != NULL) {
    // Try to find the task_to_notify in the target linked list
    // i.e., target_task was already configured to notify task_to_notify
    struct notify_delete_action* action = _find_task(target_ll, task_to_notify);

    // action wasn't found, so add it to the linked list
    if (action == NULL) {
      action = (struct notify_delete_action*)kmalloc(sizeof(struct notify_delete_action));
      if (action != NULL) {
        linked_list_prepend_data(target_ll, action);
      }
    }

    // update the action (whether it was found or newly allocated)
    if (action != NULL) {
      action->task_to_notify = task_to_notify;
      action->notify_action = notify_action;
      action->value = value;
    }
  }
  mutex_give(task_notify_when_deleting_mutex);
}

// NOTE: this code is untested, probably works, but also has a terrible name (task_notify_when_deleting_unsubscribe)
// void task_notify_when_deleting_unsubscribe(task_t target_task, task_t task_to_notify) {
//   task_to_notify = (task_to_notify == NULL) ? pxCurrentTCB : task_to_notify;
//   target_task = (target_task == NULL) ? pxCurrentTCB : target_task;
//
//   if (task_to_notify == target_task || !task_to_notify || !target_task) {
//     return;
//   }
//
//   linked_list_s_t* ll = pvTaskGetThreadLocalStoragePointer(target_task, TLSP_IDX);
//   if (ll != NULL) {
//     struct notify_delete_action* action = _find_task(ll, task_to_notify);
//     if (action != NULL) {
//       linked_list_remove_data(ll, action);
//       kfree(action);
//     }
//   }
// }

static void unsubscribe_hook_cb(ll_node_s_t* node, void* task_to_remove) {
  task_t subscription = node->payload.data;

  linked_list_s_t* subscriptions_list = pvTaskGetThreadLocalStoragePointer(subscription, SUBSCRIBERS_TLSP_IDX);
  if (subscriptions_list != NULL) {
    linked_list_remove_data(subscriptions_list, task_to_remove);
  }
}

static void delete_hook_cb(ll_node_s_t* node, void* ignore) {
  struct notify_delete_action* action = node->payload.data;
  if (action != NULL) {
    task_notify_ext(action->task_to_notify, action->value, action->notify_action, NULL);
    kfree(action);
    node->payload.data = NULL;
  }
}

void task_notify_when_deleting_hook(task_t task) {
  mutex_take(task_notify_when_deleting_mutex, TIMEOUT_MAX);
  // if this task was subscribed to any other task deletion events, unsubscribe
  linked_list_s_t* ll = pvTaskGetThreadLocalStoragePointer(task, SUBSCRIPTIONS_TLSP_IDX);
  if (ll != NULL) {
    linked_list_foreach(ll, unsubscribe_hook_cb, task);
    linked_list_free(ll);
    vTaskSetThreadLocalStoragePointer(task, SUBSCRIPTIONS_TLSP_IDX, NULL);
  }
  // notify subscribed tasks of this task's deletion
  ll = pvTaskGetThreadLocalStoragePointer(task, SUBSCRIBERS_TLSP_IDX);
  if (ll != NULL) {
    linked_list_foreach(ll, delete_hook_cb, NULL);
    linked_list_free(ll);
    vTaskSetThreadLocalStoragePointer(task, SUBSCRIBERS_TLSP_IDX, NULL); // for good measure
  }
  mutex_give(task_notify_when_deleting_mutex);
}
