/**
 * \file vdml/port.h
 *
 * This file contains the standard header info for port macros and bit masks, 
 * used mostly for the adi expander.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _VDML_PORT_H_
#define _VDML_PORT_H_

#define SMART_PORT_BITS 16
#define SMART_PORT_MASK ((1 << SMART_PORT_BITS) - 1)

/**
 * Macro Description: Given a merged ports variable, it sets the smart port and adi port to the values inside the
 * int32_t.
 */
#define get_ports(ports, smart_port, adi_port) \
	{                                            \
		uint32_t uport = (uint32_t)ports;          \
		smart_port = uport & SMART_PORT_MASK;      \
		adi_port = uport >> SMART_PORT_BITS;       \
	}

static inline uint32_t merge_adi_ports(uint8_t smart_port, uint8_t adi_port) {
	return (adi_port << SMART_PORT_BITS) | smart_port;
}

#endif
