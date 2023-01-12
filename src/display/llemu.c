/*
 * \file display/llemu.c
 *
 * Legacy LCD Emulator
 *
 * This file defines a high-level API for emulating the three-button, UART-based
 * VEX LCD, containing a set of functions that facilitate the use of a software-
 * emulated version of the classic VEX LCD module.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
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

static lv_style_t frame_style;
static lv_style_t screen_style;
static lv_style_t button_style;
static lv_style_t button_pressed_style;

static lv_res_t __touch_bits_update_pressed(lv_obj_t* btn) {
	lcd_s_t* lcd = lv_obj_get_ext_attr(lv_obj_get_parent(lv_obj_get_parent(lv_obj_get_parent(btn))));

	if (btn == lcd->btns[0])
		lcd->touch_bits |= (1 << 2);
	else if (btn == lcd->btns[1])
		lcd->touch_bits |= (1 << 1);
	else if (btn == lcd->btns[2])
		lcd->touch_bits |= (1 << 0);

	return LV_RES_OK;
}

void __touch_bits_update_released(lcd_s_t* lcd, size_t btn) {
	lcd->touch_bits &= ~(1 << btn);
}

static lv_res_t __wrap_cb(lv_obj_t* btn) {
	lcd_s_t* lcd = lv_obj_get_ext_attr(lv_obj_get_parent(lv_obj_get_parent(lv_obj_get_parent(btn))));
	if (btn == lcd->btns[0]) {
		if (lcd->callbacks[0]) (lcd->callbacks[0])();
		__touch_bits_update_released(lcd, 2);
	} else if (btn == lcd->btns[1]) {
		if (lcd->callbacks[1]) (lcd->callbacks[1])();
		__touch_bits_update_released(lcd, 1);
	} else if (btn == lcd->btns[2]) {
		if (lcd->callbacks[2]) (lcd->callbacks[2])();
		__touch_bits_update_released(lcd, 0);
	}

	return LV_RES_OK;
}

static lv_obj_t* _create_lcd(void) {
	lv_style_copy(&frame_style, &lv_style_pretty_color);
	frame_style.body.main_color = LV_COLOR_GRAY;
	frame_style.body.grad_color = LV_COLOR_SILVER;

	lv_style_copy(&screen_style, &lv_style_plain);
	screen_style.body.main_color = LV_COLOR_MAKE(0x5A, 0xBC, 0x03);
	screen_style.body.grad_color = LV_COLOR_MAKE(0x5A, 0xBC, 0x03);
	screen_style.text.color = LV_COLOR_MAKE(0x32, 0x3D, 0x13);
	screen_style.text.font = &pros_font_dejavu_mono_20;

	lv_style_copy(&button_style, &lv_style_plain);
	button_style.body.main_color = LV_COLOR_GRAY;
	button_style.body.grad_color = LV_COLOR_MAKE(0x30, 0x30, 0x30);

	lv_style_copy(&button_pressed_style, &lv_style_plain);
	button_pressed_style.body.main_color = LV_COLOR_MAKE(0x0A, 0x0A, 0x0A);
	button_pressed_style.body.grad_color = LV_COLOR_MAKE(0x80, 0x80, 0x80);

	lv_obj_t* lcd_dummy = lv_obj_create(lv_scr_act(), NULL);
	lv_obj_set_size(lcd_dummy, LCD_WIDTH, LCD_HEIGHT);

	lv_obj_t* frame = lv_cont_create(lcd_dummy, NULL);
	lv_obj_set_size(frame, LCD_WIDTH, LCD_HEIGHT);
	lv_obj_set_style(frame, &frame_style);

	lv_obj_t* screen = lv_cont_create(frame, NULL);
	lv_obj_set_size(screen, 426, 160);
	lv_obj_align(screen, frame, LV_ALIGN_IN_TOP_MID, 0, 19);
	lv_obj_set_style(screen, &screen_style);

	lv_obj_t* btn_container = lv_cont_create(frame, NULL);
	lv_obj_set_size(btn_container, 426, 30);
	lv_obj_align(btn_container, frame, LV_ALIGN_IN_BOTTOM_MID, 0, -20);
	lv_cont_set_style(btn_container, &lv_style_transp_fit);

	lv_obj_t* btn_left = lv_btn_create(btn_container, NULL);
	lv_obj_set_width(btn_left, 80);
	lv_obj_align(btn_left, btn_container, LV_ALIGN_IN_LEFT_MID, 0, 0);
	lv_btn_set_style(btn_left, LV_BTN_STYLE_REL, &button_style);
	lv_btn_set_style(btn_left, LV_BTN_STYLE_PR, &button_pressed_style);
	lv_btn_set_action(btn_left, LV_BTN_ACTION_PR, __touch_bits_update_pressed);
	lv_btn_set_action(btn_left, LV_BTN_ACTION_CLICK, __wrap_cb);

	lv_obj_t* btn_center = lv_btn_create(btn_container, NULL);
	lv_obj_set_width(btn_center, 80);
	lv_obj_align(btn_center, btn_container, LV_ALIGN_CENTER, 0, 0);
	lv_btn_set_style(btn_center, LV_BTN_STYLE_REL, &button_style);
	lv_btn_set_style(btn_center, LV_BTN_STYLE_PR, &button_pressed_style);
	lv_btn_set_action(btn_center, LV_BTN_ACTION_PR, __touch_bits_update_pressed);
	lv_btn_set_action(btn_center, LV_BTN_ACTION_CLICK, __wrap_cb);

	lv_obj_t* btn_right = lv_btn_create(btn_container, NULL);
	lv_obj_set_width(btn_right, 80);
	lv_obj_align(btn_right, btn_container, LV_ALIGN_IN_RIGHT_MID, 0, 0);
	lv_btn_set_style(btn_right, LV_BTN_STYLE_REL, &button_style);
	lv_btn_set_style(btn_right, LV_BTN_STYLE_PR, &button_pressed_style);
	lv_btn_set_action(btn_right, LV_BTN_ACTION_PR, __touch_bits_update_pressed);
	lv_btn_set_action(btn_right, LV_BTN_ACTION_CLICK, __wrap_cb);

	lcd_s_t* lcd = lv_obj_allocate_ext_attr(lcd_dummy, sizeof(lcd_s_t));
	lcd->frame = frame;
	lcd->screen = screen;

	lcd->btn_container = btn_container;
	lcd->btns[0] = btn_left;
	lcd->btns[1] = btn_center;
	lcd->btns[2] = btn_right;
	lcd->callbacks[0] = NULL;
	lcd->callbacks[1] = NULL;
	lcd->callbacks[2] = NULL;

	for (size_t i = 0; i < LLEMU_LINES; i++) {
		lcd->lcd_text[i] = lv_label_create(lcd->screen, NULL);
		lv_obj_set_width(lcd->lcd_text[i], 426);
		lv_obj_align(lcd->lcd_text[i], NULL, LV_ALIGN_IN_TOP_LEFT, 5, 20 * i);
		lv_label_set_align(lcd->lcd_text[i], LV_LABEL_ALIGN_LEFT);
		lv_label_set_long_mode(lcd->lcd_text[i], LV_LABEL_LONG_CROP);
		// lv_label_set_no_break(lcd->lcd_text[i], true);
		lv_label_set_text(lcd->lcd_text[i], "");
	}

	return lcd_dummy;
}

bool _lcd_vprint(lv_obj_t* lcd_dummy, int16_t line, const char* fmt, va_list args) {
	if (line < 0 || line > (LLEMU_LINES - 1)) {
		errno = EINVAL;
		return false;
	}
	lcd_s_t* lcd = lv_obj_get_ext_attr(lcd_dummy);
	char buf[33];
	vsnprintf(buf, 33, fmt, args);

	lv_label_set_text(lcd->lcd_text[line], buf);
	lv_obj_set_width(lcd->lcd_text[line], 426);
	return true;
}

bool _lcd_print(lv_obj_t* lcd_dummy, int16_t line, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	bool res = _lcd_vprint(lcd_dummy, line, fmt, args);
	va_end(args);
	return res;
}

bool _lcd_set_text(lv_obj_t* lcd_dummy, int16_t line, const char* text) {
	return _lcd_print(lcd_dummy, line, "%s", text);
}

bool _lcd_clear_line(lv_obj_t* lcd_dummy, int16_t line) {
	if (line < 0 || line > (LLEMU_LINES - 1)) {
		errno = EINVAL;
		return false;
	}
	lcd_s_t* lcd = lv_obj_get_ext_attr(lcd_dummy);
	lv_label_set_text(lcd->lcd_text[line], "");
	return true;
}

void _lcd_clear(lv_obj_t* lcd_dummy) {
	for (size_t i = 0; i < LLEMU_LINES; i++) {
		_lcd_clear_line(lcd_dummy, i);
	}
}

void _lcd_set_left_callback(lv_obj_t* lcd_dummy, lcd_btn_cb_fn_t cb) {
	lcd_s_t* lcd = lv_obj_get_ext_attr(lcd_dummy);
	lcd->callbacks[0] = cb;
}
void _lcd_set_center_callback(lv_obj_t* lcd_dummy, lcd_btn_cb_fn_t cb) {
	lcd_s_t* lcd = lv_obj_get_ext_attr(lcd_dummy);
	lcd->callbacks[1] = cb;
	lv_btn_set_action(lcd->btns[1], LV_BTN_ACTION_CLICK, __wrap_cb);
}
void _lcd_set_right_callback(lv_obj_t* lcd_dummy, lcd_btn_cb_fn_t cb) {
	lcd_s_t* lcd = lv_obj_get_ext_attr(lcd_dummy);
	lcd->callbacks[2] = cb;
	lv_btn_set_action(lcd->btns[2], LV_BTN_ACTION_CLICK, __wrap_cb);
}

uint8_t _lcd_read_buttons(lv_obj_t* lcd_dummy) {
	lcd_s_t* lcd = lv_obj_get_ext_attr(lcd_dummy);
	return lcd->touch_bits;
}

static volatile bool _lcd_is_initialized = false;
bool lcd_is_initialized(void) {
	return _lcd_is_initialized;
}

static lv_obj_t* _llemu_lcd;
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
	lv_obj_del(_llemu_lcd);
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

void lcd_set_background_color(lv_color_t color) {
	screen_style.body.main_color = color;
	screen_style.body.grad_color = color;
}

void lcd_set_text_color(lv_color_t color) {
	screen_style.text.color = color;
}
