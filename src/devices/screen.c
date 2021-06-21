/**
 * \file screen.c
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
#include "pros/screen.h"

#include "pros/apix.h"
#include "v5_api.h"  // vexDisplay* 

/******************************************************************************/
/**                  Screen Graphical Display Functions                      **/
/**                                                                          **/
/**   These functions allow programmers to display shapes on the v5 screen   **/
/******************************************************************************/

void screen_set_pen(uint32_t color){
    display_set_color_fg(color);
}

void screen_set_eraser(uint32_t color){
    display_set_color_bg(color);
}

uint32_t screen_get_pen(void){
    return display_get_color_fg();
}

uint32_t screen_get_eraser(void){
    return display_get_color_bg();
}

void screen_clear(void){
    display_erase();
}

void screen_scroll(int16_t start_line, int16_t lines){
    display_scroll(start_line,lines);
}

void screen_scroll_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t lines){
    display_scroll_rect(x0,y0,x1,y1,lines);
}

void screen_copy_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t* buf, int32_t stride){
    display_copy_rect(x0, y0, x1, y1, buf, stride);
}

void screen_draw_pixel(int16_t x, int16_t y){
    display_set_pixel_fg(x,y);
}

void screen_clear_pixel(int16_t x, int16_t y){
    display_set_pixel_bg(x,y);
}

void screen_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
    display_draw_line( x0, y0, x1, y1);
}

void screen_clear_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
    display_clear_line( x0, y0, x1, y1);
}

void screen_draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
    display_draw_rect( x0, y0, x1, y1);
}

void screen_clear_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
    display_clear_rect(x0, y0, x1, y1);
}

void screen_fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
    display_fill_rect(x0, y0, x1, y1);
}

void screen_draw_circle(int16_t x, int16_t y, int16_t radius){
    display_draw_circle(x,y,radius);
}

void screen_clear_circle(int16_t x, int16_t y, int16_t radius){
    display_clear_circle(x,y,radius);
}

void screen_fill_circle(int16_t x, int16_t y, int16_t radius){
    display_fill_circle(x,y,radius);
}

/******************************************************************************/
/**                       Screen Text Display Functions                      **/
/**                                                                          **/
/**     These functions allow programmers to display text on the v5 screen   **/
/******************************************************************************/

void screen_print_opacity( int32_t x, int32_t y, uint32_t opacity, const char *text, ...){
    va_list args;
	va_start(args, text);
    display_opacity_vprintf(x, y, opacity, text, args);
    va_end(args);
}

void screen_print(text_format_e_t txt_fmt, const int16_t line, const char* text, ...){
    va_list args;
	va_start(args, text);
    display_vprintf((uint8_t)txt_fmt, line, text, args);
    va_end(args);
}

void screen_print_at(text_format_e_t txt_fmt, int16_t x, int16_t y, const char* text, ...){
    va_list args;
	va_start(args, text);
    display_vprintf_at((uint8_t)txt_fmt, x, y, text, args);
    va_end(args);
}

void screen_vprintf(text_format_e_t txt_fmt, const int16_t line, const char* text, va_list args){
    display_vprintf((uint8_t)txt_fmt, line, text, args);
}

void screen_vprintf_at(text_format_e_t txt_fmt, const int16_t x, const int16_t y, const char* text, va_list args){
    display_vprintf_at((uint8_t)txt_fmt, x, y, text, args);
}
/******************************************************************************/
/**                         Screen Touch Functions                           **/
/**                                                                          **/
/**               These functions allow programmers to access                **/
/**                    information about screen touches                      **/
/******************************************************************************/

int16_t screen_last_x(void){
    return touch_last_x();
}

int16_t screen_last_y(void){
    return touch_last_y();
}

last_touch_e_t screen_touch_status(void){
    switch (touch_last_event()){
        case E_TOUCH_EVENT_PRESS:
            return TOUCH_PRESSED;
        case E_TOUCH_EVENT_PRESS_AND_HOLD:
            return TOUCH_HELD;
        case E_TOUCH_EVENT_RELEASE:
        default:
            return TOUCH_RELEASED;
    }
}