/**
 * \file pros/device.hpp
 *
 * Base class for all smart devices.
 *
 *
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_DEVICE_HPP_
#define _PROS_DEVICE_HPP_

#include "pros/misc.hpp"
#include "pros/rtos.hpp"

namespace pros {
inline namespace v5 {
class Device {
	public:
	/**
	 * Gets the port number of the Smart Device.
	 *
	 * \return The smart device's port number.
	 */
	std::uint8_t get_port(void) {
		return _port;
	}

	/**
	 * Gets the type of device.
	 *
	 * \return The device type as an enum.
	 */
	virtual pros::DeviceType get_type() const = 0;

	protected:
	/**
	 * Creates a Device object.
	 *
	 * \param port The V5 port number from 1-21
	 *
	 */
	explicit Device(const std::uint8_t port) : _port(port) {}

	protected:
	const std::uint8_t _port;
};
}  // namespace v5
}  // namespace pros

#endif
