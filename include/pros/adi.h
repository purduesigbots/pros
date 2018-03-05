/**
 * \file pros/adi.h
 *
 * \brief Prototypes for interfacing with the ADI.
 *
 * This file contains the header info for the functions used to modify the
 * status of the ADI ports.
 *
 * \copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _PROS_ADI_H_
#define _PROS_ADI_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represents the port type for an ADI port.
 */
typedef enum adi_port_config_e {
	E_ADI_ANALOG_IN = 0,
	E_ADI_ANALOG_OUT,
	E_ADI_DIGITAL_IN,
	E_ADI_DIGITAL_OUT,

	E_ADI_SMART_BUTTON,
	E_ADI_SMART_POT,

	E_ADI_LEGACY_BUTTON,
	E_ADI_LEGACY_POT,
	E_ADI_LEGACY_LINE_SENSOR,
	E_ADI_LEGACY_LIGHT_SENSOR,
	E_ADI_LEGACY_GYRO,
	E_ADI_LEGACY_ACCELEROMETER,

	E_ADI_LEGACY_SERVO,
	E_ADI_LEGACY_PWM,

	E_ADI_LEGACY_ENCODER,
	E_ADI_LEGACY_ULTRASONIC,

	E_ADI_TYPE_UNDEFINED = 255,
	E_ADI_ERR = PROS_ERR
} adi_port_config_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
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

#define NUM_ADI_PORTS 8

/*
 * For the following functions:
 * If they don't need a return value (e.g. a setter):
 * 		They will return PROS_ERR upon failure and 1 upons success
 * If they should return a bool (e.g. motor_get_over_temp_flag):
 *		They will return 1 for true, 0 for false, and PROS_ERR upon failure
 * If they should return a 32-bit integer (e.g. a getter):
 *		They will return their value. If an error occurs, they will return
 *		PROS_ERR. If their actual return value should be PROS_ERR (INT32_MAX)
 *		then they will set errno = 0 to indicate no error has occured.
 * If they should return a double (e.g. a getter):
 *		They will return their value, or PROS_ERROR_F upon failure
 *
 * Upon returning PROS_ERR or PROS_ERR_F, errno will be set to indicate the
 * type of error. Again, some functions may return PROS_ERR as a valid value;
 * in this case, errno will be set to 0
 */

/**
 * Configures an ADI port to act as a given sensor type.
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') to configure
 * \param type
 *        The configuration type for the port
 *
 * \return 1 if the operation was successful, PROS_ERR otherwise
 */
int32_t adi_port_config_set(uint8_t port, adi_port_config_e_t type);

/**
 * Returns the configuration for the given ADI port
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') for which to return
 *        the configuration
 *
 * \return The ADI configuration for the given port
 */
adi_port_config_e_t adi_port_config_get(uint8_t port);

/**
 * Sets the value for the given ADI port
 * This only works on ports configured as outputs, and the behavior will change
 * depending on the configuration of the port
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') for which the value
 *        will be set
 * \param value
 *        The value to set the ADI port to
 *
 * \return 1 if the operation was successful, PROS_ERR otherwise
 */
int32_t adi_value_set(uint8_t port, int32_t value);

/**
 * Returns the value for the given ADI port
 *
 * \param port
 *        The ADI port number (from 1-8, 'a'-'h', 'A'-'H') for which the value
 *        will be returned
 *
 * \return The value stored for the given port
 */
int32_t adi_value_get(uint8_t port);

/**
 * Used for adi_digital_write() to specify a logic HIGH state to output.
 *
 * In reality, using any non-zero expression or "true" will work to set a pin to HIGH.
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
 * Calibrates the analog sensor on the specified channel.
 *
 * This method assumes that the true sensor value is not actively changing at this time and
 * computes an average from approximately 500 samples, 1 ms apart, for a 0.5 s period of
 * calibration. The average value thus calculated is returned and stored for later calls to the
 * analog_read_calibrated() and analog_read_calibrated_HR() functions. These functions will return
 * the difference between this value and the current sensor value when called.
 *
 * Do not use this function when the sensor value might be unstable
 * (gyro rotation, accelerometer movement).
 *
 * \param port
 *        The ADI port to calibrate (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return The average sensor value computed by this function
 */
int32_t adi_analog_calibrate(uint8_t port);
/**
 * Reads an analog input channel and returns the 12-bit value.
 *
 * The value returned is undefined if the analog pin has been switched to a different mode.
 * This function is Wiring-compatible with the exception of the larger output range. The
 * meaning of the returned value varies depending on the sensor attached.
 *
 * \param port
 *        The ADI port to calibrate (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return The analog sensor value, where a value of 0 reflects an input voltage of nearly 0 V
 * and a value of 4095 reflects an input voltage of nearly 5 V
 */
int32_t adi_analog_read(uint8_t port);
/**
 * Reads the calibrated value of an analog input channel.
 *
 * The adi_analog_calibrate() function must be run first on that channel. This function is
 * inappropriate for sensor values intended for integration, as round-off error can accumulate
 * causing drift over time. Use analogReadCalibratedHR() instead.
 *
 * \param port
 *        The ADI port to read (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return The difference of the sensor value from its calibrated default from -4095 to 4095
 */
int32_t adi_analog_read_calibrated(uint8_t port);
/**
 * Reads the calibrated value of an analog input channel 1-8 with enhanced precision.
 *
 * The adi_analog_calibrate() function must be run first. This is intended for integrated sensor
 * values such as gyros and accelerometers to reduce drift due to round-off, and should not be
 * used on a sensor such as a line tracker or potentiometer.
 *
 * The value returned actually has 16 bits of "precision", even though the ADC only reads
 * 12 bits, so that errors induced by the average value being between two values come out
 * in the wash when integrated over time. Think of the value as the true value times 16.
 *
 * \param port
 *        The ADI port to read (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return The difference of the sensor value from its calibrated default from -16384 to 16384
 */
int32_t adi_analog_read_calibrated_HR(uint8_t port);
/**
 * Gets the digital value (1 or 0) of a pin configured as a digital input.
 *
 * If the pin is configured as some other mode, the digital value which reflects the current
 * state of the pin is returned, which may or may not differ from the currently set value. The
 * return value is undefined for pins configured as Analog inputs, or for ports in use by a
 * Communications interface. This function is Wiring-compatible.
 *
 * \param port
 *        The ADI port to read (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return True if the pin is HIGH, or false if it is LOW
 */
int32_t adi_digital_read(uint8_t port);
/**
 * Returns a rising-edge case for a digital button press.
 *
 * This function is not thread-safe.
 * Multiple tasks polling a single button may return different results under the
 * same circumstances, so only one task should call this function for any given
 * button. E.g., Task A calls this function for buttons 1 and 2. Task B may call
 * this function for button 3, but should not for buttons 1 or 2. A typical
 * use-case for this function is to call inside opcontrol to detect new button
 * presses, and not in any other tasks.
 *
 * \param port
 *        The ADI port to read (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return 1 if the button on the controller is pressed and had not been pressed
 *         the last time this function was called, 0 otherwise.
 */
int32_t adi_digital_get_new_press(uint8_t port);

/**
 * Sets the digital value (1 or 0) of a pin configured as a digital output.
 *
 * If the pin is configured as some other mode, behavior is undefined. This function is
 * Wiring-compatible.
 *
 * \param port
 *        The ADI port to read (from 1-8, 'a'-'h', 'A'-'H')
 * \param value
 *        An expression evaluating to "true" or "false" to set the output to HIGH or LOW
 *        respectively, or the constants HIGH or LOW themselves
 *
 * \return 1 if the operation was successful, PROS_ERR otherwise
 */
int32_t adi_digital_write(uint8_t port, const bool value);
/**
 * Configures the pin as an input or output with a variety of settings.
 *
 * \param port
 *        The ADI port to read (from 1-8, 'a'-'h', 'A'-'H')
 * \param mode
 *        One of INPUT, INPUT_ANALOG, INPUT_FLOATING, OUTPUT, or OUTPUT_OD
 *
 * \return 1 if the operation was successful, PROS_ERR otherwise
 */
int32_t adi_pin_mode(uint8_t port, uint8_t mode);

/**
 * Sets the speed of the motor on the given port.
 *
 * \param port
 *        The ADI port to set (from 1-8, 'a'-'h', 'A'-'H')
 * \param speed
 *        The new signed speed; -127 is full reverse and 127 is full forward, with 0
 *        being off
 *
 * \return 1 if the operation was successful, PROS_ERR otherwise
 */
int32_t adi_motor_set(uint8_t port, const int8_t speed);
/**
 * Returns the last set speed of the motor on the given port.
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
 * \param port
 *        The ADI port to set (from 1-8, 'a'-'h', 'A'-'H')
 *
 * \return 1 if the operation was successful, PROS_ERR otherwise
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
 * \param enc
 *        The adi_encoder_t object from adi_encoder_init() to read
 *
 * \return The signed and cumulative number of counts since the last start or reset
 */
int32_t adi_encoder_get(adi_encoder_t enc);
/**
 * Initializes and enables a quadrature encoder on two digital ports.
 *
 * \param port_top
 *        The "top" wire from the encoder sensor with the removable cover side UP
 * \param port_bottom
 *        The "bottom" wire from the encoder sensor
 * \param reverse
 *        If "true", the sensor will count in the opposite direction
 *
 * \return An adi_encoder_t object to be stored and used for later calls to encoder functions
 */
adi_encoder_t adi_encoder_init(uint8_t port_top, uint8_t port_bottom, const bool reverse);
/**
 * Resets the encoder to zero.
 *
 * It is safe to use this method while an encoder is enabled. It is not necessary to call this
 * method before stopping or starting an encoder.
 *
 * \param enc
 *        The adi_encoder_t object from adi_encoder_init() to reset
 *
 * \return 1 if the operation was successful, PROS_ERR otherwise
 */
int32_t adi_encoder_reset(adi_encoder_t enc);
/**
 * Stops and disables the encoder.
 *
 * \param enc
 *        The adi_encoder_t object from adi_encoder_init() to stop
 *
 * \return 1 if the operation was successful, PROS_ERR otherwise
 */
int32_t adi_encoder_shutdown(adi_encoder_t enc);

/**
 * Reference type for an initialized ultrasonic.
 *
 * This merely contains the port number for the ultrasonic, unlike its use as an
 * object to store encoder data in PROS 2.
 */
typedef int32_t adi_ultrasonic_t;

/**
 * Gets the current ultrasonic sensor value in centimeters.
 *
 * If no object was found, zero is returned. If the ultrasonic sensor was never started, the
 * return value is undefined. Round and fluffy objects can cause inaccurate values to be
 * returned.
 *
 * \param ult
 *        The adi_ultrasonic_t object from adi_ultrasonic_init() to read
 *
 * \return The distance to the nearest object in centimeters
 */
int32_t adi_ultrasonic_get(adi_ultrasonic_t ult);
/**
 * Initializes an ultrasonic sensor on the specified ADI ports.
 *
 * \param port_echo
 *        The port connected to the yellow INPUT cable. This should be in port
 *        1, 3, 5, or 7 ('A', 'C', 'E', 'G').
 * \param port_ping
 *        The port connected to the orange OUTPUT cable. This should be in the
 *        next highest port following port_echo.
 *
 * \return An adi_ultrasonic_t object to be stored and used for later calls to ultrasonic functions
 */
adi_ultrasonic_t adi_ultrasonic_init(uint8_t port_echo, uint8_t port_ping);
/**
 * Stops and disables the ultrasonic sensor.
 *
 * \param ult
 *        The adi_ultrasonic_t object from adi_ultrasonic_init() to stop
 *
 * \return 1 if the operation was successful, PROS_ERR otherwise
 */
int32_t adi_ultrasonic_shutdown(adi_ultrasonic_t ult);

#ifdef __cplusplus
}
#endif

#endif
