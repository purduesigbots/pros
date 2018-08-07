/**
 * \file jinx_vdml.h
 *
 * \brief Contains VDML-specific applications of jinx
 *
 * This file contains functionality which automatically tracks
 * variables of different smart devices.
 *
 * \copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * LIcense, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef JINX_VDML_H
#define JINX_VDML_H

#include "kapi.h"

void initialize_tracker(void);
void update_all_variables(void);
void track_motor_vars(uint8_t port);
void track_vision_vars(uint8_t port);
void track_adi_vars(void);
#endif
