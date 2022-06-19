/**
 * \file pros/llemu.hpp
 * \ingroup cpp-llemu
 * 
 * Legacy LCD Emulator
 *
 * This file defines a high-level API for emulating the three-button, UART-based
 * VEX LCD, containing a set of functions that facilitate the use of a software-
 * emulated version of the classic VEX LCD module.
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/llemu.html to learn
 * more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * \defgroup cpp-llemu LLEMU (Legacy LCD Emulator) C++ API
 * \note Additional example code for this module can be found in its [Tutorial](@ref llemu).
 */

#ifndef _PROS_LLEMU_HPP_
#define _PROS_LLEMU_HPP_

#include <cstdint>
#include <string>

#include "pros/llemu.h"

namespace pros {

/**
 * \ingroup cpp-llemu
 */
namespace lcd {

/**
 * \addtogroup cpp-llemu
 *  @{
 */

/// \name Functions
/// These functions allow programmers to manipilate the emulated classic VEX LCD module.

/**
 * Checks whether the emulated three-button LCD has already been initialized.
 *
 * \return True if the LCD has been initialized or false if not.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 *   std::cout << "Is the LCD initialized?" << pros::is_initialized();
 *   // Will Display True
 * }
 * \endcode
 */
bool is_initialized(void);

/**
 * Creates an emulation of the three-button, UART-based VEX LCD on the display.
 *
 * \return True if the LCD was successfully initialized, or false if it has
 * already been initialized.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 *   pros::lcd::set_text(1, "Hello World!");
 * }
 * \endcode
 */
bool initialize(void);

/**
 * Turns off the Legacy LCD Emulator.
 *
 * Calling this function will clear the entire display, and you will not be able
 * to call any further LLEMU functions until another call to lcd_initialize.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The LCD has not been initialized. Call lcd_initialize() first.
 *
 * \return True if the operation was successful, or false otherwise, setting
 * errno values as specified above.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 *   pros::lcd::set_text(1, "Hello World!");
 *   pros::lcd::shutdown(); // All done with the LCD
 * }
 * \endcode
 */
bool shutdown(void);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
namespace {
template <typename T>
T convert_args(T arg) {
	return arg;
}
const char* convert_args(const std::string& arg) {
	return arg.c_str();
}
}  // namespace
#pragma GCC diagnostic pop

/**
 * Displays a formatted string on the emulated three-button LCD screen.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO  - The LCD has not been initialized. Call lcd_initialize() first.
 * EINVAL - The line number specified is not in the range [0-7]
 *
 * \param line
 *        The line on which to display the text [0-7]
 * \param fmt
 *        Format string
 * \param ...
 *        Optional list of arguments for the format string
 *
 * \return True if the operation was successful, or false otherwise, setting
 * errno values as specified above.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 * }
 * 
 * void opcontrol {
 *   while (true) {
 *     pros::lcd::print(0, "Buttons Bitmap: %d\n", pros::lcd::read_buttons());
 *     delay(20);
 *   }
 * }
 * \endcode
 */
template <typename... Params>
bool print(std::int16_t line, const char* fmt, Params... args) {
	return pros::c::lcd_print(line, fmt, convert_args(args)...);
}

/**
 * Displays a string on the emulated three-button LCD screen.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO  - The LCD has not been initialized. Call lcd_initialize() first.
 * EINVAL - The line number specified is not in the range [0-7]
 *
 * \param line
 *        The line on which to display the text [0-7]
 * \param text
 *        The text to display
 *
 * \return True if the operation was successful, or false otherwise, setting
 * errno values as specified above.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 * }
 * 
 * void opcontrol {
 *   while (true) {
 *     pros::lcd::print(0, "Buttons Bitmap: %d\n", pros::lcd::read_buttons());
 *     delay(20);
 *   }
 * }
 * \endcode
 */
bool set_text(std::int16_t line, std::string text);

/**
 * Clears the contents of the emulated three-button LCD screen.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO  - The LCD has not been initialized. Call lcd_initialize() first.
 * EINVAL - The line number specified is not in the range [0-7]
 *
 * \return True if the operation was successful, or false otherwise, setting
 * errno values as specified above.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 *   pros::lcd::set_text(1, "Hello World!");
 *   pros::lcd::clear(); // No more text will be displayed
 * }
 * \endcode
 */
bool clear(void);

/**
 * Clears the contents of a line of the emulated three-button LCD screen.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO  - The LCD has not been initialized. Call lcd_initialize() first.
 * EINVAL - The line number specified is not in the range [0-7]
 *
 * \param line
 *        The line to clear
 *
 * \return True if the operation was successful, or false otherwise, setting
 * errno values as specified above.
 * 
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 *   pros::lcd::set_text(1, "Hello World!");
 *   pros::lcd::clear_line(1); // No more text will be displayed
 * }
 * \endcode
 */
bool clear_line(std::int16_t line);

using lcd_btn_cb_fn_t = void (*)(void);

/**
 * Changes the color of the LCD background to a provided color expressed in
 * type lv_color_t.
 *
 * \param color
 *         A color of type lv_color_t
 * 
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 *   pros::lcd::set_background_color(COLOR_BLACK);
 * }
 * \endcode
 */
void set_background_color(lv_color_t color);

/**
 * Changes the color of the LCD background to a provided color expressed in RGB
 * form, with three values of type uint8_t.
 *
 * \param r
 *         A value of type uint8_t, with a range of 0 to 255, representing the
 *         red value of a color
 * 
 * \param g
 *         A value of type uint8_t, with a range of 0 to 255, representing the
 *         green value of a color
 * 
 * \param b
 *         A value of type uint8_t, with a range of 0 to 255, representing the
 *         blue value of a color
 * 
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 *   pros::lcd::set_background_color(0, 0, 0);
 * }
 * \endcode
 */
void set_background_color(std::uint8_t r, std::uint8_t g, std::uint8_t b);

/**
 * Changes the text color of the LCD to a provided color expressed in
 * type lv_color_t.
 *
 * \param color
 *         A color of type lv_color_t
 *
 * \return void
 * 
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 *   pros::lcd::set_text_color(COLOR_WHITE);
 * }
 * \endcode
 */
void set_text_color(lv_color_t color);

/**
 * Changes the text color of the LCD to a provided color expressed in RGB
 * form, with three values of type uint8_t.
 *
 * \param r
 *         A value of type uint8_t, with a range of 0 to 255, representing the
 *         red value of a color
 *
 * \param g
 *         A value of type uint8_t, with a range of 0 to 255, representing the
 *         green value of a color
 *
 * \param b
 *         A value of type uint8_t, with a range of 0 to 255, representing the
 *         blue value of a color
 *
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 *   pros::lcd::set_text_color(255, 255, 255);
 * }
 * \endcode
 */
void set_text_color(std::uint8_t r, std::uint8_t g, std::uint8_t b);

/**
 * Registers a callback function for the leftmost button.
 *
 * When the leftmost button on the emulated three-button LCD is pressed, the
 * user-provided callback function will be invoked.
 *
 * \param cb
 *        A callback function of type lcd_btn_cb_fn_t(void (*cb)(void))
 * 
 * \b Example
 * \code
 * void on_center_button() {
 *   static bool pressed = false;
 *   pressed = !pressed;
 *   if (pressed) {
 *     pros::lcd::set_text(2, "I was pressed!");
 *   } else {
 *     pros::lcd::clear_line(2);
 *   }
 * }
 * 
 * void initialize() {
 *   pros::lcd::initialize();
 *   pros::lcd::register_btn0_cb(on_center_button);
 * }
 * \endcode
 */
void register_btn0_cb(lcd_btn_cb_fn_t cb);

/**
 * Registers a callback function for the center button.
 *
 * When the center button on the emulated three-button LCD is pressed, the
 * user-provided callback function will be invoked.
 *
 * \param cb
 *         A callback function of type lcd_btn_cb_fn_t(void (*cb)(void))
 * 
 * \b Example
 * \code
 * void on_center_button() {
 *   static bool pressed = false;
 *   pressed = !pressed;
 *   if (pressed) {
 *     pros::lcd::set_text(2, "I was pressed!");
 *   } else {
 *     pros::lcd::clear_line(2);
 *   }
 * }
 * 
 * void initialize() {
 *   pros::lcd::initialize();
 *   pros::lcd::register_btn1_cb(on_center_button);
 * }
 * \endcode
 */
void register_btn1_cb(lcd_btn_cb_fn_t cb);

/**
 * Registers a callback function for the rightmost button.
 *
 * When the rightmost button on the emulated three-button LCD is pressed, the
 * user-provided callback function will be invoked.
 *
 * \param cb
 *         A callback function of type lcd_btn_cb_fn_t(void (*cb)(void))
 * 
 * \b Example
 * \code
 * void on_center_button() {
 *   static bool pressed = false;
 *   pressed = !pressed;
 *   if (pressed) {
 *     pros::lcd::set_text(2, "I was pressed!");
 *   } else {
 *     pros::lcd::clear_line(2);
 *   }
 * }
 * 
 * void initialize() {
 *   pros::lcd::initialize();
 *   pros::lcd::register_btn2_cb(on_center_button);
 * }
 * \endcode
 */
void register_btn2_cb(lcd_btn_cb_fn_t cb);

/**
 * Gets the button status from the emulated three-button LCD.
 *
 * The value returned is a 3-bit integer where 1 0 0 indicates the left button
 * is pressed, 0 1 0 indicates the center button is pressed, and 0 0 1
 * indicates the right button is pressed. 0 is returned if no buttons are
 * currently being pressed.
 *
 * Note that this function is provided for legacy API compatibility purposes,
 * with the caveat that the V5 touch screen does not actually support pressing
 * multiple points on the screen at the same time.
 *
 * \return The buttons pressed as a bit mask
 * 
 * \b Example
 * \code
 * void initialize() {
 *   pros::lcd::initialize();
 * }
 * 
 * void opcontrol() {
 *   while (true) {
 *     printf("Buttons Bitmap: %d\n", pros::lcd::read_buttons());
 *     delay(20);
 *   }
 * }
 * \endcode
 */
std::uint8_t read_buttons(void);

///@}

}  // namespace lcd
}  // namespace pros

#endif  // _PROS_LLEMU_HPP_