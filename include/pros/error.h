/**
 * \file pros/error.h
 *
 * Contains macro definitions for return types, mostly errors
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _PROS_ERROR_H_
#define _PROS_ERROR_H_

#include "limits.h"

// Different Byte Size Errors
#define PROS_ERR_BYTE (INT8_MAX)
#define PROS_ERR_2_BYTE (INT16_MAX)
#define PROS_ERR (INT32_MAX)
#define PROS_ERR_F (INFINITY)

// Return This on Success
#define PROS_SUCCESS (1)

#endif
