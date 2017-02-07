/*
 * watchdog.h - IWDG API for reseting the cortex when something goes wrong
 *
 * Copyright (c) 2011-2016, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

// iwdgStart - Enables watchdog, must be called in InitializeIO
void iwdgEnable();

// iwdgStart - Initializes the watchdog and feed task
void iwdgStart();

// iwdgFeed - Resets the watchdog's timer
void iwdgFeed();

#endif