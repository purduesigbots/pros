/**
 * \file screen.cpp
 *
 * \brief Brain screen display and touch functions.
 *
 * Contains user calls to the v5 screen for touching and displaying graphics.
 *
 * \copyright (c) 2017-2021, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "display/graphics.h"
#include "pros/screen.hpp"

namespace pros {

    void Screen::set_pen(const std::uint32_t color){
        pros::c::screen_set_pen(color);
    }
    
    void Screen::set_eraser(const std::uint32_t color){
        pros::c::screen_set_eraser(color);
    }

    std::uint32_t Screen::get_pen(void){
        return pros::c::screen_get_pen();
    }

    std::uint32_t Screen::get_eraser(void){
        return pros::c::screen_get_eraser();
    }

    void Screen::screen_clear(void){
        pros::c::screen_clear();
    }

    void Screen::scroll(const std::int16_t start_line, const std::int16_t lines){
        pros::c::screen_scroll(start_line, lines);
    }

    void Screen::scroll_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, std::int16_t lines){
        pros::c::screen_scroll_area(x0, y0, x1, y1, lines);
    }

    void Screen::copy_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, uint32_t* buf, const std::int32_t stride){
        pros::c::screen_copy_area( x0, y0, x1, y1, buf, stride);
    }

    void Screen::draw_pixel(const std::int16_t x, const std::int16_t y){
        pros::c::screen_draw_pixel(x, y);
    }

    void Screen::clear_pixel(const std::int16_t x, const std::int16_t y){
        pros::c::screen_clear_pixel(x, y);
    }

    void Screen::draw_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        pros::c::screen_draw_line(x0, y0, x1, y1);
    }

    void Screen::clear_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        pros::c::screen_clear_line(x0, y0, x1, y1);
    }

    void Screen::draw_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        pros::c::screen_draw_rect(x0, y0, x1, y1);
    }

    void Screen::clear_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        pros::c::screen_clear_rect(x0, y0, x1, y1);
    }

    void Screen::fill_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        pros::c::screen_fill_rect(x0, y0, x1, y1);
    }

    void Screen::draw_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        pros::c::screen_draw_circle(x, y, radius);
    }

    void Screen::clear_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        pros::c::screen_clear_circle(x, y, radius);
    }

    void Screen::fill_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        pros::c::screen_fill_circle(x, y, radius);
    }

    template <typename... Params>
    void Screen::print(pros::c::text_format_e_t txt_fmt, const std::int16_t line, const char* text, Params... args){
	    display_print(txt_fmt, line, text, convert_args(args)...);
    }

    template <typename... Params>
    void Screen::print(pros::c::text_format_e_t txt_fmt, const std::int16_t x, const std::int16_t y, const char* text, Params... args){
	    display_print_at(txt_fmt, x, y, text, convert_args(args)...);
    }

    template <typename... Params>
    void Screen::print(pros::c::text_format_e_t txt_fmt, const std::int16_t x, const std::int16_t y, uint32_t opacity, const char *text, Params... args){
        display_print_opacity_at(x, y, text, convert_args(args)...);
    }

    std::int16_t Screen::last_x(void){
        return pros::c::screen_last_x();
    }

    std::int16_t Screen::last_y(void){
        return pros::c::screen_last_y();
    }

    pros::c::last_touch_e_t Screen::touch_status(void){
        return pros::c::screen_touch_status();
    }

}  // namespace pros
