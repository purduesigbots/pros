/*
 * \file tmei.c
 *
 * \brief Touch Management Engine/Interface layer 0
 *
 * This file represents a low-level interface for interacting with the built-in
 * LCD touch screen.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/tmei.hpp"

namespace pros {
namespace display {
	void set_color_fg(std::uint32_t color) {
		display_set_color_fg(color);
	}
	void set_color_bg(std::uint32_t color) {
		display_set_color_bg(color);
	}
	void erase(void) {
		display_erase();
	}
	void scroll(std::int16_t start_line, std::int16_t lines) {
		display_scroll(start_line, lines);
	}
	void scroll_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1, std::int16_t lines) {
		display_scroll_rect(x0, y0, x1, y1, lines);
	}
	void copy_rect(std::int16_t x0,
	               std::int16_t y0,
	               std::int16_t x1,
	               std::int16_t y1,
	               std::uint32_t* buf,
	               std::int32_t stride) {
		display_copy_rect(x0, y0, x1, y1, buf, stride);
	}
	void draw_pixel(std::int16_t x, std::int16_t y) {
		display_draw_pixel(x, y);
	}
	void clear_pixel(std::int16_t x, std::int16_t y) {
		display_clear_pixel(x, y);
	}
	void draw_line(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1) {
		display_draw_line(x0, y0, x1, y1);
	}
	void clear_line(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1) {
		display_clear_line(x0, y0, x1, y1);
	}
	void draw_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1) {
		display_draw_rect(x0, y0, x1, y1);
	}
	void clear_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1) {
		display_clear_rect(x0, y0, x1, y1);
	}
	void fill_rect(std::int16_t x0, std::int16_t y0, std::int16_t x1, std::int16_t y1) {
		display_fill_rect(x0, y0, x1, y1);
	}
	void draw_circle(std::int16_t x, std::int16_t y, std::int16_t r) {
		display_draw_circle(x, y, r);
	}
	void clear_circle(std::int16_t x, std::int16_t y, std::int16_t r) {
		display_clear_circle(x, y, r);
	}
	void fill_circle(std::int16_t x, std::int16_t y, std::int16_t r) {
		display_fill_circle(x, y, r);
	}
	void puts(const std::int16_t line, const std::string text) {
		display_puts(line, text.c_str());
	}
	void puts_at(std::int16_t x, std::int16_t y, const std::string text) {
		display_puts_at(x, y, text.c_str());
	}
	void big_puts(const std::int16_t line, const std::string text) {
		display_big_puts(line, text.c_str());
	}
	void big_puts_at(std::int16_t x, std::int16_t y, const std::string text) {
		display_big_puts_at(x, y, text.c_str());
	}
	void small_puts_at(std::int16_t x, std::int16_t y, const std::string text) {
		display_small_puts_at(x, y, text.c_str());
	}
	void center_puts(const std::int16_t line, const std::string text) {
		display_center_puts(line, text.c_str());
	}
	void center_big_puts(const std::int16_t line, const std::string text) {
		display_center_big_puts(line, text.c_str());
	}
}
}

namespace pros {
namespace touch {
	void register_callback(event_cb_fn_t cb, event_e_t event_type) {
		register_touch_callback(cb, event_type);
	}
	void unregister_callback(event_cb_fn_t cb, event_e_t event_type) {
		unregister_touch_callback(cb, event_type);
	}
}
}
