/*
 * \file llemu.c
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

#include <string.h>

#include "pros/llemu.h"

static bool lcd_is_initialized = false;

static lcd_btn_cb_fn_t _lcd_btn0_cb;
static lcd_btn_cb_fn_t _lcd_btn1_cb;
static lcd_btn_cb_fn_t _lcd_btn2_cb;

static uint8_t _touch_bits;

static const uint32_t _FRAME_COLOR = 0xA18A78;
static const uint32_t _LCD_SCREEN_COLOR = 0x5ABC03;
static const uint32_t _BUTTON_COLOR = 0x1A1917;
static const uint32_t _TEXT_COLOR = 0x323D13;

static const int32_t _SCREEN_X0 = 0;
static const int32_t _SCREEN_Y0 = 0;
static const int32_t _SCREEN_X1 = 480;
static const int32_t _SCREEN_Y1 = 240;
static const int32_t _LCD_SCREEN_X0 = 27;
static const int32_t _LCD_SCREEN_Y0 = 19;
static const int32_t _LCD_SCREEN_X1 = 453;
static const int32_t _LCD_SCREEN_Y1 = 179;
static const int32_t _BUTTON0_X0 = 50;
static const int32_t _BUTTON0_X1 = 130;
static const int32_t _BUTTON1_X0 = 200;
static const int32_t _BUTTON1_X1 = 280;
static const int32_t _BUTTON2_X0 = 350;
static const int32_t _BUTTON2_X1 = 430;
static const int32_t _BUTTON_Y0 = 190;
static const int32_t _BUTTON_Y1 = 230;

bool lcd_initialize(void) {
	if (lcd_is_initialized) return false;
	display_set_color_bg(_FRAME_COLOR);  // LCD frame
	display_clear_rect(_SCREEN_X0, _SCREEN_Y0, _SCREEN_X1, _SCREEN_Y1);
	display_set_color_bg(_LCD_SCREEN_COLOR);  // LCD screen
	display_clear_rect(_LCD_SCREEN_X0, _LCD_SCREEN_Y0, _LCD_SCREEN_X1, _LCD_SCREEN_Y1);
	display_set_color_bg(_BUTTON_COLOR);  // LCD button
	display_clear_rect(_BUTTON0_X0, _BUTTON_Y0, _BUTTON0_X1, _BUTTON_Y1);
	display_clear_rect(_BUTTON1_X0, _BUTTON_Y0, _BUTTON1_X1, _BUTTON_Y1);
	display_clear_rect(_BUTTON2_X0, _BUTTON_Y0, _BUTTON2_X1, _BUTTON_Y1);

	register_touch_callback(_lcd_cb_handle_press, E_TOUCH_EVENT_PRESS);
	register_touch_callback(_lcd_cb_handle_release, E_TOUCH_EVENT_RELEASE);
	_touch_bits = 0;

	lcd_is_initialized = true;
	return true;
}

bool lcd_shutdown(void) {
	if (!lcd_is_initialized) {
		errno = ENXIO;
		return false;
	}
	display_erase();
	lcd_is_initialized = false;
	return true;
}

bool lcd_set_text(int16_t line, const char* fmt, ...) {
	if (!lcd_is_initialized) {
		errno = ENXIO;
		return false;
	}
	line++;                      // make lines zero-indexed in terms of the emulated LCD screen
	if (line < 0 || line > 7) {  // make sure that the supplied line is displayable
		errno = EINVAL;
		return false;
	}
	display_set_color_bg(_LCD_SCREEN_COLOR);
	display_set_color_fg(_TEXT_COLOR);
	char s[47];                // max number of displayable characters is 44
	s[0] = s[1] = s[2] = ' ';  // first three characters are alignment spaces
	strcpy(s + 3, fmt);
	va_list args;
	va_start(args, fmt);
	display_vprintf(line, s, args);
	va_end(args);

	display_set_color_bg(_FRAME_COLOR);  // cover overflowing text
	display_clear_rect(_SCREEN_X0, _SCREEN_Y0, _LCD_SCREEN_X0, _SCREEN_Y1);
	display_clear_rect(_LCD_SCREEN_X1, _SCREEN_Y0, _SCREEN_X1, _SCREEN_Y1);
	return true;
}

bool lcd_clear(void) {
	if (!lcd_is_initialized) {
		errno = ENXIO;
		return false;
	}
	display_set_color_bg(_LCD_SCREEN_COLOR);
	display_clear_rect(_LCD_SCREEN_X0, _LCD_SCREEN_Y0, _LCD_SCREEN_X1, _LCD_SCREEN_Y1);
	return true;
}

bool lcd_clear_line(int16_t line) {
	if (!lcd_is_initialized) {
		errno = ENXIO;
		return false;
	}
	line++;
	if (line < 0 || line > 7) {
		errno = EINVAL;
		return false;
	}
	display_set_color_bg(_LCD_SCREEN_COLOR);
	int32_t line_start_y = _LCD_SCREEN_Y0 + 20 * (line - 1);
	// NOTE: for some reason, using _LCD_SCREEN_X0 and _LCD_SCREEN_X1 makes the
	//       rect bleed over the edges of the screen by one pixel
	display_clear_rect(_LCD_SCREEN_X0 + 1, line_start_y, _LCD_SCREEN_X1 - 1, line_start_y + 20);
	return true;
}

void _lcd_cb_handle_press(int16_t x, int16_t y) {
	if (_BUTTON_Y0 <= y && y <= _BUTTON_Y1) {
		if ((_BUTTON0_X0 <= x && x <= _BUTTON0_X1)) {
			_touch_bits |= (1 << 2);
			if (_lcd_btn0_cb) _lcd_btn0_cb();
		} else if ((_BUTTON1_X0 <= x && x <= _BUTTON1_X1)) {
			_touch_bits |= (1 << 1);
			if (_lcd_btn1_cb) _lcd_btn1_cb();
		} else if ((_BUTTON2_X0 <= x && x <= _BUTTON2_X1)) {
			_touch_bits |= (1 << 0);
			if (_lcd_btn2_cb) _lcd_btn2_cb();
		}
	}
}
void _lcd_cb_handle_release(int16_t x, int16_t y) {
	if (_BUTTON_Y0 <= y && y <= _BUTTON_Y1) {
		if ((_BUTTON0_X0 <= x && x <= _BUTTON0_X1))
			_touch_bits &= ~(1 << 2);
		else if ((_BUTTON1_X0 <= x && x <= _BUTTON1_X1))
			_touch_bits &= ~(1 << 1);
		else if ((_BUTTON2_X0 <= x && x <= _BUTTON2_X1))
			_touch_bits &= ~(1 << 0);
	}
}

void lcd_register_btn0_cb(lcd_btn_cb_fn_t cb) {
	_lcd_btn0_cb = cb;
}
void lcd_register_btn1_cb(lcd_btn_cb_fn_t cb) {
	_lcd_btn1_cb = cb;
}
void lcd_register_btn2_cb(lcd_btn_cb_fn_t cb) {
	_lcd_btn2_cb = cb;
}

uint8_t lcd_read_buttons(void) {
	return _touch_bits;
}