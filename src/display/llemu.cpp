/*
 * \file display/llemu.cpp
 *
 * C++ Legacy LCD Emulator
 *
 * This file defines a high-level API for emulating the three-button, UART-based
 * VEX LCD, containing a set of functions that facilitate the use of a software-
 * emulated version of the classic VEX LCD module.
 *
 * Copyright (c) 2017-2020, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "pros/llemu.hpp"

namespace pros {
namespace lcd {
using namespace pros::c;

bool is_initialized(void) {
	return lcd_is_initialized();
}
bool initialize(void) {
	return lcd_initialize();
}
bool shutdown(void) {
	return lcd_shutdown();
}
bool set_text(std::int16_t line, std::string text) {
	return lcd_set_text(line, text.c_str());
}
bool clear(void) {
	return lcd_clear();
}
bool clear_line(std::int16_t line) {
	return lcd_clear_line(line);
}

void register_btn0_cb(lcd_btn_cb_fn_t cb) {
	lcd_register_btn0_cb(cb);
}
void register_btn1_cb(lcd_btn_cb_fn_t cb) {
	lcd_register_btn1_cb(cb);
}
void register_btn2_cb(lcd_btn_cb_fn_t cb) {
	lcd_register_btn2_cb(cb);
}
std::uint8_t read_buttons(void) {
	return lcd_read_buttons();
}
}  // namespace lcd
}  // namespace pros
