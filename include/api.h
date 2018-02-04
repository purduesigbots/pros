/**
 * api.h - PROS API header provides high-level user functionality
 *
 * Contains declarations for use by typical VEX programmers using PROS.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_API_H_
#define _PROS_API_H_

/**
 * If defined, some commonly used enums will have preprocessor macros which give a shorter,
 * more convenient naming pattern. If this isn't desired, simply comment the following line out
 */
#define PROS_USE_SIMPLE_NAMES

#ifdef __cplusplus
#include <cmath>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#else
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif

typedef bool bool_t;

#define PROS_VERSION_MAJOR 3
#define PROS_VERSION_MINOR 0
#define PROS_VERSION_PATCH 0
#define PROS_VERSION_STRING "3.0.0-beta1"

#define PROS_ERR (INT32_MAX)
#define PROS_ERR_F (INFINITY)

#include "pros/adi.h"
#include "pros/colors.h"
#include "pros/llemu.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.h"

#endif /* _PROS_API_H_ */
