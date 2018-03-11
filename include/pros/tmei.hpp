/*
 * \file tmei.hpp
 *
 * C++ bindings for Touch Management Engine/Interface layer 0
 *
 * This file represents a low-level interface for interacting with the built-in
 * LCD touch screen.
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

#include "pros/tmei.h"

namespace pros {
namespace display {
	void set_color_fg(std::uint32_t color);
	void set_color_bg(std::uint32_t color);
	void erase(void);
	void scroll(std::int16_t start_line, std::int16_t lines);
	void scroll_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1, std::int16_t lines);
	void copy_rect(std::int16_t x0,
	               std::int16_t y0,
	               std::int16_t x1,
	               std::int16_t y1,
	               std::uint32_t* buf,
	               std::int32_t stride);
	void draw_pixel(std::int16_t x, std::int16_t y);
	void clear_pixel(std::int16_t x, std::int16_t y);
	void draw_line(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1);
	void clear_line(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1);
	void draw_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1);
	void clear_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1);
	void fill_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1);
	void draw_circle(std::int16_t x, std::int16_t y, std::int16_t r);
	void clear_circle(std::int16_t x, std::int16_t y, std::int16_t r);
	void fill_circle(std::int16_t x, std::int16_t y, std::int16_t r);
	namespace {
		template <typename T> T convert_args(T arg) {
			return arg;
		}
		const char* convert_args(const std::string& arg) {
			return arg.c_str();
		}
	}
	template <typename... Params> void printf(const std::int16_t line, const char* fmt, Params... args) {
		display_printf(line, fmt, convert_args(args)...);
	}
	template <typename... Params> void printf_at(std::int16_t x, std::int16_t y, const char* fmt, Params... args) {
		display_printf_at(x, y, fmt, convert_args(args)...);
	}
	template <typename... Params> void big_printf(const std::int16_t line, const char* fmt, Params... args) {
		display_big_printf(line, fmt, convert_args(args)...);
	}
	template <typename... Params>
	void big_printf_at(std::int16_t x, std::int16_t y, const char* fmt, Params... args) {
		display_printf_at(x, y, fmt, convert_args(args)...);
	}
	template <typename... Params>
	void small_printf_at(std::int16_t x, std::int16_t y, const char* fmt, Params... args) {
		display_small_printf_at(x, y, fmt, convert_args(args)...);
	}
	template <typename... Params> void center_printf(const std::int16_t line, const char* fmt, Params... args) {
		display_center_printf(line, fmt, convert_args(args)...);
	}
	template <typename... Params> void center_big_printf(const std::int16_t line, const char* fmt, Params... args) {
		display_center_big_printf(line, fmt, convert_args(args)...);
	}
	void puts(const std::int16_t line, const std::string text);
	void puts_at(std::int16_t x, std::int16_t y, const std::string text);
	void big_puts(const std::int16_t line, const std::string text);
	void big_puts_at(std::int16_t x, std::int16_t y, const std::string text);
	void small_puts_at(std::int16_t x, std::int16_t y, const std::string text);
	void center_puts(const std::int16_t line, const std::string text);
	void center_big_puts(const std::int16_t line, const std::string text);
}
// enum touch_event_e { E_TOUCH_EVENT_RELEASE, E_TOUCH_EVENT_PRESS, E_TOUCH_EVENT_PRESS_AND_HOLD };
// using touch_event_e_t = enum touch_event_e;
}

namespace pros {
namespace touch {
	using event_e_t = touch_event_e_t;
	using event_cb_fn_t = void (*)(std::int16_t, std::int16_t);

	void register_callback(event_cb_fn_t cb, event_e_t event_type);
	void unregister_callback(event_cb_fn_t cb, event_e_t event_type);
}
}