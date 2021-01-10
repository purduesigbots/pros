/**
 * \file pros/adi.h
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

#ifndef _PROS_ADI_H_
#define _PROS_ADI_H_

#include <stdbool.h>
#include <stdint.h>
#ifndef PROS_ERR
#define PROS_ERR (INT32_MAX)
#endif

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

/**
 * Represents the port type for an ADI port.
 */
typedef enum adi_port_config_e {
	E_ADI_ANALOG_IN = 0,
	E_ADI_ANALOG_OUT = 1,
	E_ADI_DIGITAL_IN = 2,
	E_ADI_DIGITAL_OUT = 3,

#ifdef _INTELLISENSE
#define _DEPRECATE_DIGITAL_IN = E_ADI_DIGITAL_IN
#define _DEPRECATE_ANALOG_IN = E_ADI_ANALOG_IN
#else
#define _DEPRECATE_DIGITAL_IN __attribute__((deprecated("use E_ADI_DIGITAL_IN instead"))) = E_ADI_DIGITAL_IN
#define _DEPRECATE_ANALOG_IN __attribute__((deprecated("use E_ADI_ANALOG_IN instead"))) = E_ADI_ANALOG_IN
#endif

	E_ADI_SMART_BUTTON _DEPRECATE_DIGITAL_IN,
	E_ADI_SMART_POT _DEPRECATE_ANALOG_IN,

	E_ADI_LEGACY_BUTTON _DEPRECATE_DIGITAL_IN,
	E_ADI_LEGACY_POT _DEPRECATE_ANALOG_IN,
	E_ADI_LEGACY_LINE_SENSOR _DEPRECATE_ANALOG_IN,
	E_ADI_LEGACY_LIGHT_SENSOR _DEPRECATE_ANALOG_IN,
	E_ADI_LEGACY_GYRO = 10,
	E_ADI_LEGACY_ACCELEROMETER _DEPRECATE_ANALOG_IN,

#undef _DEPRECATE_DIGITAL_IN
#undef _DEPRECATE_ANALOG_IN

	E_ADI_LEGACY_SERVO = 12,
	E_ADI_LEGACY_PWM = 13,

	E_ADI_LEGACY_ENCODER = 14,
	E_ADI_LEGACY_ULTRASONIC = 15,

	E_ADI_TYPE_UNDEFINED = 255,
	E_ADI_ERR = PROS_ERR
} adi_port_config_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define ADI_ANALOG_IN pros::E_ADI_ANALOG_IN
#define ADI_ANALOG_OUT pros::E_ADI_ANALOG_OUT
#define ADI_DIGITAL_IN pros::E_ADI_DIGITAL_IN
#define ADI_DIGITAL_OUT pros::E_ADI_DIGITAL_OUT
#define ADI_SMART_BUTTON pros::E_ADI_SMART_BUTTON
#define ADI_SMART_POT pros::E_ADI_SMART_POT
#define ADI_LEGACY_BUTTON pros::E_ADI_LEGACY_BUTTON
#define ADI_LEGACY_POT pros::E_ADI_LEGACY_POT
#define ADI_LEGACY_LINE_SENSOR pros::E_ADI_LEGACY_LINE_SENSOR
#define ADI_LEGACY_LIGHT_SENSOR pros::E_ADI_LEGACY_LIGHT_SENSOR
#define ADI_LEGACY_GYRO pros::E_ADI_LEGACY_GYRO
#define ADI_LEGACY_ACCELEROMETER pros::E_ADI_LEGACY_ACCELEROMETER
#define ADI_LEGACY_SERVO pros::E_ADI_LEGACY_SERVO
#define ADI_LEGACY_PWM pros::E_ADI_LEGACY_PWM
#define ADI_LEGACY_ENCODER pros::E_ADI_LEGACY_ENCODER
#define ADI_LEGACY_ULTRASONIC pros::E_ADI_LEGACY_ULTRASONIC
#define ADI_TYPE_UNDEFINED pros::E_ADI_TYPE_UNDEFINED
#define ADI_ERR pros::E_ADI_ERR
#else
#define ADI_ANALOG_IN E_ADI_ANALOG_IN
#define ADI_ANALOG_OUT E_ADI_ANALOG_OUT
#define ADI_DIGITAL_IN E_ADI_DIGITAL_IN
#define ADI_DIGITAL_OUT E_ADI_DIGITAL_OUT
#define ADI_SMART_BUTTON E_ADI_SMART_BUTTON
#define ADI_SMART_POT E_ADI_SMART_POT
#define ADI_LEGACY_BUTTON E_ADI_LEGACY_BUTTON
#define ADI_LEGACY_POT E_ADI_LEGACY_POT
#define ADI_LEGACY_LINE_SENSOR E_ADI_LEGACY_LINE_SENSOR
#define ADI_LEGACY_LIGHT_SENSOR E_ADI_LEGACY_LIGHT_SENSOR
#define ADI_LEGACY_GYRO E_ADI_LEGACY_GYRO
#define ADI_LEGACY_ACCELEROMETER E_ADI_LEGACY_ACCELEROMETER
#define ADI_LEGACY_SERVO E_ADI_LEGACY_SERVO
#define ADI_LEGACY_PWM E_ADI_LEGACY_PWM
#define ADI_LEGACY_ENCODER E_ADI_LEGACY_ENCODER
#define ADI_LEGACY_ULTRASONIC E_ADI_LEGACY_ULTRASONIC
#define ADI_TYPE_UNDEFINED E_ADI_TYPE_UNDEFINED
#define ADI_ERR E_ADI_ERR
#endif
#endif

#define INTERNAL_ADI_PORT 22
#define NUM_ADI_PORTS 8

#ifdef __cplusplus
namespace c {
#endif

/******************************************************************************/
/**                         General ADI Use Functions                        **/
/**                                                                          **/
/**       These functions allow for interaction with any ADI port type       **/
/******************************************************************************/

/**
 * Gets the configuration for the given ADI port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports.
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') for which to return
 *        the configuration
 *
 * \return The ADI configuration for the given port
 */
adi_port_config_e_t adi_port_get_config(uint8_t port);

/**
 * Gets the value for the given ADI port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports.
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') for which the value
 *        will be returned
 *
 * \return The value stored for the given port
 */
int32_t adi_port_get_value(uint8_t port);

/**
 * Configures an ADI port to act as a given sensor type.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports.
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
 * \param type
 *        The configuration type for the port
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t adi_port_set_config(uint8_t port, adi_port_config_e_t type);

/**
 * Sets the value for the given ADI port.
 *
 * This only works on ports configured as outputs, and the behavior will change
 * depending on the configuration of the port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO  - The given value is not within the range of ADI Ports.
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') for which the value
 *        will be set
 * \param value
 *        The value to set the ADI port to
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t adi_port_set_value(uint8_t port, int32_t value);

/******************************************************************************/
/**                      PROS 2 Compatibility Functions                      **/
/**                                                                          **/
/**     These functions provide similar functionality to the PROS 2 API      **/
/******************************************************************************/

/**
 * Used for adi_digital_write() to specify a logic HIGH state to output.
 *
 * In reality, using any non-zero expression or "true" will work to set a pin to
 * HIGH.
 */
#define HIGH 1
/**
 * Used for adi_digital_write() to specify a logic LOW state to output.
 *
 * In reality, using a zero expression or "false" will work to set a pin to LOW.
 */
#define LOW 0

/**
 * adi_pin_mode() state for a digital input.
 */
#define INPUT 0x00
/**
 * adi_pin_mode() state for a digital output.
 */
#define OUTPUT 0x01
/**
 * adi_pin_mode() state for an analog input.
 */
#define INPUT_ANALOG 0x02

/**
 * adi_pin_mode() state for an analog output.
 */
#define OUTPUT_ANALOG 0x03

/**
 * Calibrates the analog sensor on the specified port and returns the new
 * calibration value.
 *
 * This method assumes that the true sensor value is not actively changing at
 * this time and computes an average from approximately 500 samples, 1 ms apart,
 * for a 0.5 s period of calibration. The average value thus calculated is
 * returned and stored for later calls to the adi_analog_read_calibrated() and
 * adi_analog_read_calibrated_HR() functions. These functions will return
 * the difference between this value and the current sensor value when called.
 *
 * Do not use this function when the sensor value might be unstable
 * (gyro rotation, accelerometer movement).
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 *
 * \param port
 *        The ADI port to calibrate (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return The average sensor value computed by this function
 */
int32_t adi_analog_calibrate(uint8_t port);

/**
 * Gets the 12-bit value of the specified port.
 *
 * The value returned is undefined if the analog pin has been switched to a
 * different mode.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an analog input
 *
 * \param port
 *        The ADI port (from 1-8, 'a'-'h', 'A'-'H') for which the value will be
 *        returned
 *
 * \return The analog sensor value, where a value of 0 reflects an input voltage
 * of nearly 0 V and a value of 4095 reflects an input voltage of nearly 5 V
 */
int32_t adi_analog_read(uint8_t port);

/**
 * Gets the 12 bit calibrated value of an analog input port.
 *
 * The adi_analog_calibrate() function must be run first. This function is
 * inappropriate for sensor values intended for integration, as round-off error
 * can accumulate causing drift over time. Use adi_analog_read_calibrated_HR()
 * instead.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an analog input
 *
 * \param port
 *        The ADI port (from 1-8, 'a'-'h', 'A'-'H') for which the value will be
 *        returned
 *
 * \return The difference of the sensor value from its calibrated default from
 * -4095 to 4095
 */
int32_t adi_analog_read_calibrated(uint8_t port);

/**
 * Gets the 16 bit calibrated value of an analog input port.
 *
 * The adi_analog_calibrate() function must be run first. This is intended for
 * integrated sensor values such as gyros and accelerometers to reduce drift due
 * to round-off, and should not be used on a sensor such as a line tracker
 * or potentiometer.
 *
 * The value returned actually has 16 bits of "precision", even though the ADC
 * only reads 12 bits, so that error induced by the average value being between
 * two values when integrated over time is trivial. Think of the value as the
 * true value times 16.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an analog input
 *
 * \param port
 *        The ADI port (from 1-8, 'a'-'h', 'A'-'H') for which the value will be
 *        returned
 *
 * \return The difference of the sensor value from its calibrated default from
 * -16384 to 16384
 */
int32_t adi_analog_read_calibrated_HR(uint8_t port);

/**
 * Gets the digital value (1 or 0) of a port configured as a digital input.
 *
 * If the port is configured as some other mode, the digital value which
 * reflects the current state of the port is returned, which may or may not
 * differ from the currently set value. The return value is undefined for ports
 * configured as any mode other than a Digital Input.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as a digital input
 *
 * \param port
 *        The ADI port to read (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return True if the pin is HIGH, or false if it is LOW
 */
int32_t adi_digital_read(uint8_t port);

/**
 * Gets a rising-edge case for a digital button press.
 *
 * This function is not thread-safe.
 * Multiple tasks polling a single button may return different results under the
 * same circumstances, so only one task should call this function for any given
 * button. E.g., Task A calls this function for buttons 1 and 2. Task B may call
 * this function for button 3, but should not for buttons 1 or 2. A typical
 * use-case for this function is to call inside opcontrol to detect new button
 * presses, and not in any other tasks.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as a digital input
 *
 * \param port
 *        The ADI port to read (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return 1 if the button is pressed and had not been pressed
 * the last time this function was called, 0 otherwise.
 */
int32_t adi_digital_get_new_press(uint8_t port);

/**
 * Sets the digital value (1 or 0) of a port configured as a digital output.
 *
 * If the port is configured as some other mode, behavior is undefined.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as a digital output
 *
 * \param port
 *        The ADI port to read (from 1-8, 'a'-'h', 'A'-'H')
 * \param value
 *        An expression evaluating to "true" or "false" to set the output to
 *        HIGH or LOW respectively, or the constants HIGH or LOW themselves
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t adi_digital_write(uint8_t port, bool value);

/**
 * Configures the port as an input or output with a variety of settings.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 *
 * \param port
 *        The ADI port to read (from 1-8, 'a'-'h', 'A'-'H')
 * \param mode
 *        One of INPUT, INPUT_ANALOG, INPUT_FLOATING, OUTPUT, or OUTPUT_OD
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t adi_pin_mode(uint8_t port, uint8_t mode);

/**
 * Sets the speed of the motor on the given port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an motor
 *
 * \param port
 *        The ADI port to set (from 1-8, 'a'-'h', 'A'-'H')
 * \param speed
 *        The new signed speed; -127 is full reverse and 127 is full forward,
 *        with 0 being off
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t adi_motor_set(uint8_t port, int8_t speed);

/**
 * Gets the last set speed of the motor on the given port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an motor
 *
 * \param port
 *        The ADI port to get (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return The last set speed of the motor on the given port
 */
int32_t adi_motor_get(uint8_t port);

/**
 * Stops the motor on the given port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an motor
 *
 * \param port
 *        The ADI port to set (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t adi_motor_stop(uint8_t port);

/**
 * Reference type for an initialized encoder.
 *
 * This merely contains the port number for the encoder, unlike its use as an
 * object to store encoder data in PROS 2.
 */
typedef int32_t adi_encoder_t;

/**
 * Gets the number of ticks recorded by the encoder.
 *
 * There are 360 ticks in one revolution.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an encoder

 *
 * \param enc
 *        The adi_encoder_t object from adi_encoder_init() to read
 *
 * \return The signed and cumulative number of counts since the last start or
 * reset
 */
int32_t adi_encoder_get(adi_encoder_t enc);

/**
 * Creates an encoder object and configures the specified ports accordingly.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an encoder

 *
 * \param port_top
 *        The "top" wire from the encoder sensor with the removable cover side
 *        up. This should be in port 1, 3, 5, or 7 ('A', 'C', 'E', or 'G').
 * \param port_bottom
 *        The "bottom" wire from the encoder sensor
 * \param reverse
 *        If "true", the sensor will count in the opposite direction
 *
 * \return An adi_encoder_t object to be stored and used for later calls to
 * encoder functions
 */
adi_encoder_t adi_encoder_init(uint8_t port_top, uint8_t port_bottom, bool reverse);

/**
 * Sets the encoder value to zero.
 *
 * It is safe to use this method while an encoder is enabled. It is not
 * necessary to call this method before stopping or starting an encoder.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an encoder

 *
 * \param enc
 *        The adi_encoder_t object from adi_encoder_init() to reset
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t adi_encoder_reset(adi_encoder_t enc);

/**
 * Disables the encoder and voids the configuration on its ports.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an encoder
 *
 * \param enc
 *        The adi_encoder_t object from adi_encoder_init() to stop
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t adi_encoder_shutdown(adi_encoder_t enc);

/**
 * Reference type for an initialized ultrasonic.
 *
 * This merely contains the port number for the ultrasonic, unlike its use as an
 * object to store ultrasonic data in PROS 2.
 */
typedef int32_t adi_ultrasonic_t;

/**
 * Gets the current ultrasonic sensor value in centimeters.
 *
 * If no object was found, zero is returned. If the ultrasonic sensor was never
 * started, the return value is undefined. Round and fluffy objects can cause
 * inaccurate values to be returned.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an ultrasonic
 *
 * \param ult
 *        The adi_ultrasonic_t object from adi_ultrasonic_init() to read
 *
 * \return The distance to the nearest object in m^-4 (10000 indicates 1 meter),
 * measured from the sensor's mounting points.
 */
int32_t adi_ultrasonic_get(adi_ultrasonic_t ult);

/**
 * Creates an ultrasonic object and configures the specified ports accordingly.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an ultrasonic
 *
 * \param port_ping
 *        The port connected to the orange OUTPUT cable. This should be in port
 *        1, 3, 5, or 7 ('A', 'C', 'E', 'G').
 * \param port_echo
 *        The port connected to the yellow INPUT cable. This should be in the
 *        next highest port following port_ping.
 *
 * \return An adi_ultrasonic_t object to be stored and used for later calls to
 * ultrasonic functions
 */
adi_ultrasonic_t adi_ultrasonic_init(uint8_t port_ping, uint8_t port_echo);

/**
 * Disables the ultrasonic sensor and voids the configuration on its ports.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as an ultrasonic
 *
 * \param ult
 *        The adi_ultrasonic_t object from adi_ultrasonic_init() to stop
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t adi_ultrasonic_shutdown(adi_ultrasonic_t ult);

/**
 * Reference type for an initialized gyroscope.
 *
 * This merely contains the port number for the gyroscope, unlike its use as an
 * object to store gyro data in PROS 2.
 */
typedef int32_t adi_gyro_t;

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
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as a gyro
 *
 * \param gyro
 *        The adi_gyro_t object for which the angle will be returned
 *
 * \return The gyro angle in degrees.
 */
double adi_gyro_get(adi_gyro_t gyro);

/**
 * Initializes a gyroscope on the given port. If the given port has not
 * previously been configured as a gyro, then this function starts a 1300 ms
 * calibration period.
 *
 * It is highly recommended that this function be called from initialize() when
 * the robot is stationary to ensure proper calibration.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as a gyro
 *
 * \param port
 *        The ADI port to initialize as a gyro (from 1-8, 'a'-'h', 'A'-'H')
 * \param multiplier
 *        A scalar value that will be multiplied by the gyro heading value
 *        supplied by the ADI
 *
 * \return An adi_gyro_t object containing the given port, or PROS_ERR if the
 * initialization failed.
 */
adi_gyro_t adi_gyro_init(uint8_t port, double multiplier);

/**
 * Resets the gyroscope value to zero.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as a gyro
 *
 * \param gyro
 *        The adi_gyro_t object for which the angle will be returned
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t adi_gyro_reset(adi_gyro_t gyro);

/**
 * Disables the gyro and voids the configuration on its port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of ADI Ports
 * EADDRINUSE - The port is not configured as a gyro
 *
 * \param gyro
 *        The adi_gyro_t object to be shut down
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 */
int32_t adi_gyro_shutdown(adi_gyro_t gyro);

#ifdef __cplusplus
}  // namespace c
}  // namespace pros
}
#endif

#endif  // _PROS_ADI_H_
