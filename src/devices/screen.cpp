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

#include "pros/screen.hpp"

namespace pros {
namespace screen {

    std::uint32_t set_pen(const std::uint32_t color){
        return pros::c::screen_set_pen(color);
    }
    
    std::uint32_t set_eraser(const std::uint32_t color){
        return pros::c::screen_set_eraser(color);
    }

    std::uint32_t get_pen(){
        return pros::c::screen_get_pen();
    }

    std::uint32_t get_eraser(){
        return pros::c::screen_get_eraser();
    }

    std::uint32_t erase(){
        return pros::c::screen_erase();
    }

    std::uint32_t scroll(const std::int16_t start_line, const std::int16_t lines){
        return pros::c::screen_scroll(start_line, lines);
    }

    std::uint32_t scroll_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, std::int16_t lines){
        return pros::c::screen_scroll_area(x0, y0, x1, y1, lines);
    }

    std::uint32_t copy_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, uint32_t* buf, const std::int32_t stride){
        return pros::c::screen_copy_area( x0, y0, x1, y1, buf, stride);
    }

    std::uint32_t draw_pixel(const std::int16_t x, const std::int16_t y){
        return pros::c::screen_draw_pixel(x, y);
    }

    std::uint32_t erase_pixel(const std::int16_t x, const std::int16_t y){
        return pros::c::screen_erase_pixel(x, y);
    }

    std::uint32_t draw_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        return pros::c::screen_draw_line(x0, y0, x1, y1);
    }

    std::uint32_t erase_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        return pros::c::screen_erase_line(x0, y0, x1, y1);
    }

    std::uint32_t draw_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        return pros::c::screen_draw_rect(x0, y0, x1, y1);
    }

    std::uint32_t erase_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        return pros::c::screen_erase_rect(x0, y0, x1, y1);
    }

    std::uint32_t fill_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        return pros::c::screen_fill_rect(x0, y0, x1, y1);
    }

    std::uint32_t draw_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        return pros::c::screen_draw_circle(x, y, radius);
    }

    std::uint32_t erase_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        return pros::c::screen_erase_circle(x, y, radius);
    }

    std::uint32_t fill_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        return pros::c::screen_fill_circle(x, y, radius);
    }

    screen_touch_status_s_t touch_status() {
        return pros::c::screen_touch_status();
    }

    std::uint32_t touch_callback(touch_event_cb_fn_t cb, last_touch_e_t event_type){
        return pros::c::screen_touch_callback(cb, event_type);
    }

}  // namespace screen
}  // namespace pros
