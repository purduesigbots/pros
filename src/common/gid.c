/**
 * \file gid.c
 *
 * Globally Unique Identifiers Map
 *
 * Contains an implementation to efficiently assign globally unique IDs
 * e.g. to assign entries in a global table
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <limits.h>
#include <stdint.h>

#include "common/gid.h"
#include "kapi.h"

// Note: the V5 is a 32-bit architecture, so we'll use 32-bit integers

void gid_init(struct gid_metadata* const metadata) {
	// metadata arguments aren't checked for correctness since this is an
	// internal data structure
	size_t i;
	for (i = 0; i < metadata->bitmap_size; i++) {
		metadata->bitmap[i] = ~0;
	}

	metadata->bitmap[0] = (~0 << (metadata->reserved));

	metadata->_lock = mutex_create();
	return;
}

uint32_t gid_alloc(struct gid_metadata* const metadata) {
	if (mutex_take(metadata->_lock, TIMEOUT_MAX)) {
		size_t i;
		uint32_t gid = 0;
		uint32_t* gid_word = NULL;
		uint32_t gid_idx = 0;
		// check if the _cur_val + 1 is free
		if (!gid_check(metadata, (metadata->_cur_val + 1) % metadata->max)) {
			// that gid was free! so allocate it
			gid = metadata->_cur_val = (metadata->_cur_val + 1) % metadata->max;
			gid_word = metadata->bitmap + (gid / UINT32_WIDTH);
			gid_idx = gid % UINT32_WIDTH;
			goto return_gid;
		} else {
			for (i = 0; i < metadata->bitmap_size; i++) {
				gid_word = metadata->bitmap + i;
				if (*gid_word == 0) {  // all GIDs in this word are assigned
					continue;
				}
				// __builtin_ctz counts trailing zeros. This effectively returns the
				// position of the first unassigned gid withing the word
				gid_idx = __builtin_ctz(*gid_word);
				gid = gid_idx + (i * UINT32_WIDTH);
				// mark the id as allocated
				goto return_gid;
			}
		}
	return_gid:
		if (gid > metadata->max || gid == 0) {
			mutex_give(metadata->_lock);
			return 0;
		}
		*gid_word &= ~(1 << gid_idx);
		metadata->_cur_val = gid;
		mutex_give(metadata->_lock);
		return gid;
	}
	return 0;
}

void gid_free(struct gid_metadata* const metadata, uint32_t id) {
	if (id > metadata->max || id == 0) {
		return;
	}

	size_t word_idx = id / UINT32_WIDTH;
	metadata->bitmap[word_idx] |= 1 << (id % UINT32_WIDTH);
}

bool gid_check(struct gid_metadata* metadata, uint32_t id) {
	if (id > metadata->max) {
		return false;
	}

	size_t word_idx = id / UINT32_WIDTH;
	return (metadata->bitmap[word_idx] & (1 << (id % UINT32_WIDTH))) ? false : true;
}
