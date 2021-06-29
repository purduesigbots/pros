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
namespace screen {

    struct TouchStatus {
        std::int16_t screen_last_x;
        std::int16_t screen_last_y;
        pros::last_touch_e_t screen_touch_status;
    }

    void set_pen(const std::uint32_t color){
        pros::c::screen_set_pen(color);
    }
    
    void set_eraser(const std::uint32_t color){
        pros::c::screen_set_eraser(color);
    }

    std::uint32_t get_pen(){
        return pros::c::screen_get_pen();
    }

    std::uint32_t get_eraser(){
        return pros::c::screen_get_eraser();
    }

    void screen_clear(){
        pros::c::screen_clear();
    }

    void scroll(const std::int16_t start_line, const std::int16_t lines){
        pros::c::screen_scroll(start_line, lines);
    }

    void scroll_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, std::int16_t lines){
        pros::c::screen_scroll_area(x0, y0, x1, y1, lines);
    }

    void copy_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, uint32_t* buf, const std::int32_t stride){
        pros::c::screen_copy_area( x0, y0, x1, y1, buf, stride);
    }

    void draw_pixel(const std::int16_t x, const std::int16_t y){
        pros::c::screen_draw_pixel(x, y);
    }

    void clear_pixel(const std::int16_t x, const std::int16_t y){
        pros::c::screen_clear_pixel(x, y);
    }

    void draw_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        pros::c::screen_draw_line(x0, y0, x1, y1);
    }

    void clear_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        pros::c::screen_clear_line(x0, y0, x1, y1);
    }

    void draw_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        pros::c::screen_draw_rect(x0, y0, x1, y1);
    }

    void clear_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        pros::c::screen_clear_rect(x0, y0, x1, y1);
    }

    void fill_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1){
        pros::c::screen_fill_rect(x0, y0, x1, y1);
    }

    void draw_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        pros::c::screen_draw_circle(x, y, radius);
    }

    void clear_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        pros::c::screen_clear_circle(x, y, radius);
    }

    void fill_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius){
        pros::c::screen_fill_circle(x, y, radius);
    }

    //new one
    struct TouchStatus touch_status_screen() {
        return struct TouchStatus(pros::c::screen_last_x(), pros::c::screen_last_y(), pros::c::screen_touch_status());
    }

    // old functions
    std::int16_t last_x(){
        return pros::c::screen_last_x();
    }

    std::int16_t last_y(){
        return pros::c::screen_last_y();
    }

    pros::last_touch_e_t touch_status(){
        return pros::c::screen_touch_status();
    }

}  // namespace screen
}  // namespace pros
