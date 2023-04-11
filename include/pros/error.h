/**
 * \file pros/error.h
 *
 * Contains macro definitions for return types, mostly errors
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _PROS_ERROR_H_
#define _PROS_ERROR_H_

#include "limits.h"

// Different Byte Size Errors

/// @brief
/// Return This on Byte Sized Return Error
#define PROS_ERR_BYTE (INT8_MAX)

/// @brief
/// Return This on 2 Byte Sized Return Error
#define PROS_ERR_2_BYTE (INT16_MAX)

/// @brief
/// Return This on 4 Byte Sized Return Error
#define PROS_ERR (INT32_MAX)

/// @brief
/// Return This on 8 Byte Sized Return Error
#define PROS_ERR_F (INFINITY)

/// @brief
/// Return This on Success (1)
#define PROS_SUCCESS (1)

#endif
