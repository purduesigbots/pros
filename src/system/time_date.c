/**
 * \file system/time_date.c
 *
 * Wrappers for the VEXos date and time functionality.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "kapi.h"
#include "v5_api.h"

date_s_t get_date(void) {
	date_s_t d;
	vexGetdate((struct date*)(&d));
	return d;
}

time_s_t get_time(void) {
	time_s_t t;
	vexGettime((struct time*)(&t));
	return t;
}
