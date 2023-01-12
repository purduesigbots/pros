/**
 * \file common/gid.h
 *
 * Globally unique Identifer facility header
 *
 * See common/gid.c for discussion
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
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
	uint32_t* const bitmap;    // a constant pointer to a bitmap
	const size_t max;          // Maximum gid value
	const size_t reserved;     // first n GIDs may be reserved, at most 32, but at least 1
	const size_t bitmap_size;  // Cached number of uint32_t's used to map gid_max.
	                           // Use gid_size_to_words to compute

	// internal usage to ensure that GIDs get delegated linearly before wrapping
	// around back to 0
	size_t _cur_val;
	mutex_t _lock;
};

#ifndef UINT32_WIDTH
#define UINT32_WIDTH 32
#endif

/**
 * convert the maximum number of gids into a number of words needed to store the
 * bitmap
 */
#define gid_size_to_words(size) (((size) + UINT32_WIDTH - 1) / UINT32_WIDTH)

/**
 * Initializes a gid_metadata structure by "freeing" all IDs in the bitmap
 *
 * \param[in] metadata
 *            The gid_metadata structure to initialize
 */
void gid_init(struct gid_metadata* const metadata);

/**
 * Allocates a gid from the gid structure and returns it.
 *
 * \param[in] metadata
 *            The gid_metadata to record to the gid structure
 *
 * \return The gid, or 0 if there are no more gids left.
 */
uint32_t gid_alloc(struct gid_metadata* const metadata);

/**
 * Frees the gid specified from the structure.
 *
 * \param[in] metadata
 *            The gid_metadata to free from the gid structure
 * \param id
 *        The gid value indicating the metadata's position in the gid structure
 */
void gid_free(struct gid_metadata* const metadata, uint32_t id);

/**
 * Checks if the gid specified is allocated.
 *
 * \param[in] metadata
 *            The gid_metadata to check
 * \param id
 *        The gid value indicating the metadata's position in the gid structure
 *
 * \return True if the given metadata/id combo is present in the gid structure,
 * false otherwise.
 */
bool gid_check(struct gid_metadata* metadata, uint32_t id);
