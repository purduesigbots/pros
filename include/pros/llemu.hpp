/*
 * \file llemu.hpp
 *
 * C++ bindings for Legacy LCD Emulator
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

#pragma once
#include <cstdint>
#include <string>

#include "pros/llemu.h"

namespace pros {
namespace lcd {
	bool is_initialized(void);
	bool initialize(void);
	bool shutdown(void);
	namespace {
		template <typename T> T convert_args(T arg) {
			return arg;
		}
		const char* convert_args(const std::string& arg) {
			return arg.c_str();
		}
	}
	template <typename... Params> bool print(std::int16_t line, const char* fmt, Params... args) {
		return lcd_print(line, fmt, convert_args(args)...);
	}
	bool set_text(std::int16_t line, std::string text);
	bool clear(void);
	bool clear_line(std::int16_t line);

	using lcd_btn_cb_fn_t = void (*)(void);
	void register_btn0_cb(lcd_btn_cb_fn_t cb);
	void register_btn1_cb(lcd_btn_cb_fn_t cb);
	void register_btn2_cb(lcd_btn_cb_fn_t cb);
}
}