/*
 * \file pros/llemu.h
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
 * Copyright (c) 2017-2019, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_LLEMU_H_
#define _PROS_LLEMU_H_

#include <errno.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#include "display/lvgl.h"
#pragma GCC diagnostic pop

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

typedef void (*lcd_btn_cb_fn_t)(void);

#define LCD_BTN_LEFT 4
#define LCD_BTN_CENTER 2
#define LCD_BTN_RIGHT 1

typedef struct lcd_s {
	lv_obj_t* frame;
	lv_obj_t* screen;
	lv_obj_t* lcd_text[8];
	lv_obj_t* btn_container;
	lv_obj_t* btns[3];             // < 0 => left; 1 => center; 2 => right
	lcd_btn_cb_fn_t callbacks[3];  // < 0 => left; 1 => center; 2 => right
	volatile uint8_t touch_bits;   // < 4 => left; 2 => center; 1 => right (no
	                               // multitouch support)
} lcd_s_t;

#ifdef __cplusplus
namespace c {
#endif

/**
 * Checks whether the emulated three-button LCD has already been initialized.
 *
 * \return True if the LCD has been initialized or false if not.
 */
bool lcd_is_initialized(void);

/**
 * Creates an emulation of the three-button, UART-based VEX LCD on the display.
 *
 * \return True if the LCD was successfully initialized, or false if it has
 * already been initialized.
 */
bool lcd_initialize(void);

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
 */
bool lcd_shutdown(void);

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
 */
bool lcd_print(int16_t line, const char* fmt, ...);

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
 */
bool lcd_set_text(int16_t line, const char* text);

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
 */
bool lcd_clear(void);

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
 */
bool lcd_clear_line(int16_t line);

/**
 * Registers a callback function for the leftmost button.
 *
 * When the leftmost button on the emulated three-button LCD is pressed, the
 * user-provided callback function will be invoked.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO  - The LCD has not been initialized. Call lcd_initialize() first.
 *
 * \param cb
 *        A callback function of type lcd_btn_cb_fn_t (void (*cb)(void))
 *
 * \return True if the operation was successful, or false otherwise, setting
 * errno values as specified above.
 */
bool lcd_register_btn0_cb(lcd_btn_cb_fn_t cb);

/**
 * Registers a callback function for the center button.
 *
 * When the center button on the emulated three-button LCD is pressed, the
 * user-provided callback function will be invoked.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO  - The LCD has not been initialized. Call lcd_initialize() first.
 *
 * \param cb
 *        A callback function of type lcd_btn_cb_fn_t (void (*cb)(void))
 *
 * \return True if the operation was successful, or false otherwise, setting
 * errno values as specified above.
 */
bool lcd_register_btn1_cb(lcd_btn_cb_fn_t cb);

/**
 * Registers a callback function for the rightmost button.
 *
 * When the rightmost button on the emulated three-button LCD is pressed, the
 * user-provided callback function will be invoked.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO  - The LCD has not been initialized. Call lcd_initialize() first.
 *
 * \param cb
 *        A callback function of type lcd_btn_cb_fn_t (void (*cb)(void))
 *
 * \return True if the operation was successful, or false otherwise, setting
 * errno values as specified above.
 */
bool lcd_register_btn2_cb(lcd_btn_cb_fn_t cb);

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
 */
uint8_t lcd_read_buttons(void);

#ifdef __cplusplus
}  // namespace c
}  // namespace pros
}
#endif
#endif  // _PROS_LLEMU_H_
