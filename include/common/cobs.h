/**
 * system/common/cobs.h - Consistent overhead byte stuffing header
 *
 * See system/common/cobs.c for discussion
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdint.h>

#define COBS_ENCODE_MEASURE_MAX(src_len) ((src_len) + (((src_len) + 253) / 254))

size_t cobs_encode_measure(const uint8_t* restrict src, const size_t src_len, const uint32_t prefix);

// encodes src in the consistent overhead byte stuffing algorithm
// dest must be sufficiently long. use cobs_encode_measure to compute the size
// of the buff or use COBS_ENCODE_MEASURE_MAX(src_len) macro to get the max buffer
// size needed (e.g. for static allocation)
int cobs_encode(uint8_t* restrict dest, const uint8_t* restrict src, const size_t src_len, const uint32_t prefix);