/*
 * \file common/linkedlist.h
 *
 * Linked list implementation for internal use
 *
 * This file defines a linked list implementation that operates on the FreeRTOS
 * heap, and is able to generically store function pointers and data
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

typedef void (*generic_fn_t)(void);

typedef struct ll_node_s {
	union {
		generic_fn_t func; // Note: a list should not contain both data/funcs
		void* data;
	} payload;
	struct ll_node_s* next;
} ll_node_s_t;

typedef struct {
	ll_node_s_t* head;
} linked_list_s_t;

/**
 * Initialize a linked list node storing an arbitrary function pointer
 *
 * \param func
 *        Function pointer to store in the node
 *
 * \return A linked list node that stores a function pointer
 */
ll_node_s_t* linked_list_init_func_node(generic_fn_t func);

/**
 * Initialize a linked list node storing a pointer to arbitrary data
 *
 * \param[in] data
 *            Pointer to data
 *
 * \return A linked list node that stores some data
 */
ll_node_s_t* linked_list_init_data_node(void* data);

/**
 * Initialize a linked list
 *
 * \return An initialized linked list
 */
linked_list_s_t* linked_list_init();

/**
 * Prepend a node containing a function pointer to a linked list
 *
 * If the provided linked list is NULL, it will be initialized first.
 *
 * \param[in, out] list
 *                 Linked list to which the node will be prepended
 * \param func
 *        Function pointer with which to initialize the node
 */
void linked_list_prepend_func(linked_list_s_t* list, generic_fn_t func);

/**
 * Prepend a node containing some data to a linked list
 *
 * If the provided linked list is NULL, it will be initialized first.
 *
 * \param[in, out] list
 *                 Linked list to which the node will be prepended
 * \param[in] data
 *            Data with which to initialize the node
 */
void linked_list_prepend_data(linked_list_s_t* list, void* data);

/**
 * Append a node containing a function pointer to a linked list
 *
 * If the provided linked list is NULL, it will be initialized first.
 *
 * \param[in, out] list
 *                 Linked list to which the node will be appended
 * \param func
 *        Function pointer with which to initialize the node
 */
void linked_list_append_func(linked_list_s_t* list, generic_fn_t func);

/**
 * Removes the node containing the given function pointer from the linked list
 *
 * \param[in, out] list
 *                 Linked list from which the node will be removed
 * \param func
 *        Function pointer to be removed
 */
void linked_list_remove_func(linked_list_s_t* list, generic_fn_t func);

/**
 * Append a node containing some data to a linked list
 *
 * If the provided linked list is NULL, it will be initialized first.
 *
 * \param[in, out] list
 *                 Linked list to which the node will be appended
 * \param data
 *        Data with which to initialize the node
 */
void linked_list_append_data(linked_list_s_t* list, void* data);

/**
 * Remove the node containing the given data from the linked list
 *
 * \param[in, out] list
 *                 Linked list from which the node will be removed
 * \param data
 *        Data to be removed
 */
void linked_list_remove_data(linked_list_s_t* list, void* data);

typedef void (*linked_list_foreach_fn_t)(ll_node_s_t*, void*);

/**
 * Perform a function on every node in a linked list
 *
 * If the provided linked list is NULL, the function will terminate.
 *
 * \param list
 *        Linked list upon which to perform the function
 * \param cb
 *        Pointer to a callback function that will be provided the current node
 *        as well as some extra data
 * \param extra_data
 *        Extra data to pass to the callback function
 */
void linked_list_foreach(linked_list_s_t* list, linked_list_foreach_fn_t, void* extra_data);

/**
 * Frees a linked_list_s_t, making it no longer a valid list. This does not free any
 * internal data, only the linekd_list structure.
 *
 * \param list
 *				List to free
 */
void linked_list_free(linked_list_s_t* list);
