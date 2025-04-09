/**
 * \file pros/adi.hpp
 * \ingroup cpp-adi
 *
 * Contains prototypes for interfacing with the ADI.
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
 * \defgroup cpp-adi ADI (TriPort) C++ API
 * \note The external ADI API can be found [here.](@ref ext-adi)
 * \note Additional example code for this module can be found in its [Tutorial.](@ref adi)
 */

#ifndef _PROS_ADI_HPP_
#define _PROS_ADI_HPP_

#include <cstdint>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

#include "pros/adi.h"

#define LEGACY_TYPEDEF(old_name, new_name) using old_name [[deprecated("use " #new_name " instead")]] = new_name

namespace pros {
namespace adi {

/** type definition for the pair of smart port and adi port for the basic adi devices */
using ext_adi_port_pair_t = std::pair<std::uint8_t, std::uint8_t>;

/** type definition for the triplet of smart port and two adi ports for the two wire adi devices*/
using ext_adi_port_tuple_t = std::tuple<std::uint8_t, std::uint8_t, std::uint8_t>;

/**
 * \ingroup cpp-adi
 */
class Port {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */

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
	 *
	 * \b Example
	 * \code
	 * #define POTENTIOMETER_PORT 1
	 * #define POTENTIOMETER_TYPE pros::E_ADI_POT_EDR
	 *
	 * void opcontrol() {
	 *   pros::ADIPotentiometer potentiometer (POTENTIOMETER_PORT, POTENTIOMETER_TYPE);
	 *   while (true) {
	 *     // Get the potentiometer angle
	 *     std::cout << "Angle: " << potnetiometer.get_angle();
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	explicit Port(std::uint8_t adi_port, adi_port_config_e_t type = E_ADI_TYPE_UNDEFINED);

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
	 *
	 * \b Example
	 * \code
	 * #define ANALOG_SENSOR_PORT 'a'
	 * #define EXT_ADI_SMART_PORT 1
	 *
	 * void initialize() {
	 *   pros::adi::Port sensor ({EXT_ADI_SMART_PORT, ANALOG_SENSOR_PORT}, E_ADI_ANALOG_IN);
	 *   // Displays the value of E_ADI_ANALOG_IN
	 *   std::cout << "Port Type: " << sensor.get_config();
	 * }
	 * \endcode
	 */
	explicit Port(ext_adi_port_pair_t port_pair, adi_port_config_e_t type = E_ADI_TYPE_UNDEFINED);

	/**
	 * Gets the configuration for the given ADI port.
	 *
	 * \return The ADI configuration for the given port
	 *
	 * \b Example
	 * \code
	 * #define ANALOG_SENSOR_PORT 1
	 *   void initialize() {
	 *     adi_port_set_config(ANALOG_SENSOR_PORT, E_ADI_ANALOG_IN);
	 *     // Displays the value of E_ADI_ANALOG_IN
	 *     printf("Port Type: %d\n", adi_port_get_config(ANALOG_SENSOR_PORT));
	 *   }
	 * \endcode
	 */
	std::int32_t get_config() const;

	/**
	 * Gets the value for the given ADI port.
	 *
	 * \return The value stored for the given port
	 *
	 * \b Example
	 * \code
	 * #define ANALOG_SENSOR_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Port sensor (ANALOG_SENSOR_PORT, E_ADI_ANALOG_IN);
	 *   std::cout << "Port Value: " << sensor.get_value();
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 * #define ANALOG_SENSOR_PORT 1
	 *
	 * void initialize() {
	 *   pros::adi::Port sensor (ANALOG_SENSOR_PORT, E_ADI_DIGITAL_IN);
	 *   // Do things as a digital sensor
	 *   // Digital is unplugged and an analog is plugged in
	 *   sensor.set_config(E_ADI_ANALOG_IN);
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 * #define DIGITAL_SENSOR_PORT 1
	 *
	 * void initialize() {
	 *   pros::adi::Port sensor (DIGITAL_SENSOR_PORT, E_ADI_DIGITAL_OUT);
	 *   sensor.set_value(DIGITAL_SENSOR_PORT, HIGH);
	 * }
	 * \endcode
	 */
	std::int32_t set_value(std::int32_t value) const;

	/**
	 * Gets the port of the sensor.
	 *
	 * \return returns a tuple of integer ports.
	 *
	 * \note The parts of the tuple are {smart port, adi port, second adi port (when applicable)}.
	 *
	 *
	 * \b Example
	 * \code
	 * #define DIGITAL_SENSOR_PORT 1 // 'A'
	 *
	 * void initialize() {
	 *   pros::adi::AnalogIn sensor (DIGITAL_SENSOR_PORT);
	 *
	 * 	 // Getting values from the tuple using std::get<index>
	 * 	 int sensorSmartPort = std::get<0>(sensor.get_port()); // First value
	 *   int sensorAdiPort = std::get<1>(sensor.get_port()); // Second value
	 *
	 * 	 // Prints the first and second value from the port tuple (The Adi Port. The first value is the Smart Port)
	 *   printf("Sensor Smart Port: %d\n", sensorSmartPort);
	 *   printf("Sensor Adi Port: %d\n", sensorAdiPort);
	 * }
	 * \endcode
	 */
	virtual ext_adi_port_tuple_t get_port() const;

	protected:
	std::uint8_t _smart_port;
	std::uint8_t _adi_port;
};
///@}

class AnalogIn : protected Port {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */
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
	 *
	 * \b Example
	 * \code
	 * #define ANALOG_SENSOR_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::ADIAnalogIn sensor (ANALOG_SENSOR_PORT);
	 *   while (true) {
	 *     // Use the sensor
	 *   }
	 * }
	 * \endcode
	 */
	explicit AnalogIn(std::uint8_t adi_port);

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
	 *
	 * \b Example
	 * \code
	 * #define EXT_ADI_SENSOR_PORT 1
	 * #define ADI_PORT 'a'
	 *
	 * void opcontrol() {
	 *   pros::ADIAnalogIn sensor ({EXT_ADI_SMART_PORT, ADI_PORT});
	 *   while (true) {
	 *     // Use the sensor
	 *   }
	 * }
	 * \endcode
	 */
	explicit AnalogIn(ext_adi_port_pair_t port_pair);

	/**
	 * Calibrates the analog sensor on the specified port and returns the new
	 * calibration value.
	 *
	 * This method assumes that the true sensor value is not actively changing at
	 * this time and computes an average from approximately 500 samples, 1 ms
	 * apart, for a 0.5 s period of calibration. The average value thus calculated
	 * is returned and stored for later calls to the
	 * pros::AnalogIn::get_value_calibrated() and
	 * pros::AnalogIn::get_value_calibrated_HR() functions. These functions
	 * will return the difference between this value and the current sensor value
	 * when called.
	 *
	 * Do not use this function when the sensor value might be unstable (gyro
	 * rotation, accelerometer movement).
	 *
	 * \note The ADI currently returns data at 10ms intervals, in contrast to the
	 * calibrate function’s 1ms sample rate.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as an analog input
	 *
	 * \return The average sensor value computed by this function
	 *
	 * \b Example
	 * \code
	 * #define ANALOG_SENSOR_PORT 1
	 *
	 * void initialize() {
	 *   pros::adi::AnalogIn sensor (ANALOG_SENSOR_PORT);
	 *   sensor.calibrate(ANALOG_SENSOR_PORT);
	 *   std::cout << "Calibrated Reading:" << sensor.get_value_calibrated();
	 *   // All readings from then on will be calibrated
	 * }
	 * \endcode
	 */
	std::int32_t calibrate() const;

	/**
	 * Gets the 12 bit calibrated value of an analog input port.
	 *
	 * The pros::adi::AnalogIn::calibrate() function must be run first. This
	 * function is inappropriate for sensor values intended for integration, as
	 * round-off error can accumulate causing drift over time. Use
	 * pros::adi::AnalogIn::get_value_calibrated_HR() instead.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as an analog input
	 *
	 * \return The difference of the sensor value from its calibrated default from
	 * -4095 to 4095
	 *
	 * \b Example
	 * \code
	 * #define ANALOG_SENSOR_PORT 1
	 *
	 * void initialize() {
	 *   pros::adi::AnalogIn sensor (ANALOG_SENSOR_PORT);
	 *   sensor.calibrate(ANALOG_SENSOR_PORT);
	 *   std::cout << "Calibrated Reading:" << sensor.get_value_calibrated();
	 *   // All readings from then on will be calibrated
	 * }
	 * \endcode
	 */
	std::int32_t get_value_calibrated() const;

	/**
	 * Gets the 16 bit calibrated value of an analog input port.
	 *
	 * The pros::adi::AnalogIn::calibrate() function must be run first. This is
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
	 *
	 * \b Example
	 * \code
	 * #define ANALOG_SENSOR_PORT 1
	 *
	 * void initialize() {
	 *   pros::adi::AnalogIn sensor (ANALOG_SENSOR_PORT);
	 *   sensor.calibrate(ANALOG_SENSOR_PORT);
	 *   std::cout << "Calibrated Reading:" << sensor.get_value_calibrated();
	 *   // All readings from then on will be calibrated
	 * }
	 * \endcode
	 */
	std::int32_t get_value_calibrated_HR() const;

	/**
	 * Reads an analog input channel and returns the 12-bit value.
	 *
	 * The value returned is undefined if the analog pin has been switched to a different mode. The meaning of the
	 * returned value varies depending on the sensor attached.
	 *
	 * Inherited from ADIPort::get_value.
	 *
	 * This function uses the following values of errno when an error state is reached:
	 * EADDRINUSE - The port is not configured as an analog input (e.g. the port has been reconfigured)
	 *
	 * \return The analog sensor value, where a value of 0 reflects an input
	 * voltage of nearly 0 V and a value of 4095 reflects an input voltage of
	 * nearly 5 V
	 *
	 * \b Example
	 * \code
	 * #define ANALOG_SENSOR_PORT 1
	 *
	 * void initialize() {
	 *   pros::adi::AnalogIn sensor (ANALOG_SENSOR_PORT);
	 *   std::cout << "Sensor Reading:" << sensor.get_value();
	 * }
	 * \endcode
	 */
	using Port::get_value;

	/**
	 * This is the overload for the << operator for printing to streams
	 *
	 * Prints in format(this below is all in one line with no new line):
	 * AnalogIn [smart_port: analog_in._smart_port, adi_port: analog_in._adi_port,
	 * value calibrated: (12 bit calibrated value),
	 * value calibrated HR: (16 bit calibrated value), value: (12 bit value)]
	 */
	friend std::ostream& operator<<(std::ostream& os, pros::adi::AnalogIn& analog_in);

	using Port::get_port;
};

///@}

// using ADIPotentiometer = ADIAnalogIn;
using LineSensor = AnalogIn;
using LightSensor = AnalogIn;
using Accelerometer = AnalogIn;

class AnalogOut : private Port {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */
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
	 *
	 * \b Example
	 * \code
	 * #define ANALOG_SENSOR_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::AnalogOut sensor (ANALOG_SENSOR_PORT);
	 *   // Use the sensor
	 * }
	 * @endcode
	 */
	explicit AnalogOut(std::uint8_t adi_port);

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
	 * \b Example
	 * \code
	 * #define EXT_ADI_SMART_PORT 1
	 * #define ADI_PORT 'a'
	 *
	 * void opcontrol() {
	 *   pros::AnalogOut sensor ({EXT_ADI_SMART_PORT, ADI_PORT});
	 *   // Use the sensor
	 * }
	 * \endcode
	 */
	explicit AnalogOut(ext_adi_port_pair_t port_pair);

	/**
	 * Sets the output for the Analog Output from 0 (0V) to 4095 (5V).
	 *
	 * Inherited from ADIPort::set_value.
	 *
	 * This function uses the following values of errno when an error state is reached:
	 * EACCES - Another resource is currently trying to access the ADI.
	 *
	 * \param value
	 *        The value to set the ADI port to
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * #define ANALOG_SENSOR_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::AnalogOut sensor (ANALOG_SENSOR_PORT);
	 *   sensor.set_value(4095); // Set the port to 5V
	 * }
	 * \endcode
	 */
	using Port::set_value;

	using Port::get_port;

	/**
	 * This is the overload for the << operator for printing to streams
	 *
	 * Prints in format(this below is all in one line with no new line):
	 * AnalogOut [smart_port: analog_out._smart_port, adi_port: analog_out._adi_port,
	 * value: (value)]
	 */
	friend std::ostream& operator<<(std::ostream& os, pros::adi::AnalogOut& analog_out);
};
///@}

class DigitalOut : private Port {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */
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
	 *
	 * \b Example
	 * \code
	 * 	#define DIGITAL_SENSOR_PORT 1
	 *
	 * void opcontrol() {
	 *   bool state = LOW;
	 *   pros::adi::DigitalOut sensor (DIGITAL_SENSOR_PORT, state);
	 *   while (true) {
	 *     state != state;
	 *     sensor.set_value(state);
	 *     pros::delay(10); // toggle the sensor value every 50ms
	 *   }
	 * }
	 * \endcode
	 */
	explicit DigitalOut(std::uint8_t adi_port, bool init_state = LOW);

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
	 *
	 * \b Example
	 * \code
	 * #define EXT_ADI_SMART_PORT 1
	 * #define ADI_PORT 'a'
	 *
	 * void opcontrol() {
	 *   bool state = LOW;
	 *   pros::adi::DigitalOut sensor ({EXT_ADI_SMART_PORT , ADI_PORT});
	 *   while (true) {
	 *     state != state;
	 *     sensor.set_value(state);
	 *     pros::delay(10); // toggle the sensor value every 50ms
	 *   }
	 * }
	 * \endcode
	 */
	explicit DigitalOut(ext_adi_port_pair_t port_pair, bool init_state = LOW);

	/**
	 * Sets the digital value (1 or 0) of a pin.
	 *
	 * Inherited from ADIPort::set_value.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EADDRINUSE - The port is not configured as a digital output (e.g. the port has been reconfigured)
	 *
	 * \param value
	 *        The value to set the ADI port to
	 *
	 * \return if the operation was successful or PROS_ERR if the operation failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * #define DIGITAL_SENSOR_PORT 1
	 *
	 * void opcontrol() {
	 *   bool state = LOW;
	 *   pros::adi::DigitalOut sensor (DIGITAL_SENSOR_PORT);
	 *   while (true) {
	 *     state != state;
	 *     sensor.set_value(state);
	 *     pros::delay(10); // toggle the sensor value every 50ms
	 *   }
	 * }
	 * \endcode
	 */
	using Port::set_value;

	using Port::get_port;

	/**
	 * This is the overload for the << operator for printing to streams
	 *
	 * Prints in format(this below is all in one line with no new line):
	 * DigitalOut [smart_port: digital_out._smart_port, adi_port: digital_out._adi_port,
	 * value: (value)]
	 */
	friend std::ostream& operator<<(std::ostream& os, pros::adi::DigitalOut& digital_out);
};
///@}

class DigitalIn : private Port {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */
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
	 *
	 * \b Example
	 * \code
	 * #define DIGITAL_SENSOR_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::DigitalIn sensor (ANALOG_SENSOR_PORT);
	 *   // Use the sensor
	 * }
	 * \endcode
	 */
	explicit DigitalIn(std::uint8_t adi_port);

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
	 *
	 * \b Example
	 * \code
	 * #define EXT_ADI_SMART_PORT 1
	 * #define ADI_PORT 'a'
	 *
	 * void opcontrol() {
	 *   pros::adi::DigitalIn sensor ({EXT_ADI_SMART_PORT, ADI_PORT});
	 *   // Use the sensor
	 * }
	 * \endcode
	 */
	explicit DigitalIn(ext_adi_port_pair_t port_pair);

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
	 *
	 * \b Example
	 * \code
	 * #define DIGITAL_SENSOR_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::DigitalIn sensor (DIGITAL_SENSOR_PORT);
	 *   while (true) {
	 *     if (sensor.get_new_press()) {
	 *       // Toggle pneumatics or other state operations
	 *     }
	 *   pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_new_press() const;

	/**
	 * Gets the digital value (1 or 0) of a pin.
	 *
	 * Inherited from ADIPort::get_value.
	 *
	 * This function uses the following values of errno when an error state is reached:
	 *
	 * EADDRINUSE - The port is not configured as a digital input (e.g. the port has been reconfigured)
	 *
	 * Analogous to adi_digital_read.
	 *
	 * \return The value stored for the given port
	 *
	 * \b Example
	 * \code
	 * #define DIGITAL_SENSOR_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::DigitalIn sensor (DIGITAL_SENSOR_PORT);
	 *   while (true) {
	 *     std::cout << "Sensor Value:" << sensor.get_value();
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	using Port::get_value;

	/**
	 * This is the overload for the << operator for printing to streams
	 *
	 * Prints in format(this below is all in one line with no new line):
	 * DigitalIn [smart_port: digital_in._smart_port, adi_port: digital_in._adi_port,
	 * value: (value)]
	 */
	friend std::ostream& operator<<(std::ostream& os, pros::adi::DigitalIn& digital_in);

	using Port::get_port;
};

///@}

// Derived Class(es) from DigitalIn
using Button = DigitalIn;

class Motor : private Port {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */
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
	 *
	 * \b Example
	 * \code
	 * #define MOTOR_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Motor motor (MOTOR_PORT);
	 *   motor.set_value(127); // Go full speed forward
	 *   std::cout << "Commanded Motor Power: " << motor.get_value(); // Will display 127
	 *   delay(1000);
	 *   motor.set_value(0); // Stop the motor
	 * }
	 * \endcode
	 */
	explicit Motor(std::uint8_t adi_port);

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
	 *
	 * \b Example
	 * \code
	 * #define EXT_ADI_SMART_PORT 1
	 * #define ADI_MOTOR_PORT 'a'
	 *
	 * void opcontrol() {
	 *   pros::adi::Motor motor ({EXT_ADI_SMART_PORT, ADI_MOTOR_PORT});
	 *   motor.set_value(127); // Go full speed forward
	 *   std::cout << "Commanded Motor Power: " << motor.get_value(); // Will display 127
	 *   delay(1000);
	 *   motor.set_value(0); // Stop the motor
	 * }
	 * \endcode
	 */
	explicit Motor(ext_adi_port_pair_t port_pair);

	/**
	 * Stops the motor on the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a motor
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \b Example
	 * \code
	 * #define MOTOR_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Motor motor (MOTOR_PORT);
	 *   motor.set_value(127); // Go full speed forward
	 *   std::cout << "Commanded Motor Power: " << motor.get_value(); // Will display 127
	 *   delay(1000);
	 *   motor.stop(); // Stop the motor
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 * #define MOTOR_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Motor motor (MOTOR_PORT);
	 *   motor.set_value(127); // Go full speed forward
	 *   std::cout << "Commanded Motor Power: " << motor.get_value(); // Will display 127
	 *   delay(1000);
	 *   motor.set_value(0); // Stop the motor
	 * }
	 * \endcode
	 */
	using Port::set_value;

	/**
	 * Gets the last set speed of the motor on the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a motor
	 *
	 * \return The last set speed of the motor on the given
	 *
	 * \b Example
	 * \code
	 * #define MOTOR_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Motor motor (MOTOR_PORT);
	 *   motor.set_value(127); // Go full speed forward
	 *   std::cout << "Commanded Motor Power: " << motor.get_value(); // Will display 127
	 *   delay(1000);
	 *   motor.set_value(0); // Stop the motor
	 * }
	 * \endcode
	 */
	using Port::get_value;

	using Port::get_port;
};

///@}

class Encoder : private Port {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */
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
	 *
	 * \b Example
	 * \code
	 * #define PORT_TOP 1
	 * #define PORT_BOTTOM 2
	 *
	 * void opcontrol() {
	 *   pros::adi::Encoder sensor (PORT_TOP, PORT_BOTTOM, false);
	 *   // Use the sensor
	 * }
	 * \endcode
	 */
	explicit Encoder(std::uint8_t adi_port_top, std::uint8_t adi_port_bottom, bool reversed = false);

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
	 *
	 * \b Example
	 * \code
	 * #define PORT_TOP 'A'
	 * #define PORT_BOTTOM 'B'
	 * #define SMART_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Encoder sensor ({ SMART_PORT, PORT_TOP, PORT_BOTTOM }, false);
	 *   // Use the sensor
	 * }
	 * \endcode
	 */
	explicit Encoder(ext_adi_port_tuple_t port_tuple, bool reversed = false);

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
	 *
	 * \b Example
	 * \code
	 * #define PORT_TOP 1
	 * #define PORT_BOTTOM 2
	 *
	 * void opcontrol() {
	 *   pros::adi::Encoder sensor (PORT_TOP, PORT_BOTTOM, false);
	 *   delay(1000); // Move the encoder around in this time
	 *   sensor.reset(); // The encoder is now zero again
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 * #define PORT_TOP 1
	 * #define PORT_BOTTOM 2
	 *
	 * void opcontrol() {
	 *   pros::adi::Encoder sensor (PORT_TOP, PORT_BOTTOM, false);
	 *   while (true) {
	 *     std::cout << "Encoder Value: " << sensor.get_value();
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_value() const;

	/**
	 * This is the overload for the << operator for printing to streams
	 *
	 * Prints in format(this below is all in one line with no new line):
	 * Encoder [smart_port: encoder._smart_port, adi_port: encoder._adi_port,
	 * value: (value)]
	 */
	friend std::ostream& operator<<(std::ostream& os, pros::adi::Encoder& encoder);
	ext_adi_port_tuple_t get_port() const override;

	private:
	ext_adi_port_pair_t _port_pair;
};

///@}

class Ultrasonic : private Port {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */
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
	 *
	 * \b Example
	 * \code
	 * #define PORT_PING 1
	 * #define PORT_ECHO 2
	 *
	 * void opcontrol() {
	 *   pros::adi::Ultrasonic sensor (PORT_PING, PORT_ECHO);
	 *   while (true) {
	 *     // Print the distance read by the ultrasonic
	 *     std::cout << "Distance: " << sensor.get_value();
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	explicit Ultrasonic(std::uint8_t adi_port_ping, std::uint8_t adi_port_echo);

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
	 *
	 * \b Example
	 * \code
	 * #define PORT_PING 'A'
	 * #define PORT_ECHO 'B'
	 * #define SMART_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Ultrasonic sensor ( {{ SMART_PORT, PORT_PING, PORT_ECHO }} );
	 *   while (true) {
	 *     // Print the distance read by the ultrasonic
	 *     std::cout << "Distance: " << sensor.get_value();
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	explicit Ultrasonic(ext_adi_port_tuple_t port_tuple);

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
	 *
	 * \b Example
	 * \code
	 * #define PORT_PING 1
	 * #define PORT_ECHO 2
	 *
	 * void opcontrol() {
	 *   pros::adi::Ultrasonic sensor (PORT_PING, PORT_ECHO);
	 *   while (true) {
	 *     // Print the distance read by the ultrasonic
	 *     std::cout << "Distance: " << sensor.get_value();
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_value() const;

	using Port::get_port;
};

///@}

class Gyro : private Port {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */
	public:
	/**
	 * Initializes a gyroscope on the given port. If the given port has not
	 * previously been configured as a gyro, then this function starts a 1300ms
	 * calibration period.
	 *
	 * It is highly recommended that an Gyro object be created in initialize()
	 * when the robot is stationary to ensure proper calibration. If an Gyro
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
	 *        supplied by the
	 *
	 * \b Example
	 * \code
	 * #define GYRO_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Gyro gyro (GYRO_PORT);
	 *   while (true) {
	 *     // Get the gyro heading
	 *     std::cout << "Distance: " << gyro.get_value();
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	explicit Gyro(std::uint8_t adi_port, double multiplier = 1);

	/**
	 * Initializes a gyroscope on the given port of an adi expander. If the given
	 * port has not previously been configured as a gyro, then this function starts
	 * a 1300ms calibration period.
	 *
	 * It is highly recommended that an adi::Gyro object be created in initialize()
	 * when the robot is stationary to ensure proper calibration. If an adi::Gyro
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
	 *        supplied by the
	 *
	 * \b Example
	 * \code
	 * #define ADI_GYRO_PORT 'a'
	 * #define SMART_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Gyro gyro ({SMART_PORT ,ADI_GYRO_PORT});
	 *   while (true) {
	 *     // Get the gyro heading
	 *   std::cout << "Distance: " << gyro.get_value();
	 *   pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	explicit Gyro(ext_adi_port_pair_t port_pair, double multiplier = 1);

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
	 *
	 * \b Example
	 * \code
	 * #define GYRO_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Gyro gyro (GYRO_PORT);
	 *   while (true) {
	 *   // Get the gyro heading
	 *   std::cout << "Distance: " << gyro.get_value();
	 *   pros::delay(10);
	 *   }
	 * }
	 * \endcode
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
	 *
	 * \b Example
	 * \code
	 * #define GYRO_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Gyro gyro (GYRO_PORT);
	 *   std::uint32_t now = pros::millis();
	 *   while (true) {
	 *     // Get the gyro heading
	 *     std::cout << "Distance: " << gyro.get_value();
	 *
	 *   if (pros::millis() - now > 2000) {
	 *     // Reset the gyro every 2 seconds
	 *     gyro.reset();
	 *     now = pros::millis();
	 *   }
	 *
	 *   pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t reset() const;

	using Port::get_port;
};

///@}

class Potentiometer : public AnalogIn {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */
	public:
	/**
	 * Configures an ADI port to act as a Potentiometer.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param adi_port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param potentiometer_type
	 *        An adi_potentiometer_type_e_t enum value specifying the potentiometer version type
	 *
	 * \b Example
	 * \code
	 * #define POTENTIOMETER_PORT 1
	 * #define POTENTIOMETER_TYPE pros::E_ADI_POT_EDR
	 *
	 * void opcontrol() {
	 *   pros::adi::Potentiometer potentiometer (POTENTIOMETER_PORT, POTENTIOMETER_TYPE);
	 *   while (true) {
	 *     // Get the potentiometer angle
	 *     std::cout << "Angle: " << potentiometer.get_angle();
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	explicit Potentiometer(std::uint8_t adi_port, adi_potentiometer_type_e_t potentiometer_type = E_ADI_POT_EDR);

	/**
	 * Configures an ADI port on an adi_expander to act as a Potentiometer.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param adi_port
	 *        The pair of the smart port number (from 1-22) and the
	 *  	  ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param potentiometer_type
	 *        An adi_potentiometer_type_e_t enum value specifying the potentiometer version type
	 *
	 * \b Example
	 * \code
	 * #define ADI_POTENTIOMETER_PORT 'a'
	 * #define SMART_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Potentiometer potentiometer ({SMART_PORT, ADI_POTENTIOMETER_PORT});
	 *   while (true) {
	 *     // Get the potentiometer angle
	 *     std::cout << "Angle: " << potentiometer.get_angle();
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	explicit Potentiometer(ext_adi_port_pair_t port_pair, adi_potentiometer_type_e_t potentiometer_type = E_ADI_POT_EDR);

	/**
	 * Gets the current potentiometer angle in tenths of a degree.
	 *
	 * The original potentiometer rotates 250 degrees thus returning an angle between 0-250 degrees.
	 * Potentiometer V2 rotates 330 degrees thus returning an angle between 0-330 degrees.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of ADI Ports
	 * EADDRINUSE - The port is not configured as a potentiometer
	 *
	 * \return The potentiometer angle in degrees.
	 *
	 * \b Example
	 * \code
	 * #define ADI_POTENTIOMETER_PORT 'a'
	 * #define SMART_PORT 1
	 *
	 * void opcontrol() {
	 *   pros::adi::Potentiometer potentiometer ({{ SMART_PORT , ADI_POTENTIOMETER_PORT }});
	 *   while (true) {
	 *     // Get the potentiometer angle
	 *     std::cout << "Angle: " << potentiometer.get_angle();
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	double get_angle() const;

	/**
	 * Gets the 12-bit value of the specified port.
	 *
	 * The value returned is undefined if the analog pin has been switched to a
	 * different mode.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a potentiometer
	 *
	 * \return The analog sensor value, where a value of 0 reflects an input
	 * voltage of nearly 0 V and a value of 4095 reflects an input voltage of
	 * nearly 5 V
	 */
	using AnalogIn::get_value;

	/**
	 * Calibrates the potentiometer on the specified port and returns the new
	 * calibration value.
	 *
	 * This method assumes that the potentiometer value is not actively changing at
	 * this time and computes an average from approximately 500 samples, 1 ms
	 * apart, for a 0.5 s period of calibration. The average value thus calculated
	 * is returned and stored for later calls to the
	 * pros::adi::Potentiometer::get_value_calibrated() function. This function
	 * will return the difference between this value and the current sensor value
	 * when called.
	 *
	 * Do not use this function when the potentiometer value might be unstable (rotating the potentiometer)
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a potentiometer
	 *
	 * \return The average potentiometer value computed by this function
	 */
	using AnalogIn::calibrate;

	/**
	 * Gets the 12 bit calibrated value of a potentiometer port.
	 *
	 * The pros::adi::Potentiometer::calibrate() function must be run first.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENODEV - The port is not configured as a potentiometer
	 *
	 * \return The difference of the potentiometer value from its calibrated default from
	 * -4095 to 4095
	 */
	using AnalogIn::get_value_calibrated;

	/**
	 * This is the overload for the << operator for printing to streams
	 * Potentiometer [value: (value), value calibrated: (calibrated value),
	 * angle: (angle)]
	 * Prints in format(this below is all in one line with no new line):
	 */
	friend std::ostream& operator<<(std::ostream& os, pros::adi::Potentiometer& potentiometer);

	using Port::get_port;
};

///@}

class Led : protected Port {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */
	public:
	/**
	 * @brief Configures an ADI port to act as a LED.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param adi_port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param length
	 *        The number of LEDs in the chain
	 *
	 * \b Example:
	 * \code
	 * #define LED_PORT 'a'
	 * #define LED_LENGTH 3
	 *
	 * void opcontrol() {
	 *   pros::Led led (LED_PORT, LED_LENGTH);
	 *   while (true) {
	 * 	   // Set entire LED strip to red
	 *     led.set_all(0xFF0000);
	 * 	   pros::delay(20);
	 * 	 }
	 * }
	 * \endcode
	 *
	 */
	explicit Led(std::uint8_t adi_port, std::uint32_t length);

	/**
	 * @brief Configures an ADI port on a adi_expander to act as a LED.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - Either the ADI port value or the smart port value is not within its
	 *	   valid range (ADI port: 1-8, 'a'-'h', or 'A'-'H'; smart port: 1-21).
	 *
	 * \param port_pair
	 *        The pair of the smart port number (from 1-22) and the
	 *  	  ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param length
	 * 	  The number of LEDs in the chain
	 *
	 * \b Example:
	 * \code
	 * #define LED_PORT 'a'
	 * #define SMART_PORT 1
	 * #define LED_LENGTH 3
	 *
	 * void opcontrol() {
	 *   pros::Led led ({SMART_PORT, LED_PORT}, LED_LENGTH);
	 *   while (true) {
	 *     // Set entire LED strip to red
	 *     led.set_all(0xFF0000);
	 * 	   pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	explicit Led(ext_adi_port_pair_t port_pair, std::uint32_t length);

	/**
	 * @brief Operator overload to access the buffer in the ADILed class, it is
	 * recommended that you call .update(); after doing any operations with this.
	 *
	 * @param i 0 indexed pixel of the lED
	 * @return uint32_t& the address of the buffer at i to modify
	 *
	 * \b Example:
	 * \code
	 * #define LED_PORT 'a'
	 * #define LED_LENGTH 3
	 *
	 * void opcontrol() {
	 *   pros::Led led (LED_PORT, LED_LENGTH);
	 *   while (true) {
	 * 	   // Set the first 3 pixels to red, green, and blue
	 * 	   led.set_pixel(0xFF0000, 0);
	 * 	   led.set_pixel(0x00FF00, 1);
	 * 	   led.set_pixel(0x0000FF, 2);
	 * 	   pros::delay(20);
	 *
	 * 	   // Use the [] operator to set the first pixel to black
	 * 	   led.operator[](0) = 0x000000;
	 * 	   led.update();
	 * 	   pros::delay(20);
	 *   }
	 * }
	 */
	std::uint32_t& operator[](size_t i);

	/**
	 * @brief Clear the entire led strip of color
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of ADI Ports
	 * EINVAL - A parameter is out of bounds/incorrect
	 * EADDRINUSE - The port is not configured for ADI output
	 *
	 * @return PROS_SUCCESS if successful, PROS_ERR if not
	 *
	 * \b Example:
	 * \code
	 * #define LED_PORT 'a'
	 * #define LED_LENGTH 3
	 *
	 * void opcontrol() {
	 *   pros::Led led (LED_PORT, LED_LENGTH);
	 *   while (true) {
	 * 	   // Set the first 3 pixels to red, green, and blue
	 * 	   led.set_pixel(0xFF0000, 0);
	 * 	   led.set_pixel(0x00FF00, 1);
	 * 	   led.set_pixel(0x0000FF, 2);
	 * 	   pros::delay(20);
	 *
	 * 	   // Clear the led strip of color
	 * 	   led.clear();
	 * 	   pros::delay(20);
	 *  }
	 * }
	 * \endcode
	 */
	std::int32_t clear_all();
	std::int32_t clear();

	/**
	 * @brief Force the LED strip to update with the current buffered values, this
	 * should be called after any changes to the buffer using the [] operator.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - A parameter is out of bounds/incorrect
	 * EADDRINUSE - The port is not configured for ADI output
	 *
	 * @return PROS_SUCCESS if successful, PROS_ERR if not
	 *
	 * \b Example:
	 * \code
	 * #define LED_PORT 'a'
	 * #define LED_LENGTH 3
	 *
	 * void opcontrol() {
	 *   pros::Led led (LED_PORT, LED_LENGTH);
	 *   while (true) {
	 * 	   // Set the first 3 pixels to red, green, and blue
	 * 	   led.set_pixel(0xFF0000, 0);
	 * 	   led.set_pixel(0x00FF00, 1);
	 * 	   led.set_pixel(0x0000FF, 2);
	 * 	   pros::delay(20);
	 *
	 * 	   // Use the [] operator to set the first pixel to black
	 * 	   led.operator[](0) = 0x000000;
	 *      // Update the led strip with the new values
	 * 	   led.update();
	 * 	   pros::delay(20);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t update() const;

	/**
	 * @brief Set the entire led strip to one color
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - A parameter is out of bounds/incorrect
	 * EADDRINUSE - The port is not configured for ADI output
	 *
	 * @param color color to set all the led strip value to
	 * @return PROS_SUCCESS if successful, PROS_ERR if not
	 *
	 * \b Example:
	 * \code
	 * #define LED_PORT 'a'
	 * #define LED_LENGTH 3
	 *
	 * void opcontrol() {
	 *   pros::Led led (LED_PORT, LED_LENGTH);
	 *   while (true) {
	 * 	   // Set the entire led strip to blue
	 * 	   led.set_all(0x0000FF);
	 * 	   pros::delay(20);
	 * 	 }
	 * }
	 * \endcode
	 */
	std::int32_t set_all(uint32_t color);

	/**
	 * @brief Set one pixel on the led strip
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - A parameter is out of bounds/incorrect
	 * EADDRINUSE - The port is not configured for ADI output
	 *
	 * @param color color to clear all the led strip to
	 * @param pixel_position position of the pixel to clear
	 * @return PROS_SUCCESS if successful, PROS_ERR if not
	 *
	 * \b Example:
	 * \code
	 * #define LED_PORT 'a'
	 * #define LED_LENGTH 3
	 *
	 * void opcontrol() {
	 *   pros::Led led (LED_PORT, LED_LENGTH);
	 *   while (true) {
	 * 	   // Set the first pixel to blue
	 * 	   led.set_pixel(0x0000FF, 0);
	 * 	   pros::delay(20);
	 * 	 }
	 * }
	 * \endcode
	 */
	std::int32_t set_pixel(uint32_t color, uint32_t pixel_position);

	/**
	 * @brief Clear one pixel on the led strip
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - A parameter is out of bounds/incorrect
	 * EADDRINUSE - The port is not configured for ADI output
	 *
	 * @param pixel_position position of the pixel to clear
	 * @return PROS_SUCCESS if successful, PROS_ERR if not
	 *
	 * \b Example:
	 * \code
	 * #define LED_PORT 'a'
	 * #define LED_LENGTH 3
	 *
	 * void opcontrol() {
	 *   pros::Led led (LED_PORT, LED_LENGTH);
	 *   while (true) {
	 * 	   // Set the first pixel to blue
	 * 	   led.set_pixel(0x0000FF, 0);
	 * 	   pros::delay(20);
	 *
	 * 	   // Clear the first pixel
	 * 	   led.clear_pixel(0);
	 * 	   pros::delay(20);
	 * 	}
	 * }
	 * \endcode
	 */
	std::int32_t clear_pixel(uint32_t pixel_position);

	/**
	 * @brief Get the length of the led strip
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EINVAL - A parameter is out of bounds/incorrect
	 * EADDRINUSE - The port is not configured for ADI output
	 *
	 * @return The length (in pixels) of the LED strip
	 *
	 * \b Example:
	 * \code
	 * #define LED_PORT 'a'
	 * #define LED_LENGTH 3
	 *
	 * void opcontrol() {
	 *   pros::Led led (LED_PORT, LED_LENGTH);
	 *   while (true) {
	 * 	   // Get the length of the led strip
	 * 	   int length = led.length();
	 * 	   pros::lcd::print(1, "Length: %d", length);
	 * 	   pros::delay(20);
	 * 	 }
	 * }
	 * \endcode
	 */
	std::int32_t length();

	using Port::get_port;

	protected:
	std::vector<uint32_t> _buffer;
};
///@}

/// @brief Alias for ADILed
using LED = Led;

class Pneumatics : public DigitalOut {
	/**
	 * \addtogroup cpp-adi
	 *  @{
	 */
	public:
	/**
	 * Creates a Pneumatics object for the given port.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of ADI Ports
	 *
	 * \param adi_port
	 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param start_extended
	 * 		  If true, the pneumatic will start extended when the program starts.
	 *		  By default, the piston starts retracted when the program starts.
	 * \param extended_is_low
	 * 		  A flag to set whether the the pneumatic is extended when the ADI
	 * 		  it receives a high or a low value. When true, the extended state
	 * 		  corresponds to a output low on the ADI port. This allows the user
	 * 		  to reverse the behavior of the pneumatics if needed.
	 *
	 * /b Example:
	 * \code
	 * void opcontrol() {
	 * 	 pros::adi::Pneumatics left_piston('a', false);			// Starts retracted, extends when the ADI port is high
	 *   pros::adi::Pneumatics right_piston('b', false, true);	// Starts retracted, extends when the ADI port is low
	 *
	 *   pros::Controller master(pros::E_CONTROLLER_MASTER);
	 *
	 *   while (true) {
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
	 *       left_piston.extend();
	 *     }
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
	 *       left_piston.retract();
	 *     }
	 *
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
	 *       left_piston.extend();
	 *     }
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_2)) {
	 *       left_piston.retract();
	 *     }
	 *
	 *     pros::delay(10);
	 *   }
	 *
	 * \endcode
	 */
	explicit Pneumatics(std::uint8_t adi_port, bool start_extended, bool extended_is_low = false);

	/**
	 * Creates a Pneumatics object for the given port pair.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * ENXIO - The given value is not within the range of ADI Ports
	 *
	 * \param port_pair
	 *        The pair of the smart port number (from 1-22) and the
	 *  	  ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
	 * \param start_extended
	 * 		  If true, the pneumatic will start extended when the program starts.
	 *		  By default, the piston starts retracted when the program starts.
	 * \param extended_is_low
	 * 		  A flag to set whether the the pneumatic is extended when the ADI
	 * 		  it receives a high or a low value. When true, the extended state
	 * 		  corresponds to a output low on the ADI port. This allows the user
	 * 		  to reverse the behavior of the pneumatics if needed.
	 *
	 * /b Example:
	 * \code
	 * void opcontrol() {
	 * 	 pros::adi::Pneumatics left_piston({1, 'a'}, false);			// Starts retracted, extends when the ADI port is high
	 *   pros::adi::Pneumatics right_piston({1, 'b'}, false, true);	    // Starts retracted, extends when the ADI port is
	 *low
	 *
	 *   pros::Controller master(pros::E_CONTROLLER_MASTER);
	 *
	 *   while (true) {
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
	 *       left_piston.extend();
	 *     }
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
	 *       left_piston.retract();
	 *     }
	 *
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
	 *       left_piston.extend();
	 *     }
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
	 *       left_piston.retract();
	 *     }
	 *
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	explicit Pneumatics(ext_adi_port_pair_t port_pair, bool start_extended, bool extended_is_low = false);

	/**
	 * Extends the piston, if not already extended.
	 *
	 * \return 1 if the piston newly extended, 0 if the piston was already
	 *         extended, or PROS_ERR is the operation failed, setting errno.
	 *
	 * \b Example:
	 * \code
	 * void opcontrol() {
	 * 	 pros::adi::Pneumatics piston({1, 'a'}, false);            // Starts retracted, extends when the ADI port is high
	 *
	 *   pros::Controller master(pros::E_CONTROLLER_MASTER);
	 *
	 *   while (true) {
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
	 *       left_piston.extend();
	 *     }
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
	 *       left_piston.retract();
	 *     }
	 *     if(mastetr.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
	 *       left_piston.toggle();
	 *     }
	 *
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t extend();

	/**
	 * Retracts the piston, if not already retracted.
	 *
	 * \return 1 if the piston newly retracted, 0 if the piston was already
	 *         retracted, or PROS_ERR is the operation failed, setting errno.
	 *
	 * \b Example:
	 * \code
	 * void opcontrol() {
	 * 	 pros::adi::Pneumatics piston({1, 'a'}, false);            // Starts retracted, extends when the ADI port is high
	 *
	 *   pros::Controller master(pros::E_CONTROLLER_MASTER);
	 *
	 *   while (true) {
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
	 *       left_piston.extend();
	 *     }
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
	 *       left_piston.retract();
	 *     }
	 *     if(mastetr.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
	 *       left_piston.toggle();
	 *     }
	 *
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t retract();

	/**
	 * Puts the piston into the opposite state of its current state.
	 * If it is retracted, it will extend. If it is extended, it will retract.
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 *
	 * \return 1 if the piston successfully toggled, or PROS_ERR if the
	 *         operation failed, setting errno.
	 *
	 *\b Example:
	 * \code
	 * void opcontrol() {
	 * 	 pros::adi::Pneumatics piston({1, 'a'}, false);            // Starts retracted, extends when the ADI port is high
	 *
	 *   pros::Controller master(pros::E_CONTROLLER_MASTER);
	 *
	 *   while (true) {
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
	 *       left_piston.extend();
	 *     }
	 *     if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
	 *       left_piston.retract();
	 *     }
	 *     if(mastetr.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
	 *       left_piston.toggle();
	 *     }
	 *
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t toggle();

	/**
	 * Returns whether the piston is extended or not.
	 *
	 * \return true if the piston is extended, false if it is retracted.
	 *
	 * \b Example
	 * \code
	 * #define ADI_PNEUMATICS_PORT 'a'
	 *
	 * void opcontrol() {
	 *   pros::adi::Pneumatics pneumatics (ADI_PNEUMATICS_PORT);
	 *   while (true) {
	 *     // Check if the piston is extended
	 *     if (pneumatics.is_extended()) {
	 *       printf("The pneumatic is extended\n");
	 *     }
	 *     else {
	 *       printf("The pneumatic is not extended\n");
	 *     }
	 *
	 *     pros::delay(10);
	 *   }
	 * }
	 * \endcode
	 */
	bool is_extended() const;

	private:
	bool state;            // Holds the physical state of the ADI port
	bool extended_is_low;  // A flag that sets whether extended corresponds to
	                       // a low signal
};
///@}

}  // namespace adi

/*
Pros4 upgrade backwards compatibility for ADI api.

Prints a deprecated warning when user uses old pros::ADIDevice style API.
Remove when and if fully removing old API.
*/
LEGACY_TYPEDEF(ADIPort, pros::adi::Port);
LEGACY_TYPEDEF(ADIAnalogIn, pros::adi::AnalogIn);
LEGACY_TYPEDEF(ADIAnalogOut, pros::adi::AnalogOut);
LEGACY_TYPEDEF(ADIDigitalIn, pros::adi::DigitalIn);
LEGACY_TYPEDEF(ADIDigitalOut, pros::adi::DigitalOut);
LEGACY_TYPEDEF(ADIMotor, pros::adi::Motor);
LEGACY_TYPEDEF(ADIGyro, pros::adi::Gyro);
LEGACY_TYPEDEF(ADIEncoder, pros::adi::Encoder);
LEGACY_TYPEDEF(ADIUltrasonic, pros::adi::Ultrasonic);
LEGACY_TYPEDEF(LED, pros::adi::Led);

// Backwards Compatibility for Derived Classes
LEGACY_TYPEDEF(ADIPotentiometer, pros::adi::Potentiometer);
LEGACY_TYPEDEF(ADILineSensor, pros::adi::LineSensor);
LEGACY_TYPEDEF(ADILightSensor, pros::adi::LightSensor);
LEGACY_TYPEDEF(ADIAccelerometer, pros::adi::Accelerometer);
LEGACY_TYPEDEF(ADIButton, pros::adi::Button);
LEGACY_TYPEDEF(ADIPneumatics, pros::adi::Pneumatics);
LEGACY_TYPEDEF(ADILED, pros::adi::Led);
LEGACY_TYPEDEF(ADILed, pros::adi::Led);

}  // namespace pros

#endif  // _PROS_ADI_HPP_