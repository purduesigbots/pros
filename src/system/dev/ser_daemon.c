/**
 * \file system/dev/ser_daemon.c
 *
 * Serial Input Daemon
 *
 * The serial input daemon is responsible for polling the serial line for
 * characters and responding to any kernel commands (like printing the banner or
 * enabling COBS)
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <errno.h>

#include "kapi.h"
#include "system/dev/banners.h"
#include "system/hot.h"
#include "system/optimizers.h"
#include "v5_api.h"

#define MAX_COMMAND_LENGTH 32

__attribute__((weak)) char const* const _PROS_COMPILE_TIMESTAMP = "Unknown";
__attribute__((weak)) char const* const _PROS_COMPILE_DIRECTORY = "Unknown";
__attribute__((weak)) const int         _PROS_COMPILE_TIMESTAMP_INT = 0;

void print_small_banner(void) {
	uint32_t uptime = millis();
	char const * const timestamp = (HOT_TABLE && HOT_TABLE->compile_timestamp) ? HOT_TABLE->compile_timestamp : _PROS_COMPILE_TIMESTAMP;
	char const * const directory = (HOT_TABLE && HOT_TABLE->compile_directory) ? HOT_TABLE->compile_directory : _PROS_COMPILE_DIRECTORY;
	iprintf(short_banner, PROS_VERSION_STRING, uptime / 1000, uptime % 1000, timestamp,
	        directory);
}

void print_large_banner(void) {
	uint8_t version[4];
	uint32_t* sys_ver = (uint32_t*)version;
	*sys_ver = vexSystemVersion();
	uint32_t uptime = millis();
	char const * const timestamp = (HOT_TABLE && HOT_TABLE->compile_timestamp) ? HOT_TABLE->compile_timestamp : _PROS_COMPILE_TIMESTAMP;
	char const * const directory = (HOT_TABLE && HOT_TABLE->compile_directory) ? HOT_TABLE->compile_directory : _PROS_COMPILE_DIRECTORY;
	iprintf(large_banner, PROS_VERSION_STRING, version[3], version[2], version[1], version[0], uptime / 1000,
	        uptime % 1000, timestamp, directory);
}

/******************************************************************************/
/**                              Input buffer                                **/
/**                                                                          **/
/** this is what read() reads from. Implemented as a ring buffer             **/
/**  TODO: just use a FreeRTOS queue instead of 2 semaphores                 **/
/******************************************************************************/
#define INP_BUFFER_SIZE 0x1000  // 4KB... which is larger than VEX's output buffer -_-

static static_stream_buf_s_t inp_stream_buf;
static uint8_t inp_buffer[INP_BUFFER_SIZE];
static stream_buf_t inp_stream;

static inline void inp_buffer_initialize() {
	inp_stream = stream_buf_create_static(INP_BUFFER_SIZE, 1, inp_buffer, &inp_stream_buf);
}

// if you extern this function you can place characters on the rest of the
// system's input buffer
bool inp_buffer_post(uint8_t b) {
	return stream_buf_send(inp_stream, &b, 1, TIMEOUT_MAX);
}

int32_t inp_buffer_read(uint32_t timeout) {
	// polling the semaphore from a higher priority task (as would be normal) will
	// starve the ser_daemon_task
	if (timeout == 0) {
		timeout = 1;
	}
	uint8_t b;
	if (!stream_buf_recv(inp_stream, &b, 1, timeout)) {
		return -1;
	}
	return (int32_t)b;
}

// returns the number of bytes currently in the stream
int32_t inp_buffer_available() {
	return stream_buf_get_used(inp_stream);
}

/******************************************************************************/
/**                              Serial Daemon                               **/
/******************************************************************************/
static task_stack_t ser_daemon_stack[TASK_STACK_DEPTH_MIN];
static static_task_s_t ser_daemon_task_buffer;

static inline uint8_t vex_read_char() {
	int32_t b = vexSerialReadChar(1);
	while (b == -1L) {
		task_delay(1);
		b = vexSerialReadChar(1);
	}
	// Don't get rid of the literal type suffix, it ensures optimiziations don't
	// break this condition
	return (uint8_t)b;
}

static void ser_daemon_task(void* ign) {
	uint8_t command_stack[MAX_COMMAND_LENGTH];
	size_t command_stack_idx = 0;

	print_large_banner();

	while (1) {
		uint8_t b = vex_read_char();
		if (b == 'p') {  // TODO: make the command prefix not typeable
			command_stack[command_stack_idx++] = b;
			b = command_stack[command_stack_idx++] = vex_read_char();
			if (b == 'R') {
				b = command_stack[command_stack_idx++] = vex_read_char();
				switch (b) {
					case 'a':
						fprintf(stderr, "I'm alive!\n");
						command_stack_idx = 0;
						break;
					case 'b':
						task_delay(20);
						print_small_banner();
						command_stack_idx = 0;
						break;
					case 'B':
						task_delay(20);
						print_large_banner();
						command_stack_idx = 0;
						break;
					case 'e':
						// read next 4 bytes
						command_stack[command_stack_idx++] = vex_read_char();
						command_stack[command_stack_idx++] = vex_read_char();
						command_stack[command_stack_idx++] = vex_read_char();
						command_stack[command_stack_idx++] = vex_read_char();
						// the parameter expected to serctl is the stream id (a uint32_t), so
						// we
						// need to cast to a uint32_t pointer, dereference it, and cast to a
						// void* to make the compiler happy
						serctl(SERCTL_ACTIVATE, (void*)(*(uint32_t*)(command_stack + 3)));
						// printf("enabled %s\n", command_stack + 3);
						command_stack_idx = 0;
						break;
					case 'd':
						// read next 4 bytes
						command_stack[command_stack_idx++] = vex_read_char();
						command_stack[command_stack_idx++] = vex_read_char();
						command_stack[command_stack_idx++] = vex_read_char();
						command_stack[command_stack_idx++] = vex_read_char();
						serctl(SERCTL_DEACTIVATE, (void*)(*(uint32_t*)(command_stack + 3)));
						// printf("disabled %s\n", command_stack+3);
						command_stack_idx = 0;
						break;
					case 'c':
						serctl(SERCTL_ENABLE_COBS, NULL);
						command_stack_idx = 0;
						break;
					case 'r':
						serctl(SERCTL_DISABLE_COBS, NULL);
						command_stack_idx = 0;
						break;
					default:
						command_stack_idx = 0;
						break;
				}
			}

			for (size_t i = 0; i < command_stack_idx; i++) {
				// empty out the command stack onto the input buffer since something
				// wasn't right with the command
				inp_buffer_post(command_stack[i]);
			}
			command_stack_idx = 0;
		} else {
			inp_buffer_post(b);
		}
	}
}

void ser_initialize(void) {
	inp_buffer_initialize();
	extern void ser_driver_initialize(void);
	ser_driver_initialize();

	task_create_static(ser_daemon_task, NULL, TASK_PRIORITY_MIN + 1, TASK_STACK_DEPTH_MIN, "Serial Daemon (PROS)",
	                   ser_daemon_stack, &ser_daemon_task_buffer);
}
