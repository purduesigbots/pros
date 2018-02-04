/*
 * \file llemu.h
 *
 * \brief Legacy LCD Emulator
 *
 * This file defines a high-level API for emulating the three-button, UART-based
 * VEX LCD, containing a set of functions that facilitate the use of a software-
 * emulated version of the classic VEX LCD module.
 *
 * \copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_LLEMU_H_
#define _PROS_LLEMU_H_

#include "pros/tmei.h"
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_BTN_LEFT 4
#define LCD_BTN_CENTER 2
#define LCD_BTN_RIGHT 1

/**
 * \brief Initialize the display to be an emulation of the three-button,
 *        UART-based VEX LCD
 * \return `true` if the LCD was successfully initialized, or `false` if it has
 *         already been initialized.
 */
bool lcd_initialize(void);
/**
 * \brief Turn off the Legacy LCD Emulator
 *
 * Calling this function will clear the entire display, and you will not be able
 * to call any further LLEMU functions until another call to `lcd_initialize`.
 *
 * This function uses the following values of `errno` when an error state is
 * reached:
 * `ENXIO` - The LCD has not been initialized. Call `lcd_initialize()` first.
 *
 * \return `true` if the operation was successful, or `false` otherwise, setting
 *         `errno` values as specified above.
 */
bool lcd_shutdown(void);
/**
 * \brief Display a formatted string on the emulated three-button LCD screen
 *
 * This function uses the following values of `errno` when an error state is
 * reached:
 * `ENXIO`  - The LCD has not been initialized. Call `lcd_initialize()` first.
 * `EINVAL` - The line number specified is not in the range [0-7]
 *
 * \param[in] line  The line on which to display the text [0-7]
 * \param[in] fmt   Format string
 * \param[in] ...   Optional list of arguments for the format string
 *
 * \return `true` if the operation was successful, or `false` otherwise, setting
 *         `errno` values as specified above.
 */
bool lcd_set_text(int16_t line, const char* fmt, ...);
/**
 * \brief Clear the emulated three-button LCD screen
 *
 * This function uses the following values of `errno` when an error state is
 * reached:
 * `ENXIO`  - The LCD has not been initialized. Call `lcd_initialize()` first.
 * `EINVAL` - The line number specified is not in the range [0-7]
 *
 * \return `true` if the operation was successful, or `false` otherwise, setting
 *         `errno` values as specified above.
 */
bool lcd_clear(void);
/**
 * \brief Clear a line of the emulated three-button LCD screen
 *
 * This function uses the following values of `errno` when an error state is
 * reached:
 * `ENXIO`  - The LCD has not been initialized. Call `lcd_initialize()` first.
 * `EINVAL` - The line number specified is not in the range [0-7]
 *
 * \param[in] line  The line to clear
 *
 * \return `true` if the operation was successful, or `false` otherwise, setting
 *         `errno` values as specified above.
 */
bool lcd_clear_line(int16_t line);

typedef void (*lcd_btn_cb_fn_t)(void);

void _lcd_cb_handle_press(int16_t x, int16_t y);
void _lcd_cb_handle_release(int16_t x, int16_t y);

/**
 * \brief Register a callback function for the leftmost button
 *
 * When the leftmost button on the emulated three-button LCD is pressed, the
 * user-provided callback function will be invoked
 *
 * \param[in] cb    A callback function of type `lcd_btn_cb_fn_t`
 *                  (`void (*cb)(void)`)
 */
void lcd_register_btn0_cb(lcd_btn_cb_fn_t cb);
/**
 * \brief Register a callback function for the center button
 *
 * When the center button on the emulated three-button LCD is pressed, the
 * user-provided callback function will be invoked
 *
 * \param[in] cb    A callback function of type `lcd_btn_cb_fn_t`
 *                  (`void (*cb)(void)`)
 */
void lcd_register_btn1_cb(lcd_btn_cb_fn_t cb);
/**
 * \brief Register a callback function for the rightmost button
 *
 * When the rightmost button on the emulated three-button LCD is pressed, the
 * user-provided callback function will be invoked
 *
 * \param[in] cb    A callback function of type `lcd_btn_cb_fn_t`
 *                  (`void (*cb)(void)`)
 */
void lcd_register_btn2_cb(lcd_btn_cb_fn_t cb);

/**
 * \brief Reads the button status from the emulated three-button LCD
 *
 * The value returned is a 3-bit integer where `1 0 0` indicates the left button
 * is pressed, `0 1 0` indicates the center button is pressed, and `0 0 1`
 * indicates the right button is pressed. `0` is returned if no buttons are
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
}
#endif

#endif