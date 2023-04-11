/**
 * \file pros/misc.h
 * \ingroup c-misc
 *
 * Contains prototypes for miscellaneous functions pertaining to the controller,
 * battery, and competition control.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * \defgroup c-misc Miscellaneous C API
 * \note Additional example code for this module can be found in its [Tutorial.](@ref controller)
 */

#ifndef _PROS_MISC_H_
#define _PROS_MISC_H_

#include <stdint.h>

#define NUM_V5_PORTS (22)

/**
 * \ingroup c-misc
 */

/**
 * \addtogroup c-misc
 *  @{
 */

/// \name V5 Competition
//@{

#define COMPETITION_DISABLED (1 << 0)
#define COMPETITION_AUTONOMOUS (1 << 1)
#define COMPETITION_CONNECTED (1 << 2)

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

/**
 * \fn competition_get_status(void)
 * Get the current status of the competition control.
 *
 * \return The competition control status as a mask of bits with
 * COMPETITION_{ENABLED,AUTONOMOUS,CONNECTED}.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   if (competition_get_status() & COMPETITION_CONNECTED == true) {
 *     // Field Control is Connected
 *     // Run LCD Selector code or similar
 *   }
 * }
 * \endcode
 */
uint8_t competition_get_status(void);

#ifdef __cplusplus
}
}
}
#endif

/**
 * \fn competition_is_disabled()
 * 
 * \return True if the V5 Brain is disabled, false otherwise.
 * 
 * \b Example
 * \code
 * void my_task_fn(void* ignore) {
 *   while (!competition_is_disabled()) {
 *   // Run competition tasks (like Lift Control or similar)
 *   }
 * }
 * 
 * void initialize() {
 *   task_t my_task = task_create(my_task_fn, NULL, TASK_PRIO_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
 * }
 * \endcode
 */
#define competition_is_disabled() ((competition_get_status() & COMPETITION_DISABLED) != 0)

/**
 * \return True if the V5 Brain is connected to competition control, false otherwise.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   if (competition_is_connected()) {
 *     // Field Control is Connected
 *     // Run LCD Selector code or similar
 *   }
 * }
 * \endcode
 */
#define competition_is_connected() ((competition_get_status() & COMPETITION_CONNECTED) != 0)

/**
 * \return True if the V5 Brain is in autonomous mode, false otherwise.
 * 
 * \b Example
 * \code
 * void my_task_fn(void* ignore) {
 *   while (!competition_is_autonomous()) {
 *     // Wait to do anything until autonomous starts
 *     delay(2);
 *   }
 *   while (competition_is_autonomous()) {
 *     // Run whatever code is desired to just execute in autonomous
 *     }
 * }
 * 
 * void initialize() {
 *   task_t my_task = task_create(my_task_fn, NULL, TASK_PRIO_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
 * }
 * \endcode
 */
#define competition_is_autonomous() ((competition_get_status() & COMPETITION_AUTONOMOUS) != 0)

///@}

/// \name V5 Controller
///@{
#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

/**
 * \enum
 */
typedef enum {
	///The master controller.
	E_CONTROLLER_MASTER = 0,
	///The partner controller.
	E_CONTROLLER_PARTNER } controller_id_e_t;

/**
 * \enum
 */
typedef enum {
	///The horizontal axis of the controller’s left analog stick.
	E_CONTROLLER_ANALOG_LEFT_X = 0,
	///The vertical axis of the controller’s left analog stick.
	E_CONTROLLER_ANALOG_LEFT_Y,
	///The horizontal axis of the controller’s right analog stick.
	E_CONTROLLER_ANALOG_RIGHT_X,
	///The vertical axis of the controller’s right analog stick.
	E_CONTROLLER_ANALOG_RIGHT_Y
} controller_analog_e_t;

/**
 * \enum
 */
typedef enum {
	///The first trigger on the left side of the controller.
	E_CONTROLLER_DIGITAL_L1 = 6,
	///The second trigger on the left side of the controller.
	E_CONTROLLER_DIGITAL_L2,
	///The first trigger on the right side of the controller.
	E_CONTROLLER_DIGITAL_R1,
	///The second trigger on the right side of the controller.
	E_CONTROLLER_DIGITAL_R2,
	///The up arrow on the left arrow pad of the controller.
	E_CONTROLLER_DIGITAL_UP,
	///The down arrow on the left arrow pad of the controller.
	E_CONTROLLER_DIGITAL_DOWN,
	///The left arrow on the left arrow pad of the controller.
	E_CONTROLLER_DIGITAL_LEFT,
	///The right arrow on the left arrow pad of the controller.
	E_CONTROLLER_DIGITAL_RIGHT,
	///The ‘X’ button on the right button pad of the controller.
	E_CONTROLLER_DIGITAL_X,
	///The ‘B’ button on the right button pad of the controller.
	E_CONTROLLER_DIGITAL_B,
	///The ‘Y’ button on the right button pad of the controller.
	E_CONTROLLER_DIGITAL_Y,
	///The ‘A’ button on the right button pad of the controller.
	E_CONTROLLER_DIGITAL_A
} controller_digital_e_t;

#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define CONTROLLER_MASTER pros::E_CONTROLLER_MASTER
#define CONTROLLER_PARTNER pros::E_CONTROLLER_PARTNER
#define ANALOG_LEFT_X pros::E_CONTROLLER_ANALOG_LEFT_X
#define ANALOG_LEFT_Y pros::E_CONTROLLER_ANALOG_LEFT_Y
#define ANALOG_RIGHT_X pros::E_CONTROLLER_ANALOG_RIGHT_X
#define ANALOG_RIGHT_Y pros::E_CONTROLLER_ANALOG_RIGHT_Y
#define DIGITAL_L1 pros::E_CONTROLLER_DIGITAL_L1
#define DIGITAL_L2 pros::E_CONTROLLER_DIGITAL_L2
#define DIGITAL_R1 pros::E_CONTROLLER_DIGITAL_R1
#define DIGITAL_R2 pros::E_CONTROLLER_DIGITAL_R2
#define DIGITAL_UP pros::E_CONTROLLER_DIGITAL_UP
#define DIGITAL_DOWN pros::E_CONTROLLER_DIGITAL_DOWN
#define DIGITAL_LEFT pros::E_CONTROLLER_DIGITAL_LEFT
#define DIGITAL_RIGHT pros::E_CONTROLLER_DIGITAL_RIGHT
#define DIGITAL_X pros::E_CONTROLLER_DIGITAL_X
#define DIGITAL_B pros::E_CONTROLLER_DIGITAL_B
#define DIGITAL_Y pros::E_CONTROLLER_DIGITAL_Y
#define DIGITAL_A pros::E_CONTROLLER_DIGITAL_A
#else
#define CONTROLLER_MASTER E_CONTROLLER_MASTER
#define CONTROLLER_PARTNER E_CONTROLLER_PARTNER
#define ANALOG_LEFT_X E_CONTROLLER_ANALOG_LEFT_X
#define ANALOG_LEFT_Y E_CONTROLLER_ANALOG_LEFT_Y
#define ANALOG_RIGHT_X E_CONTROLLER_ANALOG_RIGHT_X
#define ANALOG_RIGHT_Y E_CONTROLLER_ANALOG_RIGHT_Y
#define DIGITAL_L1 E_CONTROLLER_DIGITAL_L1
#define DIGITAL_L2 E_CONTROLLER_DIGITAL_L2
#define DIGITAL_R1 E_CONTROLLER_DIGITAL_R1
#define DIGITAL_R2 E_CONTROLLER_DIGITAL_R2
#define DIGITAL_UP E_CONTROLLER_DIGITAL_UP
#define DIGITAL_DOWN E_CONTROLLER_DIGITAL_DOWN
#define DIGITAL_LEFT E_CONTROLLER_DIGITAL_LEFT
#define DIGITAL_RIGHT E_CONTROLLER_DIGITAL_RIGHT
#define DIGITAL_X E_CONTROLLER_DIGITAL_X
#define DIGITAL_B E_CONTROLLER_DIGITAL_B
#define DIGITAL_Y E_CONTROLLER_DIGITAL_Y
#define DIGITAL_A E_CONTROLLER_DIGITAL_A
#endif
#endif

/**
 * \def Given an id and a port, this macro sets the port variable based on the id and allows the mutex to take that port.
 * 
 * \returns error (in the function/scope it's in) if the controller failed to connect or an invalid id is given.
*/
#define CONTROLLER_PORT_MUTEX_TAKE(id, port) \
	switch (id) {							\
		case E_CONTROLLER_MASTER:			\
			port = V5_PORT_CONTROLLER_1;	\
			break;							\
		case E_CONTROLLER_PARTNER:			\
			port = V5_PORT_CONTROLLER_2;	\
			break;							\
		default:							\
			errno = EINVAL;					\
			return PROS_ERR;				\
	}										\
	if (!internal_port_mutex_take(port)) {	\
		errno = EACCES;						\
		return PROS_ERR;					\
	}										\

#ifdef __cplusplus
namespace c {
#endif

/**
 * Checks if the controller is connected.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is
 * given.
 * EACCES - Another resource is currently trying to access the controller port.
 *
 * \param id
 *        The ID of the controller (e.g. the master or partner controller).
 *        Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 *
 * \return 1 if the controller is connected, 0 otherwise
 * 
 * \b Example
 * \code
 * void initialize() {
 *   if (competition_is_connected()) {
 *     // Field Control is Connected
 *     // Run LCD Selector code or similar
 *   }
 * }
 * \endcode
 */
int32_t controller_is_connected(controller_id_e_t id);

/**
 * Gets the value of an analog channel (joystick) on a controller.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is
 * given.
 * EACCES - Another resource is currently trying to access the controller port.
 *
 * \param id
 *        The ID of the controller (e.g. the master or partner controller).
 *        Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 * \param channel
 *        The analog channel to get.
 *        Must be one of ANALOG_LEFT_X, ANALOG_LEFT_Y, ANALOG_RIGHT_X,
 *        ANALOG_RIGHT_Y
 *
 * \return The current reading of the analog channel: [-127, 127].
 * If the controller was not connected, then 0 is returned
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
int32_t controller_get_analog(controller_id_e_t id, controller_analog_e_t channel);

/**
 * Gets the battery capacity of the given controller.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is
 * given.
 * EACCES - Another resource is currently trying to access the controller port.
 *
 * \param id
 *        The ID of the controller (e.g. the master or partner controller).
 *        Must be one of E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER
 *
 * \return The controller's battery capacity
 * 
 * \b Example
 * \code
 * void initialize() {
 *   printf("Battery Capacity: %d\n", controller_get_battery_capacity(E_CONTROLLER_MASTER));
 * }
 * \endcode
 */
int32_t controller_get_battery_capacity(controller_id_e_t id);

/**
 * Gets the battery level of the given controller.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is
 * given.
 * EACCES - Another resource is currently trying to access the controller port.
 *
 * \param id
 *        The ID of the controller (e.g. the master or partner controller).
 *        Must be one of E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER
 *
 * \return The controller's battery level
 * 
 * \b Example
 * \code
 * void initialize() {
 *   printf("Battery Level: %d\n", controller_get_battery_level(E_CONTROLLER_MASTER));
 * }
 * \endcode
 */
int32_t controller_get_battery_level(controller_id_e_t id);

/**
 * Checks if a digital channel (button) on the controller is currently pressed.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is
 * given.
 * EACCES - Another resource is currently trying to access the controller port.
 *
 * \param id
 *        The ID of the controller (e.g. the master or partner controller).
 *        Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 * \param button
 *        The button to read.
 *        Must be one of DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
 *
 * \return 1 if the button on the controller is pressed.
 * If the controller was not connected, then 0 is returned
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   while (true) {
 *   if (controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_A)) {
 *     motor_set(1, 100);
 *   }
 *   else {
 *     motor_set(1, 0);
 *   }
 *     delay(2);
 *   }
 * }

 * \endcode
 */
int32_t controller_get_digital(controller_id_e_t id, controller_digital_e_t button);

/**
 * Returns a rising-edge case for a controller button press.
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
 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is
 * given.
 * EACCES - Another resource is currently trying to access the controller port.
 *
 * \param id
 *        The ID of the controller (e.g. the master or partner controller).
 *        Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 * \param button
 * 			  The button to read. Must be one of
 *        DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
 *
 * \return 1 if the button on the controller is pressed and had not been pressed
 * the last time this function was called, 0 otherwise.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 * while (true) {
 *   if (controller_get_digital_new_press(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_A)) {
 *     // Toggle pneumatics or other similar actions
 *   }
 * 
 *   delay(2);
 *   }
 * }
 * \endcode
 */
int32_t controller_get_digital_new_press(controller_id_e_t id, controller_digital_e_t button);

/**
 * Sets text to the controller LCD screen.
 *
 * \note Controller text setting is a slow process, so updates faster than 10ms
 * when on a wired connection or 50ms over Vexnet will not be applied to the controller.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is
 * given.
 * EACCES - Another resource is currently trying to access the controller port.
 * EAGAIN - Could not send the text to the controller.
 *
 * \param id
 *        The ID of the controller (e.g. the master or partner controller).
 *        Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 * \param line
 *        The line number at which the text will be displayed [0-2]
 * \param col
 *        The column number at which the text will be displayed [0-14]
 * \param fmt
 *        The format string to print to the controller
 * \param ...
 *        The argument list for the format string
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   int count = 0;
 *   while (true) {
 *   if (!(count % 25)) {
 *     // Only print every 50ms, the controller text update rate is slow
 *     controller_print(E_CONTROLLER_MASTER, 0, 0, "Counter: %d", count);
 *   }
 *     count++;
 *     delay(2);
 *   }
 * }
 * \endcode
 */
int32_t controller_print(controller_id_e_t id, uint8_t line, uint8_t col, const char* fmt, ...);

/**
 * Sets text to the controller LCD screen.
 *
 * \note Controller text setting is a slow process, so updates faster than 10ms
 * when on a wired connection or 50ms over Vexnet will not be applied to the controller.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is
 * given.
 * EACCES - Another resource is currently trying to access the controller port.
 * EAGAIN - Could not send the text to the controller.
 *
 * \param id
 *        The ID of the controller (e.g. the master or partner controller).
 *        Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 * \param line
 *        The line number at which the text will be displayed [0-2]
 * \param col
 *        The column number at which the text will be displayed [0-14]
 * \param str
 *        The pre-formatted string to print to the controller
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   int count = 0;
 *   while (true) {
 *     if (!(count % 25)) {
 *       // Only print every 50ms, the controller text update rate is slow
 *       controller_set_text(E_CONTROLLER_MASTER, 0, 0, "Example text");
 *     }
 *   count++;
 *   delay(2);
 *   }
 * }
 * \endcode
 */
int32_t controller_set_text(controller_id_e_t id, uint8_t line, uint8_t col, const char* str);

/**
 * Clears an individual line of the controller screen.
 *
 * \note Controller text setting is currently in beta, so continuous, fast
 * updates will not work well.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is
 * given.
 * EACCES - Another resource is currently trying to access the controller port.
 *
 * \param id
 *        The ID of the controller (e.g. the master or partner controller).
 *        Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 * \param line
 *        The line number to clear [0-2]
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   controller_set_text(E_CONTROLLER_MASTER, 0, 0, "Example");
 *   delay(100);
 *   controller_clear_line(E_CONTROLLER_MASTER, 0);
 * }
 * \endcode
 */
int32_t controller_clear_line(controller_id_e_t id, uint8_t line);

/**
 * Clears all of the lines on the controller screen.
 *
 * \note Controller text setting is a slow process, so updates faster than 10ms
 * when on a wired connection or 50ms over Vexnet will not be applied to the controller.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is
 * given.
 * EACCES - Another resource is currently trying to access the controller port.
 * EAGAIN - Could not send the text to the controller.
 *
 * \param id
 *        The ID of the controller (e.g. the master or partner controller).
 *        Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   controller_set_text(E_CONTROLLER_MASTER, 0, 0, "Example");
 *   delay(100);
 *   controller_clear(E_CONTROLLER_MASTER);
 * }
 * \endcode
 */
int32_t controller_clear(controller_id_e_t id);

/**
 * Rumble the controller.
 *
 * \note Controller rumble activation is a slow process, so updates faster than 10ms
 * when on a wired connection or 50ms over Vexnet will not be applied to the controller.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is
 * given.
 * EACCES - Another resource is currently trying to access the controller port.
 *
 * \param id
 *				The ID of the controller (e.g. the master or partner controller).
 *				Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
 * \param rumble_pattern
 *				A string consisting of the characters '.', '-', and ' ', where dots
 *				are short rumbles, dashes are long rumbles, and spaces are pauses.
 *				Maximum supported length is 8 characters.
 *
 * \return 1 if the operation was successful or PROS_ERR if the operation
 * failed, setting errno.
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   int count = 0;
 *   while (true) {
 *   if (!(count % 25)) {
 *     // Only send every 50ms, the controller update rate is slow
 *     controller_rumble(E_CONTROLLER_MASTER, ". - . -");
 *   }
 *   count++;
 *   delay(2);
 *   }
 * }
 * \endcode
 */
int32_t controller_rumble(controller_id_e_t id, const char* rumble_pattern);

/**
 * Gets the current voltage of the battery, as reported by VEXos.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EACCES - Another resource is currently trying to access the battery port.
 *
 * \return The current voltage of the battery
 * 
 * \b Example
 * \code
 * void initialize() {
 *   printf("Battery's Voltage: %d\n", battery_get_voltage());
 * }
 * \endcode
 */
int32_t battery_get_voltage(void);

/**
 * Gets the current current of the battery, as reported by VEXos.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EACCES - Another resource is currently trying to access the battery port.
 *
 * \return The current current of the battery
 * 
 * \b Example
 * \code
 * void initialize() {
 *   printf("Battery Current: %d\n", battery_get_current());
 * }
 * \endcode
 */
int32_t battery_get_current(void);

/**
 * Gets the current temperature of the battery, as reported by VEXos.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EACCES - Another resource is currently trying to access the battery port.
 *
 * \return The current temperature of the battery
 * 
 * \b Example
 * \code
 * void initialize() {
 *   printf("Battery's Temperature: %d\n", battery_get_temperature());
 * }
 * \endcode
 */
double battery_get_temperature(void);

/**
 * Gets the current capacity of the battery, as reported by VEXos.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * EACCES - Another resource is currently trying to access the battery port.
 *
 * \return The current capacity of the battery
 * 
 * \b Example
 * \code
 * void initialize() {
 *   printf("Battery Level: %d\n", battery_get_capacity());
 * }
 * \endcode
 */
double battery_get_capacity(void);

/**
 * Checks if the SD card is installed.
 *
 * \return 1 if the SD card is installed, 0 otherwise
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   printf("%i", usd_is_installed());
 * }
 * \endcode
 */
int32_t usd_is_installed(void);

/******************************************************************************/
/**                              Date and Time                               **/
/******************************************************************************/

extern const char* baked_date;
extern const char* baked_time;

typedef struct {
	uint16_t year; // Year - 1980
	uint8_t day;
	uint8_t month; // 1 = January
} date_s_t;

typedef struct {
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t sec_hund; // hundredths of a second
} time_s_t;

///@}

///@}

#ifdef __cplusplus
}
} // namespace pros
}
#endif

#endif  // _PROS_MISC_H_
