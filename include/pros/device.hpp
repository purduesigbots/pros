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

/*
 * Enum of possible v5 devices.
 *
 * Contains all current V5 Devices.
 */
enum class DeviceType {
	none = 0,
	motor = 2,
	rotation = 4,
	imu = 6,
	distance = 7,
	radio = 8,
	vision = 11,
	adi = 12,
	optical = 16,
	gps = 20,
	serial = 129,
	generic __attribute__((deprecated("use E_DEVICE_SERIAL instead"))) = serial,
	undefined = 255
};

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
	virtual DeviceType get_type() const = 0;

	/**
	 * Checks if the device is installed.
	 * 
	 * \return true if the corresponding device is installed, false otherwise.
	*/
	virtual bool is_installed();

	protected:
	/**
	 * Creates a Device object.
	 *
	 * \param port The V5 port number from 1-21
	 * 
	 * \param deviceType The type of the constructed device
	 */
	Device(const std::uint8_t port, const enum DeviceType deviceType) : _port(port), _deviceType(deviceType) {}

	protected:
	const std::uint8_t _port;
	const enum DeviceType _deviceType;

	
};
}  // namespace v5
}  // namespace pros

#endif
