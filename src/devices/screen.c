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

#include "pros/screen.h"

#include <stdint.h>
#include <stdlib.h>
#include <sys/_stdint.h>

#include "pros/apix.h"
#include "v5_api.h"  // vexDisplay* 

/******************************************************************************/
/**                  Screen Graphical Display Functions                      **/
/**                                                                          **/
/**   These functions allow programmers to display shapes on the v5 screen   **/
/******************************************************************************/

static mutex_t _display_mutex = NULL;

void screen_set_pen(uint32_t color){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayForegroundColor(color);
	mutex_give(_display_mutex);
}

void screen_set_eraser(uint32_t color){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayBackgroundColor(color);
	mutex_give(_display_mutex);
}

uint32_t screen_get_pen(void){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	uint32_t color = vexDisplayForegroundColorGet();
	mutex_give(_display_mutex);
	return color;
}

uint32_t screen_get_eraser(void){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	uint32_t color = vexDisplayBackgroundColorGet();
	mutex_give(_display_mutex);
	return color;
}

void screen_clear(void){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayErase();
	mutex_give(_display_mutex);
}

void screen_scroll(int16_t start_line, int16_t lines){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayScroll(start_line, lines);
	mutex_give(_display_mutex);
}

void screen_scroll_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t lines){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayScrollRect(x0, y0, x1, y1, lines);
	mutex_give(_display_mutex);
}

void screen_copy_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t* buf, int32_t stride){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayCopyRect(x0, y0, x1, y1, buf, stride);
	mutex_give(_display_mutex);
}

void screen_draw_pixel(int16_t x, int16_t y){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayPixelSet(x, y);
	mutex_give(_display_mutex);
}

void screen_clear_pixel(int16_t x, int16_t y){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayPixelClear(x, y);
	mutex_give(_display_mutex);
}

void screen_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayLineDraw(x0, y0, x1, y1);
	mutex_give(_display_mutex);
}

void screen_clear_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayLineClear(x0, y0, x1, y1);
	mutex_give(_display_mutex);
}

void screen_draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayRectDraw(x0, y0, x1, y1);
	mutex_give(_display_mutex);
}

void screen_clear_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayRectClear(x0, y0, x1, y1);
	mutex_give(_display_mutex);
}

void screen_fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayRectFill(x0, y0, x1, y1);
	mutex_give(_display_mutex);
}

void screen_draw_circle(int16_t x, int16_t y, int16_t radius){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayCircleDraw(x, y, radius);
	mutex_give(_display_mutex);
}

void screen_clear_circle(int16_t x, int16_t y, int16_t radius){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayCircleClear(x, y, radius);
	mutex_give(_display_mutex);
}

void screen_fill_circle(int16_t x, int16_t y, int16_t radius){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayCircleFill(x, y, radius);
	mutex_give(_display_mutex);
}

/******************************************************************************/
/**                       Screen Text Display Functions                      **/
/**                                                                          **/
/**     These functions allow programmers to display text on the v5 screen   **/
/******************************************************************************/

void screen_print(text_format_e_t txt_fmt, const int16_t line, const char* text, ...){
    va_list args;
    va_start(args, text);
    screen_vprintf((uint8_t)txt_fmt, line, text, args);
    va_end(args);
}

void screen_print_at(text_format_e_t txt_fmt, int16_t x, int16_t y, const char* text, ...){
    va_list args;
    va_start(args, text);
    screen_vprintf_at((uint8_t)txt_fmt, x, y, text, args);
    va_end(args);
}

void screen_vprintf(text_format_e_t txt_fmt, const int16_t line, const char* text, va_list args){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	char* out;
	vasprintf(&out, text, args);
	va_list empty;
	switch(txt_fmt){
	case 0:
	case 1:
		{
            vexDisplayVString(line, out, empty);
            break;
        }
		case 2:
		{
            vexDisplayVBigString(line, out, empty);
            break;
        }
        case 3:
		{
            vexDisplayVCenteredString(line, out, empty);
            break;
        }
        case 4:
        {
            vexDisplayVBigCenteredString(line, out, empty);
            break;
        }
        default:
        {
            vexDisplayVString(line, out, empty);
            break;
        }
    }
	mutex_give(_display_mutex);
}

void screen_vprintf_at(text_format_e_t txt_fmt, const int16_t x, const int16_t y, const char* text, va_list args){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	char* out;
	vasprintf(&out, text, args);
	va_list empty;
	switch(txt_fmt){
		case 0:
		{
            vexDisplayVSmallStringAt(x, y, out, empty);
            break;
        }
		case 1:
		case 3:
		{
            vexDisplayVStringAt(x, y, out, empty);
            break;
        }
		case 2:
		case 4:
		{
            vexDisplayVBigStringAt(x, y, out, empty);
            break;
        }
        default:
        {
            vexDisplayVStringAt(x, y, out, empty);
            break;
        }
    }
	mutex_give(_display_mutex);
}
/******************************************************************************/
/**                         Screen Touch Functions                           **/
/**                                                                          **/
/**               These functions allow programmers to access                **/
/**                    information about screen touches                      **/
/******************************************************************************/

screen_touch_status_s_t screen_touch_status(void){
    V5_TouchStatus v5_touch_status;
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexTouchDataGet(&v5_touch_status);
    screen_touch_status_s_t rtv;
    rtv.touch_status = (last_touch_e_t)v5_touch_status.lastEvent;
    rtv.x = v5_touch_status.lastXpos;
    rtv.y = v5_touch_status.lastYpos;
    rtv.press_count = v5_touch_status.pressCount;
    rtv.release_count = v5_touch_status.releaseCount;
	mutex_give(_display_mutex);
	return rtv;
}

static linked_list_s_t* _touch_event_release_handler_list = NULL;
static linked_list_s_t* _touch_event_press_handler_list = NULL;
static linked_list_s_t* _touch_event_press_auto_handler_list = NULL;

static void _set_up_touch_callback_storage() {
	_touch_event_release_handler_list = linked_list_init();
	_touch_event_press_handler_list = linked_list_init();
	_touch_event_press_auto_handler_list = linked_list_init();
}

void register_touch_callback(touch_event_cb_fn_t cb, touch_event_e_t event_type) {
	mutex_take(_touch_event_release_handler_list, TIMEOUT_MAX);
	switch (event_type) {
	case E_TOUCH_EVENT_RELEASE:
		linked_list_prepend_func(_touch_event_release_handler_list, (generic_fn_t)cb);
		break;
	case E_TOUCH_EVENT_PRESS:
		linked_list_prepend_func(_touch_event_press_handler_list, (generic_fn_t)cb);
		break;
	case E_TOUCH_EVENT_PRESS_AND_HOLD:
		linked_list_prepend_func(_touch_event_press_auto_handler_list, (generic_fn_t)cb);
		break;
	}
	mutex_give(_touch_event_release_handler_list);
}

static task_stack_t touch_handle_task_stack[TASK_STACK_DEPTH_DEFAULT];
static static_task_s_t touch_handle_task_buffer;
static task_t touch_handle_task;

static void _handle_cb(ll_node_s_t* current, void* data) {
	((touch_event_cb_fn_t)(current->payload.func))(((touch_event_position_data_s_t*)(data))->x,
	                                               ((touch_event_position_data_s_t*)(data))->y);
}

static inline bool _touch_status_equivalent(V5_TouchStatus x, V5_TouchStatus y) {
	return (x.lastEvent == y.lastEvent) && (x.lastXpos == y.lastXpos) && (x.lastYpos == y.lastYpos);
}

void _touch_handle_task(void* ignore) {
	V5_TouchStatus last, current;
	while (true) {
		vexTouchDataGet(&current);
		if (!_touch_status_equivalent(current, last)) {
			touch_event_position_data_s_t event_data = { .x = current.lastXpos, .y = current.lastYpos };
			switch (current.lastEvent) {
			case E_TOUCH_EVENT_RELEASE:
				linked_list_foreach(_touch_event_release_handler_list, _handle_cb, (void*)&event_data);
				break;
			case E_TOUCH_EVENT_PRESS:
				linked_list_foreach(_touch_event_press_handler_list, _handle_cb, (void*)&event_data);
				break;
			case E_TOUCH_EVENT_PRESS_AND_HOLD:
				linked_list_foreach(_touch_event_press_auto_handler_list, _handle_cb,
				                    (void*)&event_data);
				break;
			}
			last = current;
		}
		delay(10);
	}
}

void graphical_context_daemon_initialize(void) {
	_display_mutex = mutex_create();
	_set_up_touch_callback_storage();
	touch_handle_task =
	    task_create_static(_touch_handle_task, NULL, TASK_PRIORITY_MIN + 2, TASK_STACK_DEPTH_DEFAULT,
	                       "PROS Graphics Touch Handler", touch_handle_task_stack, &touch_handle_task_buffer);
}
