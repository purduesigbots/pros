/**
 * set.c - Simple sets
 *
 * Contains an implementation of a thread-safe basic set in the kernel heap.
 * It's used to check which streams are enabled in ser_driver for the moment,
 * but also has list_contains which may be useful in other contexts.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <string.h>

#include "common/set.h"
#include "kapi.h"
#include "system/optimizers.h"

/**
 * Initializes a a set.
 *
 * \param set
 *          A pointer to a set structure
 */
void set_initialize(struct set *const set) {
  set->arr = kmalloc(8 * sizeof(*(set->arr)));
  set->used = 0;
  set->size = 8;
  set->mtx = mutex_create_static(&(set->mtx_buf));
}

/**
 * Adds item to the set if it didn't already exist
 *
 * \param set
 *          A pointer to the set structure
 * \param item
 *          Item to add to the set
 * \return
 *          Returns true if the item was added to the set or was already present
 */
bool set_add(struct set *const set, uint32_t item) {
  size_t i = 0;
  if (!mutex_take(set->mtx, TIMEOUT_MAX)) {
    return false;
  }
  for (i = 0; i < set->used; i++) {
    if (set->arr[i] == item) {
      mutex_give(set->mtx);
      return true;
    }
  }
  if (set->used == set->size) {
    uint32_t *temp = set->arr;
    set->arr = kmalloc((set->size + 8) * sizeof(*(set->arr)));
    if (unlikely(set->arr == NULL)) {
      set->arr = temp;
      mutex_give(set->mtx);
      return false;
    }
    memcpy(set->arr, temp, set->size * sizeof(*(set->arr)));
    set->size += 8;
  }
  set->arr[set->used] = item;
  set->used++;
  mutex_give(set->mtx);
  return true;
}

/**
 * Removes an item from the set
 *
 * \param set
 *          A pointer to the set structure
 * \param item
 *          The item to remove
 * \return
 *          Returns true if the item was removed (or was already not present)
 */
bool set_rm(struct set *set, uint32_t item) {
  size_t i = 0;
  if (!mutex_take(set->mtx, TIMEOUT_MAX)) {
    return false;
  }
  for (i = 0; i < set->used - 1; i++) {
    if (set->arr[i] == item) {
      memcpy(set->arr + i, set->arr + i + 1, set->used - i - 1);
      set->used--;
      mutex_give(set->mtx);
      return true;
    }
  }
  if (set->arr[set->used] == item) {
    // this is the last item, no need to do memcpy, just decrement the counter
    set->used--;
  }
  mutex_give(set->mtx);
  return true;
}

/**
 * Checks if the set contains an item
 *
 * \param set
 *          A pointer to the set structure
 * \param item
 *          The item to check
 * \return
 *          Returns true if the item is in the set
 */
bool set_contains(struct set *set, uint32_t item) {
  if (!mutex_take(set->mtx, TIMEOUT_MAX)) {
    return false;
  }
  bool ret = list_contains(set->arr, set->used, item);
  mutex_give(set->mtx);
  return ret;
}

/**
 * Checks if the list contains an item
 *
 * \param list
 *          A pointer to a list of words
 * \param size
 *          The number of items in the list
 * \param item
 *          The item to check
 * \return
 *          Returns true if the item is in the list
 */
bool list_contains(uint32_t const *list, const size_t size,
                   const uint32_t item) {
  uint32_t const *const end = list + size;
  while (list <= end) {
    if (*list == item) {
      return true;
    }
    list++;
  }
  return false;
}
