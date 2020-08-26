/**
 * \file screen.cpp
 *
 * \brief Brain screen display and touch functions.
 *
 * Contains user calls to the v5 screen for touching and displaying graphics.
 *
 * \copyright (c) 2017, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/screen.hpp"
#include "tmei/graphics.h"
#include <cstdint>

namespace pros {
using namespace pros::c;

    Screen::Screen(void){
        this->pen_color = screen_get_pen();
        this->eraser_color = screen_get_eraser();
    }

    Screen::Screen(const std::uint32_t pen_color){
        this->pen_color = pen_color;
        this->eraser_color = screen_get_eraser();
    }

    Screen::Screen(const std::uint32_t pen_color, const std::uint32_t eraser_color){
        this->pen_color = pen_color;
        this->eraser_color = eraser_color;
    }

    void Screen::set_pen(const std::uint32_t color){
        this->pen_color = color;
    }
    
    void Screen::set_eraser(const std::uint32_t color){
        this->eraser_color = color;
    }

    std::uint32_t Screen::get_pen(void){
        return this->pen_color;
    }

    std::uint32_t Screen::get_eraser(void){
        return this->eraser_color;
    }

    void Screen::screen_clear(void){
        screen_clear();
    }

    void Screen::scroll(const std::int16_t start_line, const std::int16_t lines){
        screen_scroll(start_line, lines);
    }

    void Screen::scroll_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, std::int16_t lines){
        screen_scroll_area(x0, y0, x1, y1, lines);
    }

    void Screen::copy_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, uint32_t* buf, const std::int32_t stride){
        screen_copy_area( x0, y0, x1, y1, buf, stride);
    }

    void Screen::draw_pixel(const std::int16_t x, const std::int16_t y){
        screen_set_pen(this->pen_color);
        screen_draw_pixel(x, y);
    }

    void Screen::clear_pixel(const std::int16_t x, const std::int16_t y){
        screen_set_eraser(this->eraser_color);
        screen_clear_pixel(x, y);
    }

    void Screen::draw_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        screen_set_pen(this->pen_color);
        screen_draw_line(x0, y0, x1, y1);
    }

    void Screen::clear_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        screen_set_eraser(this->eraser_color);
        screen_clear_line(x0, y0, x1, y1);
    }

    void Screen::draw_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        screen_set_pen(this->pen_color);
        screen_draw_rect(x0, y0, x1, y1);
    }

    void Screen::clear_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        screen_set_eraser(this->eraser_color);
        screen_clear_rect(x0, y0, x1, y1);
    }

    void Screen::fill_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        screen_set_pen(this->pen_color);
        screen_fill_rect(x0, y0, x1, y1);
    }

    void Screen::draw_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        screen_set_pen(this->pen_color);
        screen_draw_circle(x, y, radius);
    }

    void Screen::clear_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        screen_set_eraser(this->eraser_color);
        screen_clear_circle(x, y, radius);
    }

    void Screen::fill_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        screen_set_pen(this->pen_color);
        screen_fill_circle(x, y, radius);
    }

    void Screen::print(text_format_e_t txt_fmt, const std::int16_t line, const char* text){
        screen_set_pen(this->pen_color);
        screen_set_eraser(this->eraser_color);
        screen_print(txt_fmt, line, text);
    }

    void Screen::print(text_format_e_t txt_fmt, const std::int16_t x, const std::int16_t y, const char* text){
        screen_set_pen(this->pen_color);
        screen_set_eraser(this->eraser_color);
        screen_print_at(txt_fmt, x, y, text);
    }

    void Screen::print_at(text_format_e_t txt_fmt, const std::int16_t x, const std::int16_t y, const char* text){
        screen_set_pen(this->pen_color);
        screen_set_eraser(this->eraser_color);
        screen_print_at(txt_fmt, x, y, text);
    }

    void Screen::printf(text_format_e_t txt_fmt, const std::int16_t line, const char* text, ...){
	    va_list args;
	    va_start(args, text);
        screen_set_pen(this->pen_color);
        screen_set_eraser(this->eraser_color);
	    screen_vprintf(txt_fmt, line, text, args);
    }

    void Screen::printf(text_format_e_t txt_fmt, const std::int16_t x, const std::int16_t y, const char* text, ...){
	    va_list args;
	    va_start(args, text);
        screen_set_pen(this->pen_color);
        screen_set_eraser(this->eraser_color);
	    screen_vprintf_at(txt_fmt, x, y, text, args);
    }

    void Screen::printf_at(text_format_e_t txt_fmt, const std::int16_t x, const std::int16_t y, const char* text, ...){
	    va_list args;
	    va_start(args, text);
	    screen_set_pen(this->pen_color);
        screen_set_eraser(this->eraser_color);
        screen_vprintf_at(txt_fmt, x, y, text, args);
    }

    std::int16_t Screen::last_x(void){
        return screen_last_x();
    }

    std::int16_t Screen::last_y(void){
        return screen_last_y();
    }

    last_touch_e_t Screen::touch_status(void){
        return screen_touch_status();
    }

}  // namespace pros