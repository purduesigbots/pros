/**
 * \file tests/generic_serial_file.cpp
 *
 * Test code for the generic serial filesystem driver
 *
 * NOTE: There should be a cable plugged into ports 1 and 2, connecting
 * them together
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "main.h"
#include <stdarg.h>
#include "pros/apix.h"

FILE *serial_w = nullptr, *serial_r = nullptr;
char serial_w_name[8], serial_r_name[8];
int serial_w_port, serial_r_port;

constexpr bool VERBOSE_OUTPUT = false;

constexpr uint32_t BUF_SIZE = 65536;
uint8_t out_buf[BUF_SIZE], in_buf[BUF_SIZE];

void verbose_printf(const char *format, ...) {
	if (VERBOSE_OUTPUT) {
		va_list va;
		va_start(va, format);
		vprintf(format, va);
		va_end(va);
	}
}

bool test_send_recv_byte(const uint32_t interval, const uint32_t bytes) {
	printf("%07d Starting send/recv byte test with an interval of %dms for %d bytes...\n", pros::millis(), interval, bytes);

	uint8_t count = 0, expected = 0;
	uint32_t last_send_time = 0, cur_time, recv_count = 0;
	bool pass = true;

	do {
		cur_time = pros::millis();

		if (cur_time - last_send_time >= interval) {
			int32_t w = fputc(count, serial_w);
			fflush(serial_w);
			if (w == EOF) {
				pass = false;
				printf("%07d Write failed with error %d\n", cur_time, errno);
				break;
			}
			else if (w == count) {
				count++;
				last_send_time = cur_time;
			}
		}

		int32_t read = fgetc(serial_r);
		if (read == EOF) {
			pass = false;
			printf("%07d Read failed with error %d\n", cur_time, errno);
			break;
		}
		else {
			if (read != expected) {
				printf("%07d ERR Read: 0x%02x, Expt: 0x%02x\n", cur_time, read, expected);
				pass = false;
			}
			else {
				verbose_printf("%07d 0x%02x\n", cur_time, read);
			}
			expected++;
			recv_count++;
		}

		pros::delay(interval);
	} while (recv_count < bytes);

	if (pass)
		printf("%07d PASS\n", pros::millis());
	else
		printf("%07d FAIL\n", pros::millis());

	return pass;
}

bool test_send_recv_block() {
	printf("%07d Starting send/recv block test...\n", pros::millis());

	uint32_t written = 0, read = 0;
	bool pass = true;

	for (uint32_t i = 0; i < BUF_SIZE; i++) {
		out_buf[i] = i;
	}

	while (written < BUF_SIZE || read < BUF_SIZE) {
		uint32_t cur_time = pros::millis();
		int32_t free = pros::c::fdctl(fileno(serial_w), DEVCTL_FIONWRITE, NULL);
		int32_t w = fwrite(out_buf + written, 1, BUF_SIZE - written > free ? free : BUF_SIZE - written, serial_w);
		fflush(serial_w);
		if (w == EOF) {
			pass = false;
			printf("%07d Write failed with error %d\n", cur_time, errno);
			break;
		}
		else if (w) {
			verbose_printf("%07d, W %d\n", cur_time, w);
			written += w;
		}

		int32_t avail = pros::c::fdctl(fileno(serial_r), DEVCTL_FIONREAD, NULL);
		int32_t r = fread(in_buf + read, 1, BUF_SIZE - read > avail ? avail : BUF_SIZE - read, serial_r);
		if (r == EOF) {
			pass = false;
			printf("%07d Read failed with error %d\n", cur_time, errno);
			break;
		}
		else if (r) {
			verbose_printf("%07d R %d [", cur_time, r);
			for (int32_t i = 0; i < r; i++) {
				verbose_printf("%02x", in_buf[read + i]);
				if (in_buf[read + i] != out_buf[read + i]) {
					pass = false;
					verbose_printf(" (%02x)", out_buf[read + i]);
				}
				if (i < r - 1)
					verbose_printf(", ");
			}
			verbose_printf("]\n");

			read += r;
		}

		pros::delay(1);
	}

	if (pass)
		printf("%07d PASS\n", pros::millis());
	else
		printf("%07d FAIL\n", pros::millis());

	return pass;
}

void set_baudrate(const int32_t baudrate) {
	printf("%07d Setting baudrate to %d\n", pros::millis(), baudrate);
	pros::c::fdctl(fileno(serial_w), DEVCTL_SET_BAUDRATE, (void *)baudrate);
	pros::c::fdctl(fileno(serial_r), DEVCTL_SET_BAUDRATE, (void *)baudrate);
}

void flush() {
	pros::c::serial_flush(serial_w_port);
	pros::delay(100);
	pros::c::serial_flush(serial_r_port);
}

void init_ports(uint8_t write_port, uint8_t recv_port) {
	printf("%07d Using port %d to write and %d to recv\n", pros::millis(), write_port, recv_port);

	serial_w_port = write_port;
	if (serial_w != nullptr) fclose(serial_w);
	sprintf(serial_w_name, "/dev/%d", write_port);
	serial_w = fopen(serial_w_name, "wb");
	serial_r_port = recv_port;
	if (serial_r != nullptr) fclose(serial_r);
	sprintf(serial_r_name, "/dev/%d", recv_port);
	serial_r = fopen(serial_r_name, "rb");
}

bool run_tests() {
	flush();
	set_baudrate(115200);
	if (!test_send_recv_byte(5, 1000)) return false;

	flush();
	set_baudrate(230400);
	if (!test_send_recv_byte(2, 2500)) return false;

	for (int i = 0; i < 5; i++) {
		flush();
		if (!test_send_recv_block()) return false;
	}

	return true;
}

void opcontrol() {
	printf("---Generic Serial Test---\nPlease ensure a cable is plugged into port 1 and port 2, connecting them together\n\n%07d Starting serial tests...\n", pros::millis());

	init_ports(1, 2);
	if (!run_tests()) return;

	init_ports(2, 1);
	if (!run_tests()) return;

	printf("%07d All tests passed!\n", pros::millis());
}
