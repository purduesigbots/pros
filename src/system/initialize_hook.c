#include "stddef.h"
#include "common/linkedlist.h"

static linked_list_s_t* hook_list = NULL;

static void call_hook(struct ll_node_s* node, void* extra) {
    (void) extra;
    node->payload.func();
}

void add_initialize_hook(generic_fn_t hook_function) {
    linked_list_append_func(hook_list, hook_function);
}

void call_all_hooks() {
    linked_list_foreach(hook_list, &call_hook, NULL);
}