/**
 * \file common/cobs.h
 *
 * Consistent Overhead Byte Stuffing header
 *
 * See common/cobs.c for discussion
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

#define COBS_ENCODE_MEASURE_MAX(src_len) ((src_len) + (((src_len) + 253) / 254))

/**
 * Encodes src in the Consistent Overhead Byte Stuffing algorithm, and writes
 * the result to dest. dest must be sufficiently long. use cobs_encode_measure()
 * to compute the size of the buff or use COBS_ENCODE_MEASURE_MAX(src_len) macro
 * to get the max buffer size needed (e.g. for static allocation)
 *
 * \param[out] dest
 *             The location to write the stuffed data to
 * \param[in] src
 *            The location of the incoming data
 * \param src_len
 *        The length of the source data
 * \param prefix
 *        The four character stream identifier
 *
 * \return The number of bytes written
 */
int cobs_encode(uint8_t* restrict dest, const uint8_t* restrict src, const size_t src_len, const uint32_t prefix);

/**
 * Same as cobs_encode() but doesn't write to an output buffer. Used to
 * determine how much space is needed for src.
 *
 * \param[in] src
 *            The location of the incoming data
 * \param src_len
 *        The length of the source data
 * \param prefix
 *        The four character stream identifier
 *
 * \return The size of src when encoded
 */
size_t cobs_encode_measure(const uint8_t* restrict src, const size_t src_len, const uint32_t prefix);
