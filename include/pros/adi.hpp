/**
 * \file pros/adi.hpp
 *
 * Contains prototypes for interfacing with the ADI.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/adi.html to learn more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_ADI_HPP_
#define _PROS_ADI_HPP_

#include "pros/adi.h"

#include <cstdint>

namespace pros {
class ADIPort {
	public:
	/**
	 * Configures an ADI port to act as a given sensor type.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of ADI Ports.
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param type
	 *        The configuration type for the port
	 */
	ADIPort(std::uint8_t port, adi_port_config_e_t type = E_ADI_TYPE_UNDEFINED);

	virtual ~ADIPort(void);

	/**
	 * Gets the configuration for the given ADI port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return The ADI configuration for the given port
	 */
	std::int32_t get_config(void) const;

	/**
	 * Gets the value for the given ADI port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return The value stored for the given port
	 */
	std::int32_t get_value(void) const;

	/**
	 * Configures an ADI port to act as a given sensor type.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param type
	 *        The configuration type for the port
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_config(adi_port_config_e_t type) const;

	/**
	 * Sets the value for the given ADI port.
	 *
	 * This only works on ports configured as outputs, and the behavior will
	 * change depending on the configuration of the port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param value
	 *        The value to set the ADI port to
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_value(std::int32_t value) const;

	protected:
	ADIPort(void);
	std::uint8_t _port;
};

class ADIAnalogIn : private ADIPort {
	public:
	/**
	 * Configures an ADI port to act as an Analog Input.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of ADI Ports.
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param type
	 *        The configuration type for the port
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	ADIAnalogIn(std::uint8_t port);

	/**
	 * Calibrates the analog sensor on the specified port and returns the new
	 * calibration value.
	 *
	 * This method assumes that the true sensor value is not actively changing at
	 * this time and computes an average from approximately 500 samples, 1 ms
	 * apart, for a 0.5 s period of calibration. The average value thus calculated
	 * is returned and stored for later calls to the
	 * pros::ADIAnalogIn::get_value_calibrated() and
	 * pros::ADIAnalogIn::get_value_calibrated_HR() functions. These functions
	 * will return the difference between this value and the current sensor value
	 * when called.
	 *
	 * Do not use this function when the sensor value might be unstable (gyro
	 * rotation, accelerometer movement).
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return The average sensor value computed by this function
	 */
	std::int32_t calibrate(void) const;

	/**
	 * Gets the 12 bit calibrated value of an analog input port.
	 *
	 * The pros::ADIAnalogIn::calibrate() function must be run first. This
	 * function is inappropriate for sensor values intended for integration, as
	 * round-off error can accumulate causing drift over time. Use
	 * pros::ADIAnalogIn::get_value_calibrated_HR() instead.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return The difference of the sensor value from its calibrated default from
	 * -4095 to 4095
	 */
	std::int32_t get_value_calibrated(void) const;

	/**
	 * Gets the 16 bit calibrated value of an analog input port.
	 *
	 * The pros::ADIAnalogIn::calibrate() function must be run first. This is
	 * intended for integrated sensor values such as gyros and accelerometers to
	 * reduce drift due to round-off, and should not be used on a sensor such as a
	 * line tracker or potentiometer.
	 *
	 * The value returned actually has 16 bits of "precision", even though the ADC
	 * only reads 12 bits, so that error induced by the average value being
	 * between two values when integrated over time is trivial. Think of the value
	 * as the true value times 16.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return The difference of the sensor value from its calibrated default from
	 * -16384 to 16384
	 */
	std::int32_t get_value_calibrated_HR(void) const;

	/**
	 * Gets the 12-bit value of the specified port.
	 *
	 * The value returned is undefined if the analog pin has been switched to a
	 * different mode.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return The analog sensor value, where a value of 0 reflects an input
	 * voltage of nearly 0 V and a value of 4095 reflects an input voltage of
	 * nearly 5 V
	 */
	using ADIPort::get_value;
};

using ADIPotentiometer = ADIAnalogIn;
using ADILineSensor = ADIAnalogIn;
using ADILightSensor = ADIAnalogIn;
using ADIAccelerometer = ADIAnalogIn;

class ADIAnalogOut : private ADIPort {
	public:
	/**
	 * Configures an ADI port to act as an Analog Output.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of ADI Ports.
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param type
	 *        The configuration type for the port
	 */
	ADIAnalogOut(std::uint8_t port);

	/**
	 * Sets the value for the given ADI port.
	 *
	 * This only works on ports configured as outputs, and the behavior will
	 * change depending on the configuration of the port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param value
	 *        The value to set the ADI port to
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	using ADIPort::set_value;
};

class ADIDigitalOut : private ADIPort {
	public:
	/**
	 * Configures an ADI port to act as a Digital Output.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of ADI Ports.
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param type
	 *        The configuration type for the port
	 */
	ADIDigitalOut(std::uint8_t port, bool init_state = LOW);

	/**
	 * Sets the value for the given ADI port.
	 *
	 * This only works on ports configured as outputs, and the behavior will
	 * change depending on the configuration of the port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param value
	 *        The value to set the ADI port to
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	using ADIPort::set_value;
};

class ADIDigitalIn : private ADIPort {
	public:
	/**
	 * Configures an ADI port to act as a Digital Input.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of ADI Ports.
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param type
	 *        The configuration type for the port
	 */
	ADIDigitalIn(std::uint8_t port);

	/**
	 * Gets a rising-edge case for a digital button press.
	 *
	 * This function is not thread-safe.
	 * Multiple tasks polling a single button may return different results under
	 * the same circumstances, so only one task should call this function for any
	 * given button. E.g., Task A calls this function for buttons 1 and 2. Task B
	 * may call this function for button 3, but should not for buttons 1 or 2. A
	 * typical use-case for this function is to call inside opcontrol to detect
	 * new button presses, and not in any other tasks.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return 1 if the button is pressed and had not been pressed the last time
	 * this function was called, 0 otherwise.
	 */
	std::int32_t get_new_press(void) const;

	/**
	 * Gets the value for the given ADI port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return The value stored for the given port
	 */
	using ADIPort::get_value;
};

using ADIButton = ADIDigitalIn;

class ADIMotor : private ADIPort {
	public:
	/**
	 * Configures an ADI port to act as a Motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of ADI Ports.
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param type
	 *        The configuration type for the port
	 */
	ADIMotor(std::uint8_t port);

	/**
	 * Stops the motor on the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t stop(void) const;

	/**
	 * Sets the speed of the motor on the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param value
	 *        The new signed speed; -127 is full reverse and 127 is full forward,
	 *        with 0 being off
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	using ADIPort::set_value;

	/**
	 * Gets the last set speed of the motor on the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return The last set speed of the motor on the given port
	 */
	using ADIPort::get_value;
};

class ADIEncoder : private ADIPort {
	public:
	/**
	 * Configures a set of ADI ports to act as an Encoder.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of ADI Ports.
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param port_top
	 *        The "top" wire from the encoder sensor with the removable cover side
	 *        UP
	 * \param port_bottom
	 *        The "bottom" wire from the encoder sensor
	 * \param reverse
	 *        If "true", the sensor will count in the opposite direction
	 */
	ADIEncoder(std::uint8_t port_top, std::uint8_t port_bottom, bool reversed = false);

	/**
	 * Sets the encoder value to zero.
	 *
	 * It is safe to use this method while an encoder is enabled. It is not
	 * necessary to call this method before stopping or starting an encoder.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t reset(void) const;

	/**
	 * Gets the number of ticks recorded by the encoder.
	 *
	 * There are 360 ticks in one revolution.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return The signed and cumulative number of counts since the last start or
	 * reset
	 */
	std::int32_t get_value(void) const;
};

class ADIUltrasonic : private ADIPort {
	public:
	/**
	 * Configures a set of ADI ports to act as an Ultrasonic.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - The given value is not within the range of ADI Ports.
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param port_echo
	 *        The port connected to the yellow INPUT cable. This should be in port
	 *        1, 3, 5, or 7 ('A', 'C', 'E', 'G').
	 * \param port_ping
	 *        The port connected to the orange OUTPUT cable. This should be in the
	 *        next highest port following port_echo.
	 */
	ADIUltrasonic(std::uint8_t port_echo, std::uint8_t port_ping);

	/**
	 * Gets the current ultrasonic sensor value in centimeters.
	 *
	 * If no object was found, zero is returned. If the ultrasonic sensor was
	 * never started, the return value is undefined. Round and fluffy objects can
	 * cause inaccurate values to be returned.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \return The distance to the nearest object in centimeters
	 */
	using ADIPort::get_value;
};
}  // namespace pros

#endif  // _PROS_ADI_HPP_
