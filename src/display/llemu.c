/*
 * \file display/llemu.c
 *
 * Legacy LCD Emulator
 *
 * This file defines a high-level API for emulating the three-button, UART-based
 * VEX LCD, containing a set of functions that facilitate the use of a software-
 * emulated version of the classic VEX LCD module.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define _GNU_SOURCE
// NOTE: this would normally be in the C file, but it won't compile that way
#include <stdio.h>  // vasprintf (GNU extension)
#undef _GNU_SOURCE
#include <stdarg.h>
#include <string.h>
#include "kapi.h"
#include "pros/llemu.h"

#define LCD_WIDTH	480 
#define LCD_HEIGHT 	240
#define LLEMU_LINES 8

void __touch_bits_update_released(lcd_s_t* lcd, size_t btn) {
	lcd->touch_bits &= ~(1 << btn);
}

bool _lcd_vprint(int16_t line, const char* fmt, va_list args) {
	if (line < 0 || line > (LLEMU_LINES - 1)) {
		errno = EINVAL;
		return false;
	}
	char buf[33];
	vsnprintf(buf, 33, fmt, args);

	return true;
}

bool _lcd_print(int16_t line, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	bool res = _lcd_vprint(line, fmt, args);
	va_end(args);
	return res;
}

bool _lcd_set_text(int16_t line, const char* text) {
	return _lcd_print(line, "%s", text);
}

bool _lcd_clear_line(int16_t line) {
	if (line < 0 || line > (LLEMU_LINES - 1)) {
		errno = EINVAL;
		return false;
	}
	return true;
}

void _lcd_clear() {
	for (size_t i = 0; i < LLEMU_LINES; i++) {
		_lcd_clear_line(i);
	}
}

void _lcd_set_left_callback(lcd_btn_cb_fn_t cb) {
	lcd->callbacks[0] = cb;
}
void _lcd_set_center_callback(lcd_btn_cb_fn_t cb) {
	lcd->callbacks[1] = cb;
}
void _lcd_set_right_callback(lcd_btn_cb_fn_t cb) {
	lcd->callbacks[2] = cb;
}

uint8_t _lcd_read_buttons() {
	return lcd->touch_bits;
}

static volatile bool _lcd_is_initialized = false;
bool lcd_is_initialized(void) {
	return _lcd_is_initialized;
}

bool lcd_initialize(void) {
	if (lcd_is_initialized()) return false;

	_llemu_lcd = _create_lcd();
	if (!_llemu_lcd) return false;

	_lcd_is_initialized = true;
	return true;
}

bool lcd_shutdown(void) {
	if (!lcd_is_initialized()) {
		errno = ENXIO;
		return false;
	}
	_lcd_is_initialized = false;
	return true;
}

bool lcd_print(int16_t line, const char* fmt, ...) {
	if (!lcd_is_initialized()) {
		errno = ENXIO;
		return false;
	}
	va_list args;
	va_start(args, fmt);
	bool res = _lcd_vprint(_llemu_lcd, line, fmt, args);
	va_end(args);
	return res;
}

bool lcd_set_text(int16_t line, const char* text) {
	if (!lcd_is_initialized()) {
		errno = ENXIO;
		return false;
	}
	return _lcd_set_text(_llemu_lcd, line, text);
}

bool lcd_clear(void) {
	if (!lcd_is_initialized()) {
		errno = ENXIO;
		return false;
	}
	_lcd_clear(_llemu_lcd);
	return true;
}

bool lcd_clear_line(int16_t line) {
	if (!lcd_is_initialized()) {
		errno = ENXIO;
		return false;
	}
	return _lcd_clear_line(_llemu_lcd, line);
}

bool lcd_register_btn0_cb(lcd_btn_cb_fn_t cb) {
	if (!lcd_is_initialized()) {
		errno = ENXIO;
		return false;
	}
	_lcd_set_left_callback(_llemu_lcd, cb);
	return true;
}

bool lcd_register_btn1_cb(lcd_btn_cb_fn_t cb) {
	if (!lcd_is_initialized()) {
		errno = ENXIO;
		return false;
	}
	_lcd_set_center_callback(_llemu_lcd, cb);
	return true;
}

bool lcd_register_btn2_cb(lcd_btn_cb_fn_t cb) {
	if (!lcd_is_initialized()) {
		errno = ENXIO;
		return false;
	}
	_lcd_set_right_callback(_llemu_lcd, cb);
	return true;
}

uint8_t lcd_read_buttons(void) {
	if (!lcd_is_initialized()) {
		errno = ENXIO;
		return 0;
	}
	return _lcd_read_buttons(_llemu_lcd);
}

