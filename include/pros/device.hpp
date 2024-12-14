/**
 * \file pros/device.hpp
 *
 * Base class for all smart devices.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2024, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \defgroup cpp-device VEX Generic Device C++ API (For Advanced Users)
 */

#ifndef _PROS_DEVICE_HPP_
#define _PROS_DEVICE_HPP_

#include "pros/misc.hpp"
#include "pros/rtos.hpp"

namespace pros {
inline namespace v5 {
/**
 * \ingroup cpp-device
 * \note These functions can be used for dynamic device instantiation. 
 */

/**
 * \addtogroup cpp-device
 *  @{
 */

/**
 * \enum DeviceType
 * \brief
 * Enum of possible v5 devices.
 *
 * Contains all current V5 Devices.
 */
enum class DeviceType {
	none = 0, ///< No device is plugged into the port
	motor = 2, ///< A motor is plugged into the port
	rotation = 4, ///< A rotation sensor is plugged into the port
	imu = 6, ///< An inertial sensor is plugged into the port
	distance = 7, ///< A distance sensor is plugged into the port
	radio = 8, ///< A radio is plugged into the port
	vision = 11, ///< A vision sensor is plugged into the port
	adi = 12, ///< This port is an ADI expander
	optical = 16, ///< An optical sensor is plugged into the port
	gps = 20, ///< A GPS sensor is plugged into the port
	aivision = 29, ///< An AI vision sensor is plugged into the port
	serial = 129, ///< A serial device is plugged into the port
	undefined = 255 ///< The device type is not defined, or is not a valid device
};

class Device {
	public:
  	/**
	 * Creates a Device object.
	 *
	 * \param port The V5 port number from 1-21
	 *
	 * \b Example
	 * \code
	 * #define DEVICE_PORT 1
	 *
	 * void opcontrol() {
	 *   Device device(DEVICE_PORT);
	 * }
	 * \endcode
	 */
  	explicit Device(const std::uint8_t port);
  
	/**
	 * Gets the port number of the Smart Device.
	 *
	 * \return The smart device's port number.
	 *
	* \b Example
 	 * \code
 	 * void opcontrol() {
	 * #define DEVICE_PORT 1
 	 *   while (true) {
	 *	Device device(DEVICE_PORT);
 	 *     printf("device plugged type: {port: %d}\n", device.get_port());
 	 *     delay(20);
 	 *   }
 	 * }
 	 * \endcode
	 */
	std::uint8_t get_port(void) const;

	/**
	 * Checks if the device is installed.
	 * 
	 * \return true if the corresponding device is installed, false otherwise.
	 * \b Example
	 *
 	 * \code
	 * #define DEVICE_PORT 1
	 *
	 * void opcontrol() {
	 *	Device device(DEVICE_PORT);
	 *   while (true) {
	 *     printf("device plugged type: {is_installed: %d}\n", device.is_installed());
	 *     delay(20);
	 *   }
	 * }
	 * \endcode
	*/
	virtual bool is_installed();

  	/**
	 * Gets the type of device.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Mutex of port cannot be taken (access denied).
	 * 
	 * \return The device type as an enum.
	 *
	 * \b Example
 	 * \code
	 * #define DEVICE_PORT 1
	 *
	 * void opcontrol() {
		Device device(DEVICE_PORT);
	 *   while (true) { 
	 *     DeviceType dt = device.get_plugged_type();
	 *     printf("device plugged type: {plugged type: %d}\n", dt);
	 *     delay(20);
	 *   }
	 * }
 	 * \endcode
	 */
	pros::DeviceType get_plugged_type() const;


	/**
	 * Gets the type of device on a given port.
	 * 
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Mutex of port cannot be taken (access denied).
	 * 
	 * \param port The V5 port number from 1-21
	 * 
	 * \return The device type as an enum.
	 *
	 * \b Example
 	 * \code
	 * #define DEVICE_PORT 1
	 *
	 * void opcontrol() {
	 *   while (true) { 
	 *     DeviceType dt = pros::Device::get_plugged_type(DEVICE_PORT);
	 *     printf("device plugged type: {plugged type: %d}\n", dt);
	 *     delay(20);
	 *   }
	 * }
 	 * \endcode
	 */
	static pros::DeviceType get_plugged_type(std::uint8_t port);

	/**
	 * Gets all devices of a given device type.
	 * 
	 * \param device_type The pros::DeviceType enum that matches the type of device desired.
	 * 
	 * \return A vector of Device objects for the given device type.
	 *
	 * \b Example
 	 * \code
	 * void opcontrol() {
	 *   std::vector<Device> motor_devices = pros::Device::get_all_devices(pros::DeviceType::motor);  // All Device objects are motors
	 * }
 	 * \endcode
	 */

	static std::vector<Device> get_all_devices(pros::DeviceType device_type = pros::DeviceType::undefined);

	protected:
	/**
	 * Creates a Device object.
	 *
	 * \param port The V5 port number from 1-21
	 * 
	 * \param deviceType The type of the constructed device
	 */
	Device(const std::uint8_t port, const enum DeviceType deviceType) : 
								_port(port), 
								_deviceType(deviceType) {}

	protected:
	const std::uint8_t _port;
	const enum DeviceType _deviceType = pros::DeviceType::none;
	
	///@}
};
}  // namespace v5
}  // namespace pros

#endif
