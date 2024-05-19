/**
 * \file pros/motors.h
 * \ingroup c-motors
 *
 * Contains prototypes for the V5 Motor-related functions.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * \defgroup c-motors Motors C API
 * \note Additional example code for this module can be found in its [Tutorial](@ref motors).
 */

#ifndef _PROS_MOTORS_H_
#define _PROS_MOTORS_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

/**
 * \ingroup c-motors
 */

/**
 * \addtogroup c-motors
 *  @{
 */

/// \name Motor movement functions
/// These functions allow programmers to make motors move
///@{

/**
 * Sets the voltage for the motor from -127 to 127.
 *
 * This is designed to map easily to the input from the controller's analog
 * stick for simple opcontrol use. The actual behavior of the motor is analogous
 * to use of motor_move_voltage().
 *
 * \note This function will not respect brake modes, and simply sets the voltage to the desired value.
 * 
 * \note A negative port will negate the input voltage
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param voltage
 *        The new motor voltage from -127 to 127
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_move(int8_t port, int32_t voltage);

/**
 * Stops the motor using the currently configured brake mode.
 * 
 * This function sets motor velocity to zero, which will cause it to act
 * according to the set brake mode. If brake mode is set to MOTOR_BRAKE_HOLD,
 * this function may behave differently than calling motor_move_absolute(port, 0)
 * or motor_move_relative(port, 0).
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 * 
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * 
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code 
 * 	void autonomous() {
 * 		motor_move(1, 127);
 * 		delay(1000);
 * 		motor_break(1);
 * }
 * \endcode
 */
int32_t motor_brake(int8_t port);

/**
 * Sets the target absolute position for the motor to move to.
 *
 * This movement is relative to the position of the motor when initialized or
 * the position when it was most recently reset with motor_set_zero_position().
 *
 * \note This function simply sets the target for the motor, it does not block program
 * execution until the movement finishes. The example code shows how to block until a movement is finished.
 *
 * \note A negative port number will negate the target position 
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param position
 *        The absolute position to move to in the motor's encoder units
 * \param velocity
 *        The maximum allowable velocity for the movement in RPM
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void autonomous() {
 *   motor_move_absolute(1, 100, 100); // Moves 100 units forward
 *   while (!((motor_get_position(1) < 105) && (motor_get_position(1) > 95))) {
 *     // Continue running this loop as long as the motor is not within +-5 units of its goal
 *     delay(2);
 *   }
 *   motor_move_absolute(1, 100, 100); // This will not cause a movement
 *   while(!((motor_get_position(1) < 105) && (motor_get_position(1) > 95))) {
 *     delay(2);
 *   }
 * 
 *   motor_tare_position(1);
 *   motor_move_absolute(1, 100, 100); // Moves 100 units forward
 *   while (!((motor_get_position(1) < 105) && (motor_get_position(1) > 95))) {
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_move_absolute(int8_t port, double position, const int32_t velocity);

/**
 * Sets the relative target position for the motor to move to.
 *
 * This movement is relative to the current position of the motor as given in
 * motor_get_position(). Providing 10.0 as the position parameter would result
 * in the motor moving clockwise 10 units, no matter what the current position
 * is.
 *
 * \note This function simply sets the target for the motor, it does not block
 * program execution until the movement finishes. The example code shows how to
 * block until a movement is finished.
 *
 * \note A negative port will negate the target position
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param position
 *        The relative position to move to in the motor's encoder units
 * \param velocity
 *        The maximum allowable velocity for the movement in RPM
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void autonomous() {
 *   motor_move_relative(1, 100, 100); // Moves 100 units forward
 *   while (!((motor_get_position(1) < 105) && (motor_get_position(1) > 95))) {
 *     // Continue running this loop as long as the motor is not within +-5 units of its goal
 *     delay(2);
 *   }
 * 
 *   motor_move_relative(1, 100, 100); // Also moves 100 units forward
 *   while (!((motor_get_position(1) < 205) && (motor_get_position(1) > 195))) {
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_move_relative(int8_t port, double position, const int32_t velocity);

/**
 * Sets the velocity for the motor.
 *
 * This velocity corresponds to different actual speeds depending on the gearset
 * used for the motor. This results in a range of +-100 for E_MOTOR_GEARSET_36,
 * +-200 for E_MOTOR_GEARSET_18, and +-600 for E_MOTOR_GEARSET_6. The velocity
 * is held with PID to ensure consistent speed, as opposed to setting the
 * motor's voltage.
 *
 * \note A negative port will negate the velocity
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param velocity
 *        The new motor velocity from +-100, +-200, or +-600 depending on the
 *        motor's gearset
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void autonomous() {
 *   motor_move_velocity(1, 100);
 *   delay(1000); // Move at 100 RPM for 1 second
 *   motor_move_velocity(1, 0);
 * }
 * \endcode
 */
int32_t motor_move_velocity(int8_t port, const int32_t velocity);

/**
 * Sets the output voltage for the motor from -12000 to 12000 in millivolts.
 *
 * \note A negative port negates the voltage
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 * 
 * \note This function will not respect brake modes, and simply sets the
 * voltage to the desired value.
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param voltage
 *        The new voltage value from -12000 to 12000
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void autonomous() {
 *   motor_move_voltage(1, 12000);
 *   delay(1000); // Move at max voltage for 1 second
 *   motor_move_voltage(1, 0);
 * }
 * \endcode
 */
int32_t motor_move_voltage(int8_t port, const int32_t voltage);

/**
 * Changes the output velocity for a profiled movement (motor_move_absolute or
 * motor_move_relative). This will have no effect if the motor is not following
 * a profiled movement.
 *
 * \note A negative port negates the velocity
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param velocity
 *        The new motor velocity from +-100, +-200, or +-600 depending on the
 *        motor's gearset
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void autonomous() {
 *   motor_move_absolute(1, 100, 100);
 *   delay(100);
 *   motor_modify_profiled_velocity(1, 0); // Stop the motor early
 * }
 * \endcode
 */
int32_t motor_modify_profiled_velocity(int8_t port, const int32_t velocity);

/**
 * Gets the target position set for the motor by the user.
 *
 * \note A negative port negates the return value
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The target position in its encoder units or PROS_ERR_F if the
 * operation failed, setting errno.
 * 
 * \b Example
 * \code
 * void autonomous() {
 *   motor_move_absolute(1, 100, 100);
 *   printf("Motor Target: %d\n", motor_get_target_position(1));
 *   // Prints 100
 * }
 * \endcode
 */
double motor_get_target_position(int8_t port);

/**
 * Gets the velocity commanded to the motor by the user.
 *
 * \note A negative port negates the return value
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The commanded motor velocity from +-100, +-200, or +-600, or PROS_ERR
 * if the operation failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move_velocity(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Commanded Velocity: %d\n", motor_get_target_velocity(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_get_target_velocity(int8_t port);

///@}

/// \name Motor telemetry functions
/// These functions allow programmers to collect telemetry from motors
///@{

/**
 * Gets the actual velocity of the motor.
 *
 * \note A negative port negates the return value 
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The motor's actual velocity in RPM or PROS_ERR_F if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Actual velocity: %lf\n", motor_get_actual_velocity(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
double motor_get_actual_velocity(int8_t port);

/**
 * Gets the current drawn by the motor in mA.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The motor's current in mA or PROS_ERR if the operation failed,
 * setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Current Draw: %d\n", motor_get_current_draw(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_get_current_draw(int8_t port);

/**
 * Gets the direction of movement for the motor.
 *
 * \note A negative port number negates the return value. 
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return 1 for moving in the positive direction, -1 for moving in the
 * negative direction, or PROS_ERR if the operation failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Direction: %d\n", motor_get_direction(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_get_direction(int8_t port);

/**
 * Gets the efficiency of the motor in percent.
 *
 * An efficiency of 100% means that the motor is moving electrically while
 * drawing no electrical power, and an efficiency of 0% means that the motor
 * is drawing power but not moving.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The motor's efficiency in percent or PROS_ERR_F if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Efficiency: %d\n", motor_get_efficiency(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
double motor_get_efficiency(int8_t port);

/**
 * Checks if the motor is drawing over its current limit.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return 1 if the motor's current limit is being exceeded and 0 if the current
 * limit is not exceeded, or PROS_ERR if the operation failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Current Limit Hit?: %d\n", motor_is_over_current(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_is_over_current(int8_t port);

/**
 * Checks if the motor's temperature is above its limit.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return 1 if the temperature limit is exceeded and 0 if the the temperature
 * is below the limit, or PROS_ERR if the operation failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Temp Limit: %d\n", motor_is_over_temp(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_is_over_temp(int8_t port);



#ifdef __cplusplus
}  // namespace c
#endif

/**
 * \enum motor_fault_e_t
 */
typedef enum motor_fault_e {
	/// No faults
	E_MOTOR_FAULT_NO_FAULTS = 0x00,
	/// Analogous to motor_is_over_temp()
	E_MOTOR_FAULT_MOTOR_OVER_TEMP = 0x01,
	/// Indicates a motor h-bridge fault
	E_MOTOR_FAULT_DRIVER_FAULT = 0x02,
	/// Analogous to motor_is_over_current()
	E_MOTOR_FAULT_OVER_CURRENT = 0x04,
	/// Indicates an h-bridge over current
	E_MOTOR_FAULT_DRV_OVER_CURRENT = 0x08
} motor_fault_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define MOTOR_FAULT_NO_FAULTS pros::E_MOTOR_FAULT_NO_FAULTS
#define MOTOR_FAULT_MOTOR_OVER_TEMP pros::E_MOTOR_FAULT_MOTOR_OVER_TEMP
#define MOTOR_FAULT_DRIVER_FAULT pros::E_MOTOR_FAULT_DRIVER_FAULT
#define MOTOR_FAULT_OVER_CURRENT pros::E_MOTOR_FAULT_DRV_OVER_CURRENT
#define MOTOR_FAULT_DRV_OVER_CURRENT pros::E_MOTOR_FAULT_DRV_OVER_CURRENT
#else
#define MOTOR_FAULT_NO_FAULTS E_MOTOR_FAULT_NO_FAULTS
#define MOTOR_FAULT_MOTOR_OVER_TEMP E_MOTOR_FAULT_MOTOR_OVER_TEMP
#define MOTOR_FAULT_DRIVER_FAULT E_MOTOR_FAULT_DRIVER_FAULT
#define MOTOR_FAULT_OVER_CURRENT E_MOTOR_FAULT_DRV_OVER_CURRENT
#define MOTOR_FAULT_DRV_OVER_CURRENT E_MOTOR_FAULT_DRV_OVER_CURRENT
#endif
#endif

#ifdef __cplusplus
namespace c {
#endif

/**
 * Gets the faults experienced by the motor.
 *
 * Compare this bitfield to the bitmasks in motor_fault_e_t.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1-21
 *
 * \return A bitfield containing the motor's faults.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Faults: %d\n", motor_get_faults(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
uint32_t motor_get_faults(int8_t port);

#ifdef __cplusplus
}  // namespace c
#endif

/**
 * \enum motor_flag_e_t
 * 
 */
typedef enum motor_flag_e {
	///There are no flags raised
	E_MOTOR_FLAGS_NONE = 0x00,
	/// Cannot currently communicate to the motor
	E_MOTOR_FLAGS_BUSY = 0x01,
	/// Analogous to motor_is_stopped()
	E_MOTOR_FLAGS_ZERO_VELOCITY = 0x02,
	/// Analogous to motor_get_zero_position_flag()
	E_MOTOR_FLAGS_ZERO_POSITION = 0x04
} motor_flag_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define MOTOR_FLAGS_NONE pros::E_MOTOR_FLAGS_NONE
#define MOTOR_FLAGS_BUSY pros::E_MOTOR_FLAGS_BUSY
#define MOTOR_FLAGS_ZERO_VELOCITY pros::E_MOTOR_FLAGS_ZERO_VELOCITY
#define MOTOR_FLAGS_ZERO_POSITION pros::E_MOTOR_FLAGS_ZERO_POSITION
#else
#define MOTOR_FLAGS_NONE E_MOTOR_FLAGS_NONE
#define MOTOR_FLAGS_BUSY E_MOTOR_FLAGS_BUSY
#define MOTOR_FLAGS_ZERO_VELOCITY E_MOTOR_FLAGS_ZERO_VELOCITY
#define MOTOR_FLAGS_ZERO_POSITION E_MOTOR_FLAGS_ZERO_POSITION
#endif
#endif

#ifdef __cplusplus
namespace c {
#endif

/**
 * Gets the flags set by the motor's operation.
 *
 * Compare this bitfield to the bitmasks in motor_flag_e_t.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return A bitfield containing the motor's flags.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Flags: %d\n", motor_get_flags(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
uint32_t motor_get_flags(int8_t port);

/**
 * Gets the raw encoder count of the motor at a given timestamp.
 *
 * \note A negative port value negates the return value
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param[in] timestamp
 *            A pointer to a time in milliseconds for which the encoder count
 *            will be returned. If NULL, the timestamp at which the encoder
 *            count was read will not be supplied
 *
 * \return The raw encoder count at the given timestamp or PROS_ERR if the
 * operation failed.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   uint32_t now = millis();
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Encoder Count: %d\n", motor_get_raw_position(1, &now));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_get_raw_position(int8_t port, uint32_t* const timestamp);

/**
 * Gets the absolute position of the motor in its encoder units.
 *
 * \note A negative port value negates the return value
 * 
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The motor's absolute position in its encoder units or PROS_ERR_F
 * if the operation failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Position: %lf\n", motor_get_position(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
double motor_get_position(int8_t port);

/**
 * Gets the power drawn by the motor in Watts.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The motor's power draw in Watts or PROS_ERR_F if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   uint32_t now = millis();
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Power: %lf\n", motor_get_power(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
double motor_get_power(int8_t port);

/**
 * Gets the temperature of the motor in degrees Celsius.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The motor's temperature in degrees Celsius or PROS_ERR_F if the
 * operation failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Temperature: %lf\n", motor_get_temperature(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
double motor_get_temperature(int8_t port);

/**
 * Gets the torque generated by the motor in Newton Meters (Nm).
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The motor's torque in Nm or PROS_ERR_F if the operation failed,
 * setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Torque: %lf\n", motor_get_torque(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
double motor_get_torque(int8_t port);

/**
 * Gets the voltage delivered to the motor in millivolts.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The motor's voltage in mV or PROS_ERR_F if the operation failed,
 * setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     printf("Motor Voltage: %lf\n", motor_get_voltage(1));
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_get_voltage(int8_t port);

///@}

/// \name Motor configuration functions
/// These functions allow programmers to configure the behavior of motors
///@{

#ifdef __cplusplus
}  // namespace c
#endif

/**
 * \enum motor_brake_mode_e_t
 * Indicates the current 'brake mode' of a motor.
 */
typedef enum motor_brake_mode_e {
	/// Motor coasts when stopped, traditional behavior
	E_MOTOR_BRAKE_COAST = 0,
	/// Motor brakes when stopped
	E_MOTOR_BRAKE_BRAKE = 1,
	/// Motor actively holds position when stopped
	E_MOTOR_BRAKE_HOLD = 2,
	/// Invalid brake mode
	E_MOTOR_BRAKE_INVALID = INT32_MAX
} motor_brake_mode_e_t;

/**
 * \enum motor_encoder_units_e_t
 * Indicates the units used by the motor encoders.
 */
typedef enum motor_encoder_units_e {
	/// Position is recorded as angle in degrees as a floating point number
	E_MOTOR_ENCODER_DEGREES = 0,
	/// Position is recorded as angle in rotations as a floating point number
	E_MOTOR_ENCODER_ROTATIONS = 1,
	/// Position is recorded as raw encoder ticks as a whole number
	E_MOTOR_ENCODER_COUNTS = 2,     
	///Invalid motor encoder units
	E_MOTOR_ENCODER_INVALID = INT32_MAX
} motor_encoder_units_e_t;

/**
 * \enum motor_gearset_e_t
 * Indicates the current internal gear ratio of a motor.
 */
typedef enum motor_gearset_e {
	E_MOTOR_GEARSET_36 = 0,  // 36:1, 100 RPM, Red gear set
	E_MOTOR_GEAR_RED = E_MOTOR_GEARSET_36, // 36:1, 100 RPM, Red gear set
	E_MOTOR_GEAR_100 = E_MOTOR_GEARSET_36, // 36:1, 100 RPM, Red gear set
	E_MOTOR_GEARSET_18 = 1,  // 18:1, 200 RPM, Green gear set
	E_MOTOR_GEAR_GREEN = E_MOTOR_GEARSET_18, // 18:1, 200 RPM, Green gear set
	E_MOTOR_GEAR_200 = E_MOTOR_GEARSET_18, // 18:1, 200 RPM, Green gear set
	E_MOTOR_GEARSET_06 = 2,  // 6:1, 600 RPM, Blue gear set
	E_MOTOR_GEAR_BLUE  = E_MOTOR_GEARSET_06, // 6:1, 600 RPM, Blue gear set
	E_MOTOR_GEAR_600 = E_MOTOR_GEARSET_06, // 6:1, 600 RPM, Blue gear set
	E_MOTOR_GEARSET_INVALID = INT32_MAX, // Error: Invalid Gearset
} motor_gearset_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define MOTOR_BRAKE_COAST pros::E_MOTOR_BRAKE_COAST
#define MOTOR_BRAKE_BRAKE pros::E_MOTOR_BRAKE_BRAKE
#define MOTOR_BRAKE_HOLD pros::E_MOTOR_BRAKE_HOLD
#define MOTOR_BRAKE_INVALID pros::E_MOTOR_BRAKE_INVALID
#define MOTOR_ENCODER_DEGREES pros::E_MOTOR_ENCODER_DEGREES
#define MOTOR_ENCODER_ROTATIONS pros::E_MOTOR_ENCODER_ROTATIONS
#define MOTOR_ENCODER_COUNTS pros::E_MOTOR_ENCODER_COUNTS
#define MOTOR_ENCODER_INVALID pros::E_MOTOR_ENCODER_INVALID
#define MOTOR_GEARSET_36 pros::E_MOTOR_GEARSET_36
#define MOTOR_GEAR_RED pros::E_MOTOR_GEAR_RED
#define MOTOR_GEAR_100 pros::E_MOTOR_GEAR_100
#define MOTOR_GEARSET_18 pros::E_MOTOR_GEARSET_18
#define MOTOR_GEAR_GREEN pros::E_MOTOR_GEAR_GREEN
#define MOTOR_GEAR_200 pros::E_MOTOR_GEAR_200
#define MOTOR_GEARSET_06 pros::E_MOTOR_GEARSET_06
#define MOTOR_GEARSET_6 pros::E_MOTOR_GEARSET_06
#define MOTOR_GEAR_BLUE pros::E_MOTOR_GEAR_BLUE
#define MOTOR_GEAR_600 pros::E_MOTOR_GEAR_600
#define MOTOR_GEARSET_INVALID pros::E_MOTOR_GEARSET_INVALID
#else
#define MOTOR_BRAKE_COAST E_MOTOR_BRAKE_COAST
#define MOTOR_BRAKE_BRAKE E_MOTOR_BRAKE_BRAKE
#define MOTOR_BRAKE_HOLD E_MOTOR_BRAKE_HOLD
#define MOTOR_BRAKE_INVALID E_MOTOR_BRAKE_INVALID
#define MOTOR_ENCODER_DEGREES E_MOTOR_ENCODER_DEGREES
#define MOTOR_ENCODER_ROTATIONS E_MOTOR_ENCODER_ROTATIONS
#define MOTOR_ENCODER_COUNTS E_MOTOR_ENCODER_COUNTS
#define MOTOR_ENCODER_INVALID E_MOTOR_ENCODER_INVALID
#define MOTOR_GEARSET_36 E_MOTOR_GEARSET_36
#define MOTOR_GEAR_RED E_MOTOR_GEAR_RED
#define MOTOR_GEAR_100 E_MOTOR_GEAR_100
#define MOTOR_GEARSET_18 E_MOTOR_GEARSET_18
#define MOTOR_GEAR_GREEN E_MOTOR_GEAR_GREEN
#define MOTOR_GEAR_200 E_MOTOR_GEAR_200
#define MOTOR_GEARSET_06 E_MOTOR_GEARSET_06
#define MOTOR_GEARSET_6 E_MOTOR_GEARSET_06
#define MOTOR_GEAR_BLUE E_MOTOR_GEAR_BLUE
#define MOTOR_GEAR_600 E_MOTOR_GEAR_600
#define MOTOR_GEARSET_INVALID E_MOTOR_GEARSET_INVALID
#endif
#endif

/**
 * \struct motor_pid_full_s_t
 * 
 * Holds the information about a Motor's position or velocity PID controls.
 *
 * These values are in 4.4 format, meaning that a value of 0x20 represents 2.0,
 * 0x21 represents 2.0625, 0x22 represents 2.125, etc.
 */
typedef struct motor_pid_full_s {
	/// The feedforward constant
	uint8_t kf;
	/// The proportional constant
	uint8_t kp;
	/// The integral constants
	uint8_t ki;
	/// The derivative constant
	uint8_t kd;
	/// A constant used for filtering the profile acceleration         
	uint8_t filter;
	/// The integral limit
	uint16_t limit;
	/// The threshold for determining if a position movement hasreached its goa l. This has no effect for velocity PID calculations.
	uint8_t threshold;
	/// The rate at which the PID computation is run in ms
	uint8_t loopspeed;
} motor_pid_full_s_t;

/**
 * \struct motor_pid_s_t
 * 
 * Holds just the constants for a Motor's position or velocity PID controls.
 *
 * These values are in 4.4 format, meaning that a value of 0x20 represents 2.0,
 * 0x21 represents 2.0625, 0x22 represents 2.125, etc.
 */
typedef struct motor_pid_s {
	/// The feedforward constant
	uint8_t kf;
	/// The proportional constant
	uint8_t kp;
	/// The integral constants
	uint8_t ki;
	/// The derivative constant
	uint8_t kd;
} motor_pid_s_t;

#ifdef __cplusplus
namespace c {
#endif

/**
 * Sets the position for the motor in its encoder units.
 *
 * This will be the future reference point for the motor's "absolute" position.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param position
 *        The new reference position in its encoder units
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * 
 * \code
 * void autonomous() {
 *   motor_move_absolute(1, 100, 100); // Moves 100 units forward
 *   while (!((motor_get_position(1) - 100 < 105) && (motor_get_position(1) - 100 > 95))) {
 *     // Continue running this loop as long as the motor is not within +-5 units of its goal
 *     delay(2);
 *   }
 *   motor_move_absolute(1, 100, 100); // This does not cause a movement
 *   while (!((motor_get_position(1) - 100 < 105) && (motor_get_position(1) - 100 > 95))) {
 *     delay(2);
 *   }
 * 
 *   motor_set_zero_position(1, 80);
 *   motor_move_absolute(1, 100, 100); // Moves 80 units forward
 *   while (!((motor_get_position(1) - 100 < 105) && (motor_get_position(1) - 100 > 95))) {
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_set_zero_position(int8_t port, const double position);

/**
 * Sets the "absolute" zero position of the motor to its current position.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void autonomous() {
 *   motor_move_absolute(1, 100, 100); // Moves 100 units forward
 *   while (!((motor_get_position(1) - 100 < 105) && (motor_get_position(1) - 100 > 95))) {
 *     // Continue running this loop as long as the motor is not within +-5 units of its goal
 *     delay(2);
 *   }
 *   motor_move_absolute(1, 100, 100); // This does not cause a movement
 *   while (!((motor_get_position(1) - 100 < 105) && (motor_get_position(1) - 100 > 95))) {
 *     delay(2);
 *   }
 * 
 *   motor_tare_position(1);
 *   motor_move_absolute(1, 100, 100); // Moves 100 units forward
 *   while (!((motor_get_position(1) - 100 < 105) && (motor_get_position(1) - 100 > 95))) {
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_tare_position(int8_t port);

/**
 * Sets one of motor_brake_mode_e_t to the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param mode
 *        The motor_brake_mode_e_t to set for the motor
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   motor_set_brake_mode(1, E_MOTOR_BRAKE_HOLD);
 *   printf("Brake Mode: %d\n", motor_get_brake_mode(1));
 * }
 * \endcode
 */
int32_t motor_set_brake_mode(int8_t port, const motor_brake_mode_e_t mode);

/**
 * Sets the current limit for the motor in mA.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param limit
 *        The new current limit in mA
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   motor_set_current_limit(1, 1000);
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     // The motor will reduce its output at 1000 mA instead of the default 2500 mA
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_set_current_limit(int8_t port, const int32_t limit);

/**
 * Sets one of motor_encoder_units_e_t for the motor encoder.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param units
 *        The new motor encoder units
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 *
 * \b Example
 * \code
 * void initialize() {
 *   motor_set_encoder_units(1, E_MOTOR_ENCODER_DEGREES);
 *   printf("Encoder Units: %d\n", motor_get_encoder_units(1));
 * }
 * \endcode
 */
int32_t motor_set_encoder_units(int8_t port, const motor_encoder_units_e_t units);

/**
 * Sets one of motor_gearset_e_t for the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param gearset
 *        The new motor gearset
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   motor_set_gearing(1, E_MOTOR_GEARSET_06);
 *   printf("Brake Mode: %d\n", motor_get_gearing(1));
 * }
 * \endcode
 */
int32_t motor_set_gearing(int8_t port, const motor_gearset_e_t gearset);

/**
 * Sets the voltage limit for the motor in Volts.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 * \param limit
 *        The new voltage limit in Volts
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void autonomous() {
 *   motor_set_voltage_limit(1, 10000);
 *   while (true) {
 *     motor_move(1, controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y));
 *     // The motor will not output more than 10 V
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t motor_set_voltage_limit(int8_t port, const int32_t limit);

/**
 * Gets the brake mode that was set for the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return One of motor_brake_mode_e_t, according to what was set for the motor,
 * or E_MOTOR_BRAKE_INVALID if the operation failed, setting errno.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   motor_set_brake_mode(1, E_MOTOR_BRAKE_HOLD);
 *   printf("Brake Mode: %d\n", motor_get_brake_mode(1));
 * }
 * \endcode
 */
motor_brake_mode_e_t motor_get_brake_mode(int8_t port);

/**
 * Gets the current limit for the motor in mA.
 *
 * The default value is 2500 mA.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The motor's current limit in mA or PROS_ERR if the operation failed,
 * setting errno.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   printf("Motor Current Limit: %d\n", motor_get_current_limit(1));
 *   // Prints "Motor Current Limit: 2500"
 * }
 * \endcode
 */
int32_t motor_get_current_limit(int8_t port);

/**
 * Gets the encoder units that were set for the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return One of motor_encoder_units_e_t according to what is set for the motor
 * or E_MOTOR_ENCODER_INVALID if the operation failed.
 */
motor_encoder_units_e_t motor_get_encoder_units(int8_t port);

/**
 * Gets the gearset that was set for the motor.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return One of motor_gearset_e_t according to what is set for the motor,
 * or E_GEARSET_INVALID if the operation failed.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   printf("Motor Encoder Units: %d\n", motor_get_encoder_units(1));
 *   // Prints E_MOTOR_ENCODER_DEGREES by default
 * }
 * \endcode
 */
motor_gearset_e_t motor_get_gearing(int8_t port);

/**
 * Gets the voltage limit set by the user.
 *
 * Default value is 0V, which means that there is no software limitation imposed
 * on the voltage.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports |1-21|.
 * ENODEV - The port cannot be configured as a motor
 *
 * \param port
 *        The V5 port number from 1 to 21, or from -21 to -1 for reversed motors
 *
 * \return The motor's voltage limit in V or PROS_ERR if the operation failed,
 * setting errno.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   printf("Motor Voltage Limit: %d\n", motor_get_voltage_limit(1));
 *   // Prints 0 by default, indicating no limit
 * }
 * \endcode
 */
int32_t motor_get_voltage_limit(int8_t port);

///@}

///@}

#ifdef __cplusplus
}  // namespace c
}  // namespace pros
}
#endif

#endif  // _PROS_MOTORS_H_
