/**
 * \file common/string.c
 *
 * Contains extra string functions useful for PROS and kstrdup/kstrndup which
 * use the kernel heap instead of the user heap
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <string.h>

#include "kapi.h"

char* kstrndup(const char* s, size_t n) {
	size_t copy_len = strnlen(s, n);  // strlen max'd out at n
	char* dupstr = kmalloc(n + 1);
	if (!dupstr) {
		return NULL;
	}
	memset(dupstr, 0, n + 1);
	memcpy(dupstr, s, copy_len);
	return dupstr;
}

char* kstrdup(const char* s) {
	return kstrndup(s, strlen(s));
}

void kprint_hex(uint8_t* s, size_t len) {
	for (size_t i = 0; i < len; i++) {
		if (i % 16 == 0) {
			printf("\n%u:\t", i);
		}
		printf("0x%02x ", s[i]);
	}
	printf("\n");
}
