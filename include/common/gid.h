/**
 * system/common/gid.h - Globally unique identifer facility header
 *
 * See system/common/gid.c for discussion
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdint.h>

#include "api.h"

struct gid_metadata {
  uint32_t *const bitmap; // a constant pointer to a bitmap
  const size_t max;       // Maximum gid value
  const size_t
      reserved; // first n GIDs may be reserved, at most 32, but at least 1
  const size_t bitmap_size; // Cached numebr of uint32_t's used to map gid_max.
                            // Use gid_size_to_words to compute

  // internal usage to ensure that GIDs get delegated linearly before wrapping
  // around back to 0
  size_t _cur_val;
  mutex_t _lock;
};

#define bmp_set(bmp, item) bmp[item / UINT32_WIDTH] |= (1 << (item % UINT32_WIDTH))
#define bmp_check(bmp, item) (bmp[item / UINT32_WIDTH] & (1 << (item % UINT32_WIDTH)))

#ifndef UINT32_WIDTH
#define UINT32_WIDTH 32
#endif

// convert the maximum number of gids into a number of words needed to store the
// bitmap
#define gid_size_to_words(size) (((size) + UINT32_WIDTH - 1) / UINT32_WIDTH)

// Initializes a gid_metadata structure by "freeing" all IDs in the bitmap
void gid_init(struct gid_metadata *const metadata);

// Allocates a gid from the gid structure and returns it. 0 is returned if there
// are no more gids left.
uint32_t gid_alloc(struct gid_metadata *const metadata);

// Frees the gid specified from the structure.
void gid_free(struct gid_metadata *const metadata, uint32_t id);

// Checks if the gid specified is allocated
bool gid_check(struct gid_metadata *metadata, uint32_t id);
