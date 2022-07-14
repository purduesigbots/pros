/**
 * \file pros/misc.hpp
 * \ingroup cpp-pros
 *
 * Contains prototypes for miscellaneous functions pertaining to the controller,
 * battery, and competition control.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/controller.html to
 * learn more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2022, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * \defgroup cpp-misc Miscellaneous C++ API
 * \note Additional example code for this module can be found in its [Tutorial.](@ref controller)
 */

#ifndef _PROS_MISC_HPP_
#define _PROS_MISC_HPP_

#include "pros/misc.h"

#include <cstdint>
#include <string>

namespace pros {
inline namespace v5 {
/**
 * \ingroup cpp-misc
 * 
 */
class Controller {
	/**
	 * \addtogroup cpp-misc
	 * ///@{
	 */
	public:
	/**
	 * Creates a controller object for the given controller id.
	 * 
	 * This function uses the following values of errno when an error state is reached:
	 * EINVAL - A value other than E_CONTROLLER_MASTER or E_CONTROLLER_PARTNER is given.
	 *
	 * \param id The ID of the controller (e.g. the master or partner controller).
	 * Must be one of CONTROLLER_MASTER or CONTROLLER_PARTNER
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   pros::Motor motor (1);
	 *   while (true) {
	 *     motor.move(master.get_analog(E_CONTROLLER_ANALOG_LEFT_X));
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	Controller(controller_id_e_t id);

	/**
	 * Checks if the controller is connected.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the controller
	 * port.
	 *
	 * \return 1 if the controller is connected, 0 otherwise
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Controller partner (E_CONTROLLER_PARTNER);
	 *   while (true) {
	 *     if (partner.is_connected()) {
	 *       // Use a two controller control scheme
	 *     }
	 *     else {
	 *       // Just use a single controller control scheme
	 *     }
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t is_connected(void);

	/**
	 * Gets the value of an analog channel (joystick) on a controller.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the controller
	 * port.
	 *
	 * \param channel
	 * 			  The analog channel to get.
	 * 			  Must be one of ANALOG_LEFT_X, ANALOG_LEFT_Y, ANALOG_RIGHT_X,
	 *        ANALOG_RIGHT_Y
	 *
	 * \return The current reading of the analog channel: [-127, 127].
	 * If the controller was not connected, then 0 is returned
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   pros::Motor motor (1);
	 *   while (true) {
	 *     motor.move(master.get_analog(E_CONTROLLER_ANALOG_LEFT_X));
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_analog(controller_analog_e_t channel);

	/**
	 * Gets the battery capacity of the controller.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the controller
	 * port.
	 *
	 * \return The controller's battery capacity
	 * 
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   printf("Battery Capacity: %d\n", master.get_battery_capacity());
	 * }
	 * \endcode
	 */
	std::int32_t get_battery_capacity(void);

	/**
	 * Gets the battery level of the controller.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the controller
	 * port.
	 *
	 * \return The controller's battery level
	 * 
	 * \b Example
	 * \code
	 * void initialize() {
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   printf("Battery Level: %d\n", master.get_battery_level());
	 * }
	 * \endcode
	 */
	std::int32_t get_battery_level(void);

	/**
	 * Checks if a digital channel (button) on the controller is currently
	 * pressed.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the controller
	 * port.
	 *
	 * \param button The button to read. Must be one of DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
	 *
	 * \return 1 if the button on the controller is pressed.
	 * If the controller was not connected, then 0 is returned
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   pros::Motor motor (1);
	 *   while (true) {
	 *     if (master.get_digital(E_CONTROLLER_DIGITAL_A)) {
	 *       motor.move(100);
	 *     }
	 *     else {
	 *       motor.move(0);
	 *     }
	 *   pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_digital(controller_digital_e_t button);

	/**
	 * Returns a rising-edge case for a controller button press.
	 *
	 * This function is not thread-safe.
	 * Multiple tasks polling a single button may return different results under
	 * the same circumstances, so only one task should call this function for any
	 * given button. E.g., Task A calls this function for buttons 1 and 2.
	 * Task B may call this function for button 3, but should not for buttons
	 * 1 or 2. A typical use-case for this function is to call inside opcontrol
	 * to detect new button presses, and not in any other tasks.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the controller
	 * port.
	 *
	 * \param button The button to read. Must be one of DIGITAL_{RIGHT,DOWN,LEFT,UP,A,B,Y,X,R1,R2,L1,L2}
	 *
	 * \return 1 if the button on the controller is pressed and had not been
	 * pressed the last time this function was called, 0 otherwise.
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
	 *     // Toggle pneumatics or other similar actions
	 *     }
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t get_digital_new_press(controller_digital_e_t button);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
	template <typename T>
	T convert_args(T arg) {
		return arg;
	}
	const char* convert_args(const std::string& arg) {
		return arg.c_str();
	}
#pragma GCC diagnostic pop

	/**
	 * Sets text to the controller LCD screen.
	 *
	 * \note Controller text setting is a slow process, so updates faster than 10ms
	 * when on a wired connection or 50ms over Vexnet will not be applied to the controller.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the controller
	 * port.
	 *
	 * \param line The line number at which the text will be displayed [0-2]
	 * \param col The column number at which the text will be displayed [0-14]
	 * \param fmt The format string to print to the controller
	 * \param ... The argument list for the format string
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   int count = 0;
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     if (!(count % 25)) {
	 *       // Only print every 50ms, the controller text update rate is slow
	 *       master.print(0, 0, "Counter: %d", count);
	 *     }
	 *   count++;
	 *   pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	template <typename... Params>
	std::int32_t print(std::uint8_t line, std::uint8_t col, const char* fmt, Params... args) {
		return pros::c::controller_print(_id, line, col, fmt, convert_args(args)...);
	}

	/**
	 * Sets text to the controller LCD screen.
	 *
	 * \note Controller text setting is a slow process, so updates faster than
	 * 10ms when on a wired connection or 50ms over Vexnet will not be applied to the controller.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the controller
	 * port.
	 * EAGAIN - Could not send the text to the controller.
	 *
	 * \param line The line number at which the text will be displayed [0-2]
	 * \param col The column number at which the text will be displayed [0-14]
	 * \param str The pre-formatted string to print to the controller
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   int count = 0;
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     if (!(count % 25)) {
	 *     // Only print every 50ms, the controller text update rate is slow
	 *     master.set_text(0, 0, "Example text");
	 *     }
	 *     count++;
	 *     pros::delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t set_text(std::uint8_t line, std::uint8_t col, const char* str);
	std::int32_t set_text(std::uint8_t line, std::uint8_t col, const std::string& str);

	/**
	 * Clears an individual line of the controller screen.
	 *
	 * \note Controller text setting is a slow process, so updates faster than
	 * 10ms when on a wired connection or 50ms over Vexnet will not be applied to the controller.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the controller
	 * port.
	 * EAGAIN - Could not send the text to the controller.
	 *
	 * \param line The line number to clear [0-2]
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   master.set_text(0, 0, "Example");
	 *   pros::delay(100);
	 *   master.clear_line(0);
	 * }
	 * \endcode
	 */
	std::int32_t clear_line(std::uint8_t line);

	/**
	 * Rumble the controller.
	 *
	 * \note Controller rumble activation is a slow process, so updates faster than 10ms
	 * when on a wired connection or 50ms over Vexnet will not be applied to the controller.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the controller
	 * port.
	 *
	 * \param rumble_pattern A string consisting of the characters '.', '-', and ' ', where dots
	 * are short rumbles, dashes are long rumbles, and spaces are pauses.
	 * Maximum supported length is 8 characters.
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   int count = 0;
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   while (true) {
	 *     if (!(count % 25)) {
	 *       // Only print every 50ms, the controller text update rate is slow
	 *       master.rumble(". - . -");
	 *     }
	 *   count++;
	 *   delay(2);
	 *   }
	 * }
	 * \endcode
	 */
	std::int32_t rumble(const char* rumble_pattern);

	/**
	 * Clears all of the lines on the controller screen.
	 *
	 * \note Controller text setting is a slow process, so updates faster than
	 * 10ms when on a wired connection or 50ms over Vexnet will not be applied
	 * to the controller. On vexOS version 1.0.0 this function will block for 110 ms.
	 *
	 * This function uses the following values of errno when an error state is
	 * reached:
	 * EACCES - Another resource is currently trying to access the controller
	 * port.
	 * EAGAIN - Could not send the text to the controller.
	 *
	 * \return 1 if the operation was successful or PROS_ERR if the operation
	 * failed, setting errno.
	 * 
	 * \b Example
	 * \code
	 * void opcontrol() {
	 *   pros::Controller master (E_CONTROLLER_MASTER);
	 *   master.set_text(0, 0, "Example");
	 *   pros::delay(100);
	 *   master.clear();
	 * }
	 * \endcode
	 */
	std::int32_t clear(void);

	private:
	controller_id_e_t _id;
	///@}
};
} //  namespace v5

namespace battery {
/**
 * \addtogroup cpp-misc
 * ///@{
 */

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
 *   std::cout << "Battery Level:" << pros::battery::get_capacity();
 * }
 * \endcode
 */
double get_capacity(void);

/**
 * Gets the current current of the battery in milliamps, as reported by VEXos.
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
 *   std::cout << "Battery Current:" << pros::battery::get_current();
 * }
 * \endcode
 */
int32_t get_current(void);

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
 *   std::cout << "Battery Temperature:" << pros::battery::get_temperature();
 * }
 * \endcode
 */
double get_temperature(void);

/**
 * Gets the current capacity of the battery in millivolts, as reported by VEXos.
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
 *   std::cout << "Battery Voltage:" << pros::battery::get_voltage();
 * }
 * \endcode
 */
int32_t get_voltage(void);
///@}
}  // namespace battery

namespace competition {
/**
 * \addtogroup cpp-misc
 * ///@{
 */

/**
 * Get the current status of the competition control.
 *
 * \return The competition control status as a mask of bits with
 * COMPETITION_{ENABLED,AUTONOMOUS,CONNECTED}.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   if (pros::competition::get_status() & COMPETITION_CONNECTED == true) {
 *     // Field Control is Connected
 *     // Run LCD Selector code or similar
 *   }
 * }
 * \endcode
 */
std::uint8_t get_status(void);

/**
 * \return True if the V5 Brain is in autonomous mode, false otherwise.
 * 
 * \b Example
 * \code
 * void my_task_fn(void* ignore) {
 *   while (!pros::competition::is_autonomous()) {
 *     // Wait to do anything until autonomous starts
 *     pros::delay(2);
 *   }
 *   while (pros::competition::is_autonomous()) {
 *     // Run whatever code is desired to just execute in autonomous
 *   }
 * }
 * 
 * void initialize() {
 *   pros::Task my_task (my_task_fn, NULL, TASK_PRIO_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
 * }
 * \endcode
 */
std::uint8_t is_autonomous(void);

/**
 * \return True if the V5 Brain is connected to competition control, false otherwise.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   if (pros::competition::is_connected()) {
 *     // Field Control is Connected
 *     // Run LCD Selector code or similar
 *   }
 * }
 * \endcode
 */
std::uint8_t is_connected(void);

/**
 * \return True if the V5 Brain is disabled, false otherwise.
 * 
 * \b Example
 * \code
 * void my_task_fn(void* ignore) {
 *   while (!pros::competition::is_disabled()) {
 *     // Run competition tasks (like Lift Control or similar)
 *   }
 * }
 * 
 * void initialize() {
 *   pros::Task my_task (my_task_fn, NULL, TASK_PRIO_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
 * }
 * \endcode
 */
std::uint8_t is_disabled(void);
///@}
}  // namespace competition

namespace usd {
/**
 * \addtogroup cpp-misc
 * ///@{
 */

/**
 * Checks if the SD card is installed.
 *
 * \return 1 if the SD card is installed, 0 otherwise
 * 
 * \b Example
 * \code
 * void opcontrol() {
 *   std::cout << pros::usd::is_installed() << std::endl;
 * }
 * \endcode
 */
std::int32_t is_installed(void);
///@}
}  // namespace usd
}  // namespace pros

#endif  // _PROS_MISC_HPP_
