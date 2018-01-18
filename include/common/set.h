/**
 * system/common/set.h - kernel-allocated thread-safe simple sets header
 *
 * See system/common/set.c for discussion
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "kapi.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct set {
	uint32_t* arr;
	size_t used;
	size_t size;
	static_sem_s_t mtx_buf;
	mutex_t mtx;
};

void set_initialize(struct set* const set);
bool set_add(struct set* const set, uint32_t item);
bool set_rm(struct set* const set, uint32_t item);
bool set_contains(struct set* set, uint32_t item);

bool list_contains(uint32_t const* const list, const size_t size, const uint32_t item);
