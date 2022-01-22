/**
 * \file pros/device.hpp
 *
 * Contains prototypes for functions related to the VEX Smart Deivce.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/index.html to learn more.
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

#ifndef _PROS_DEVICE_HPP_
#define _PROS_DEVICE_HPP_

#include <cstdint>

#include "pros/device.h"

namespace pros {
class Device {
	public:
	/**
	 * Creates a Device object for the given port and type.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of V5 ports (1-21).
	 * ENODEV - The port cannot be configured as the expected type of device.
	 *
	 * \param port
	 *        The V5 port number from 1-21
	 * \param type
	 *        The expected type for the device
	 */
	explicit Device(const std::uint8_t port, pros::c::v5_device_e_t type = pros::c::E_DEVICE_UNDEFINED);

	/**
	 * Gets the port number of the device.
	 *
	 * \return The device's port number.
	 */
	std::uint8_t get_port();

	/**
	 * Gets the expecred type of the device.
	 *
	 * \return The device's expected type.
	 */
	pros::c::v5_device_e_t get_type();

	protected:
	const std::uint8_t _port; // starts from 1
	const pros::c::v5_device_e_t _type;
};
}

#endif
