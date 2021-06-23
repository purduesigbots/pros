/*
 * \file graphics.c
 *
 * \brief Lower level graphical functions for screen displays. This lower level
 * is necessary to house the system task that should not be visible to users.
 *
 * This file represents a low-level interface for interacting with the built-in
 * LCD touch screen.
 *
 * \copyright (c) 2017, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//this is a test
#include "display/graphics.h"

#include <stdint.h>
#include <stdlib.h>  // free
#include <sys/_stdint.h>

#include "pros/apix.h"
#include "v5_api.h"  // vexDisplay*

static mutex_t _display_mutex = NULL;

void display_set_color_fg(uint32_t color) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayForegroundColor(color);
	mutex_give(_display_mutex);
}

uint32_t display_get_color_fg() {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	uint32_t color = vexDisplayForegroundColorGet();
	mutex_give(_display_mutex);
	return color;
}

void display_set_color_bg(uint32_t color) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayBackgroundColor(color);
	mutex_give(_display_mutex);
}

uint32_t display_get_color_bg() {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	uint32_t color = vexDisplayBackgroundColorGet();
	mutex_give(_display_mutex);
	return color;
}

void display_erase(void) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayErase();
	mutex_give(_display_mutex);
}

void display_scroll(int16_t start_line, int16_t lines) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayScroll(start_line, lines);
	mutex_give(_display_mutex);
}
void display_scroll_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t lines) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayScrollRect(x0, y0, x1, y1, lines);
	mutex_give(_display_mutex);
}

void display_copy_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t* buf, int32_t stride) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayCopyRect(x0, y0, x1, y1, buf, stride);
	mutex_give(_display_mutex);
}

void display_set_pixel_fg(int16_t x, int16_t y) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayPixelSet(x, y);
	mutex_give(_display_mutex);
}
void display_set_pixel_bg(int16_t x, int16_t y) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayPixelClear(x, y);
	mutex_give(_display_mutex);
}
void display_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayLineDraw(x0, y0, x1, y1);
	mutex_give(_display_mutex);
}
void display_clear_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayLineClear(x0, y0, x1, y1);
	mutex_give(_display_mutex);
}

void display_draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayRectDraw(x0, y0, x1, y1);
	mutex_give(_display_mutex);
}
void display_clear_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayRectClear(x0, y0, x1, y1);
	mutex_give(_display_mutex);
}
void display_fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayRectFill(x0, y0, x1, y1);
	mutex_give(_display_mutex);
}

void display_draw_circle(int16_t x, int16_t y, int16_t r) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayCircleDraw(x, y, r);
	mutex_give(_display_mutex);
}
void display_clear_circle(int16_t x, int16_t y, int16_t r) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayCircleClear(x, y, r);
	mutex_give(_display_mutex);
}
void display_fill_circle(int16_t x, int16_t y, int16_t r) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexDisplayCircleFill(x, y, r);
	mutex_give(_display_mutex);
}

//Text Display Functions
void display_vprintf(uint8_t txt_fmt, const int16_t line, const char* fmt, va_list args){
    mutex_take(_display_mutex, TIMEOUT_MAX);
	char* out;
	vasprintf(&out, fmt, args);
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

void display_vprintf_at(uint8_t txt_fmt, const int16_t x, const int16_t y, const char* fmt, va_list args){
	mutex_take(_display_mutex, TIMEOUT_MAX);
	char* out;
	vasprintf(&out, fmt, args);
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

void display_opacity_vprintf( int32_t xpos, int32_t ypos, uint32_t opacity, const char *format, va_list args) {
	mutex_take(_display_mutex, TIMEOUT_MAX);
	char* out;
	vasprintf(&out, format, args);
	va_list empty;
	vexDisplayVPrintf(xpos, ypos, opacity, out, empty);
	mutex_give(_display_mutex);
}

int16_t touch_last_x() {
	V5_TouchStatus v5_touch_status;
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexTouchDataGet(&v5_touch_status);
	mutex_give(_display_mutex);
	return v5_touch_status.lastXpos;
}

int16_t touch_last_y() {
	V5_TouchStatus v5_touch_status;
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexTouchDataGet(&v5_touch_status);
	mutex_give(_display_mutex);
	return v5_touch_status.lastYpos;
}

touch_event_e_t touch_last_event() {
	V5_TouchStatus v5_touch_status;
	mutex_take(_display_mutex, TIMEOUT_MAX);
	vexTouchDataGet(&v5_touch_status);
	mutex_give(_display_mutex);
	return v5_touch_status.lastEvent;
}

static linked_list_s_t* _touch_event_release_handler_list = NULL;
static linked_list_s_t* _touch_event_press_handler_list = NULL;
static linked_list_s_t* _touch_event_press_auto_handler_list = NULL;

void _set_up_touch_callback_storage() {
	_touch_event_release_handler_list = linked_list_init();
	_touch_event_press_handler_list = linked_list_init();
	_touch_event_press_auto_handler_list = linked_list_init();
}

void register_touch_callback(touch_event_cb_fn_t cb, touch_event_e_t event_type) {
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
}

static task_stack_t touch_handle_task_stack[TASK_STACK_DEPTH_DEFAULT];
static static_task_s_t touch_handle_task_buffer;
zz task_t touch_handle_task;

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
		delay(5);
	}
}

void graphical_context_daemon_initialize(void) {
	_display_mutex = mutex_create();
	_set_up_touch_callback_storage();
	touch_handle_task =
	    task_create_static(_touch_handle_task, NULL, TASK_PRIORITY_MAX - 3, TASK_STACK_DEPTH_DEFAULT,
	                       "PROS Graphics Touch Handler", touch_handle_task_stack, &touch_handle_task_buffer);
}
