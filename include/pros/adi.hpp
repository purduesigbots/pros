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
 * Copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_ADI_HPP_
#define _PROS_ADI_HPP_

#include <cstdint>
#include <tuple>
#include <utility>

#include "pros/adi.h"

namespace pros {

/** type definition for the pair of smart port and adi port for the basic adi devices */
using ext_adi_port_pair_t = std::pair<std::uint8_t, std::uint8_t>;

/** type definition for the triplet of smart port and two adi ports for the two wire adi devices*/
using ext_adi_port_tuple_t = std::tuple<std::uint8_t, std::uint8_t, std::uint8_t>;

class ADIPort {
	public:
	/**
	 * Configures an ADI port to act as a given sensor type.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param adi_port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param type
	 *        The configuration type for the port
	 */
	explicit ADIPort(std::uint8_t adi_port, adi_port_config_e_t type = E_ADI_TYPE_UNDEFINED);

	/**
	 * Configures an ADI port on an adi expander to act as a given sensor type.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param port_pair
	 *        The pair of the smart port number (from 1-22) and the ADI port number
	 * 		  (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param type
	 * 		  The configuration type for the port
	 */
	ADIPort(ext_adi_port_pair_t port_pair, adi_port_config_e_t type = E_ADI_TYPE_UNDEFINED);

	/**
	 * Gets the configuration for the given ADI port.
	 *
	 * \return The ADI configuration for the given port
	 */
	std::int32_t get_config() const;

	/**
	 * Gets the value for the given ADI port.
	 *
	 * \return The value stored for the given port
	 */
	std::int32_t get_value() const;

	/**
	 * Configures an ADI port to act as a given sensor type.
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
	 * \param value
	 *        The value to set the ADI port to
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t set_value(std::int32_t value) const;

	protected:
	std::uint8_t _smart_port;
	std::uint8_t _adi_port;
};

class ADIAnalogIn : private ADIPort {
	public:
	/**
	 * Configures an ADI port to act as an Analog Input.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param adi_port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	explicit ADIAnalogIn(std::uint8_t adi_port);

	/**
	 * Configures an ADI port on an adi expander to act as an Analog Input.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param port_pair
	 *        The pair of the smart port number (from 1-22) and the
	 * 		  ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	ADIAnalogIn(ext_adi_port_pair_t port_pair);

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
	 * ENODEV - The port is not configured as an analog input
	 *
	 * \return The average sensor value computed by this function
	 */
	std::int32_t calibrate() const;

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
	 * ENODEV - The port is not configured as an analog input
	 *
	 * \return The difference of the sensor value from its calibrated default from
	 * -4095 to 4095
	 */
	std::int32_t get_value_calibrated() const;

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
	 * ENODEV - The port is not configured as an analog input
	 *
	 * \return The difference of the sensor value from its calibrated default from
	 * -16384 to 16384
	 */
	std::int32_t get_value_calibrated_HR() const;

	/**
	 * Gets the 12-bit value of the specified port.
	 *
	 * The value returned is undefined if the analog pin has been switched to a
	 * different mode.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as an analog input
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
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param adi_port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 */
	explicit ADIAnalogOut(std::uint8_t adi_port);

	/**
	 * Configures an ADI port on an adi_expander to act as an Analog Output.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param port_pair
	 *        The pair of the smart port number (from 1-22) and the
	 * 		  ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 *
	 */
	ADIAnalogOut(ext_adi_port_pair_t port_pair);

	/**
	 * Sets the value for the given ADI port.
	 *
	 * This only works on ports configured as outputs, and the behavior will
	 * change depending on the configuration of the port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as an analog output
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
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param adi_port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param init_state
	 *        The initial state for the port
	 */
	explicit ADIDigitalOut(std::uint8_t adi_port, bool init_state = LOW);

	/**
	 * Configures an ADI port on an adi_expander to act as a Digital Output.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param port_pair
	 *        The pair of the smart port number (from 1-22) and the
	 * 		  ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param init_state
	 *        The initial state for the port
	 */
	ADIDigitalOut(ext_adi_port_pair_t port_pair, bool init_state = LOW);

	/**
	 * Sets the value for the given ADI port.
	 *
	 * This only works on ports configured as outputs, and the behavior will
	 * change depending on the configuration of the port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a digital output
	 *
	 * \param value
	 *        The value to set the ADI port to
	 *
	 * \return if the operation was successful or PROS_ERR if the operation failed, setting errno.
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
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param adi_port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 */
	explicit ADIDigitalIn(std::uint8_t adi_port);

	/**
	 * Configures an ADI port on an adi_expander to act as a Digital Input.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param port_pair
	 *        The pair of the smart port number (from 1-22) and the
	 *  	  ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 */
	ADIDigitalIn(ext_adi_port_pair_t port_pair);

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
	 * ENODEV - The port is not configured as a digital input
	 *
	 * \return 1 if the button is pressed and had not been pressed the last time
	 * this function was called, 0 otherwise.
	 */
	std::int32_t get_new_press() const;

	/**
	 * Gets the value for the given ADI port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a digital input
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
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param adi_port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 */
	explicit ADIMotor(std::uint8_t adi_port);

	/**
	 * Configures an ADI port on an adi_expander to act as a Motor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param port_pair
	 *        The pair of the smart port number (from 1-22) and the
	 *  	  ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 */
	ADIMotor(ext_adi_port_pair_t port_pair);

	/**
	 * Stops the motor on the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a motor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t stop() const;

	/**
	 * Sets the speed of the motor on the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a motor
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
	 * ENODEV - The port is not configured as a motor
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
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param adi_port_top
	 *        The  "top" wire from the encoder sensor with the removable cover side up
	 * \param adi_port_bottom
	 *       The "bottom" wire from the encoder sensor
	 * \param reverse
	 *        If "true", the sensor will count in the opposite direction
	 */
	ADIEncoder(std::uint8_t adi_port_top, std::uint8_t adi_port_bottom, bool reversed = false);

	/**
	 * Configures a set of ADI ports on an adi_expander to act as an Encoder.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param port_tuple
	 *        The tuple of the smart port number, the "top" wire from the encoder
	 * 		  sensor with the removable cover side up, and the "bottom" wire from
	 * 		  the encoder sensor
	 * \param reverse
	 *        If "true", the sensor will count in theopposite direction
	 */
	ADIEncoder(ext_adi_port_tuple_t port_tuple, bool reversed = false);

	/**
	 * Sets the encoder value to zero.
	 *
	 * It is safe to use this method while an encoder is enabled. It is not
	 * necessary to call this method before stopping or starting an encoder.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a motor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t reset() const;

	/**
	 * Gets the number of ticks recorded by the encoder.
	 *
	 * There are 360 ticks in one revolution.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a motor
	 *
	 * \return The signed and cumulative number of counts since the last start or
	 * reset
	 */
	std::int32_t get_value() const;
};

class ADIUltrasonic : private ADIPort {
	public:
	/**
	 * Configures a set of ADI ports to act as an Ultrasonic sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param port_ping
	 *        The port connected to the orange OUTPUT cable. This should be in port
	 *        1, 3, 5, or 7 ('A', 'C', 'E', 'G').
	 * \param port_echo
	 *        The port connected to the yellow INPUT cable. This should be in the
	 *        next highest port following port_ping.
	 */
	ADIUltrasonic(std::uint8_t adi_port_ping, std::uint8_t adi_port_echo);

	/**
	 * Configures a set of ADI ports on an adi_expander to act as an Ultrasonic sensor.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param port_tuple
	 *        The tuple of the smart port number, the port connected to the orange
	 * 		  OUTPUT cable (1, 3, 5, 7 or 'A', 'C', 'E', 'G'), and the port 
	 * 		  connected to the yellow INPUT cable (the next) highest port 
	 * 		  following port_ping).
	 */
	ADIUltrasonic(ext_adi_port_tuple_t port_tuple);

	/**
	 * Gets the current ultrasonic sensor value in centimeters.
	 *
	 * If no object was found, zero is returned. If the ultrasonic sensor was
	 * never started, the return value is undefined. Round and fluffy objects can
	 * cause inaccurate values to be returned.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as an ultrasonic
	 *
	 * \return The distance to the nearest object in m^-4 (10000 indicates 1
	 * meter), measured from the sensor's mounting points.
	 */
	std::int32_t get_value() const;
};

class ADIGyro : private ADIPort {
	public:
	/**
	 * Initializes a gyroscope on the given port. If the given port has not
	 * previously been configured as a gyro, then this function starts a 1300ms
	 * calibration period.
	 *
	 * It is highly recommended that an ADIGyro object be created in initialize()
	 * when the robot is stationary to ensure proper calibration. If an ADIGyro
	 * object is declared at the global scope, a hardcoded 1300ms delay at the
	 * beginning of initialize will be necessary to ensure that the gyro's
	 * returned values are correct at the beginning of autonomous/opcontrol.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param adi_port
	 *        The ADI port to initialize as a gyro (from 1-8, 'a'-'h', 'A'-'H')
	 * \param multiplier
	 *        A scalar value that will be multiplied by the gyro heading value
	 *        supplied by the ADI
	 */
	explicit ADIGyro(std::uint8_t adi_port, double multiplier = 1);

	/**
	 * Initializes a gyroscope on the given port of an adi expander. If the given
	 * port has not previously been configured as a gyro, then this function starts
	 * a 1300ms calibration period.
	 *
	 * It is highly recommended that an ADIGyro object be created in initialize()
	 * when the robot is stationary to ensure proper calibration. If an ADIGyro
	 * object is declared at the global scope, a hardcoded 1300ms delay at the
	 * beginning of initialize will be necessary to ensure that the gyro's
	 * returned values are correct at the beginning of autonomous/opcontrol.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
 	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param port_pair
	 *        The pair of the smart port number (from 1-22) and the
	 *  	  ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param multiplier
	 *        A scalar value that will be multiplied by the gyro heading value
	 *        supplied by the ADI
	 */
	ADIGyro(ext_adi_port_pair_t port_pair, double multiplier = 1);

	/**
	 * Gets the current gyro angle in tenths of a degree. Unless a multiplier is
	 * applied to the gyro, the return value will be a whole number representing
	 * the number of degrees of rotation times 10.
	 *
	 * There are 360 degrees in a circle, thus the gyro will return 3600 for one
	 * whole rotation.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a gyro
	 *
	 * \return The gyro angle in degrees.
	 */
	double get_value() const;

	/**
	 * Resets the gyroscope value to zero.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a gyro
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 */
	std::int32_t reset() const;
};
}  // namespace pros

#endif  // _PROS_ADI_HPP_
