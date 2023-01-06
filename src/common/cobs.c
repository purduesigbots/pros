/**
 * \file common/cobs.c
 *
 * Consistent Overhead Byte Stuffing
 *
 * Contains an implementation of Consistent Overhead Byte Stuffing, adapted from
 * https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <string.h>

#include "cobs.h"

size_t cobs_encode_measure(const uint8_t* restrict src, const size_t src_len, const uint32_t prefix) {
	size_t read_idx = 0;
	size_t write_idx = 1;
	uint8_t code = 1;

	uint8_t* prefix_bytes = (uint8_t*)&(prefix);
	for (read_idx = 0; read_idx < 4; read_idx++) {
		if (prefix_bytes[read_idx] == 0) {
			code = 1;
			write_idx++;
		} else {
			write_idx++;
			code++;
			// code will never be 0xff since the length will always be 4
		}
	}
	read_idx = 0;

	while (read_idx < src_len) {
		if (src[read_idx] == 0) {
			code = 1;
			write_idx++;
			read_idx++;
		} else {
			write_idx++;
			read_idx++;
			code++;
			if (code == 0xff) {
				code = 1;
				write_idx++;
			}
		}
	}

	return write_idx;
}

int cobs_encode(uint8_t* restrict dest, const uint8_t* restrict src, const size_t src_len, const uint32_t prefix) {
	size_t read_idx = 0;
	size_t write_idx = 1;
	size_t code_idx = 0;
	uint8_t code = 1;

	uint8_t* prefix_bytes = (uint8_t*)&prefix;
	for (read_idx = 0; read_idx < 4;) {
		if (prefix_bytes[read_idx] == 0) {
			dest[code_idx] = code;
			code = 1;
			code_idx = write_idx++;
			read_idx++;
		} else {
			dest[write_idx++] = prefix_bytes[read_idx++];
			code++;
			// code will never be 0xff since the length of the prefix is 4
		}
	}
	read_idx = 0;

	while (read_idx < src_len) {
		if (src[read_idx] == 0) {
			dest[code_idx] = code;
			code = 1;
			code_idx = write_idx++;
			read_idx++;
		} else {
			dest[write_idx++] = src[read_idx++];
			code++;
			if (code == 0xff) {
				dest[code_idx] = code;
				code = 1;
				code_idx = write_idx++;
			}
		}
	}

	dest[code_idx] = code;

	return write_idx;
}
