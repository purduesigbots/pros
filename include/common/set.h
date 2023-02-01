/**
 * \file common/set.h
 *
 * Kernel-allocated thread-safe simple sets header
 *
 * See common/set.c for discussion
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kapi.h"

struct set {
	uint32_t* arr;
	size_t used;
	size_t size;
	static_sem_s_t mtx_buf;
	mutex_t mtx;
};

/**
 * Initializes a a set.
 *
 * \param set
 *        A pointer to a set structure
 */
void set_initialize(struct set* const set);

/**
 * Adds item to the set if it didn't already exist
 *
 * \param set
 *        A pointer to the set structure
 * \param item
 *        Item to add to the set
 *
 * \return Ttrue if the item was added to the set or was already present
 */
bool set_add(struct set* const set, uint32_t item);

/**
 * Removes an item from the set
 *
 * \param set
 *        A pointer to the set structure
 * \param item
 *        The item to remove
 *
 * \return True if the item was removed (or was already not present)
 */
bool set_rm(struct set* const set, uint32_t item);

/**
 * Checks if the set contains an item
 *
 * \param set
 *        A pointer to the set structure
 * \param item
 *        The item to check
 *
 * \return True if the item is in the set
 */
bool set_contains(struct set* set, uint32_t item);

/**
 * Checks if the list contains an item
 *
 * \param list
 *        A pointer to a list of words
 * \param size
 *        The number of items in the list
 * \param item
 *        The item to check
 *
 * \return True if the item is in the list
 */
bool list_contains(uint32_t const* const list, const size_t size, const uint32_t item);
