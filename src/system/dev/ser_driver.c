/**
 * \file system/dev/ser_driver.c
 *
 * Serial Driver
 *
 * Contains the driver for communicating over the serial line. The serial driver
 * is responsible for shipping out all data.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <string.h>

#include "common/cobs.h"
#include "common/set.h"
#include "common/string.h"
#include "kapi.h"
#include "system/dev/ser.h"
#include "system/dev/vfs.h"
#include "system/optimizers.h"
#include "v5_api.h"

#define VEX_SERIAL_BUFFER_SIZE 2047

// ser_file_arg is 2 words (64 bits). The first word is the stream_id
// (i.e. sout/serr/jinx/kdbg) and is exactly 4 characters. The second word
// contains flags for serial driver operation
typedef struct ser_file_arg {
	union {
		uint32_t stream_id;
		uint8_t stream[4];
	};
	enum { E_NOBLK_WRITE = 1 } flags;
} ser_file_s_t;

#define STDIN_STREAM_ID 0x706e6973   // 'sinp' little endian
#define STDOUT_STREAM_ID 0x74756f73  // 'sout' little endian
#define STDERR_STREAM_ID 0x72726573  // 'serr' little endian
#define KDBG_STREAM_ID 0x6762646b    // 'kdbg' little endian

// This array contains the serial driver's arguments for the 4 reserved file
// descriptors. The fact that this array matches the order of the 4 reserved
// file descriptors is mostly irrelevant. We do need to know which one is which,
// but they get mapped in ser_driver_initialize.
static ser_file_s_t RESERVED_SER_FILES[] = {
    {.stream_id = STDIN_STREAM_ID, .flags = 0},
    {.stream_id = STDOUT_STREAM_ID, .flags = 0},
    {.stream_id = STDERR_STREAM_ID, .flags = 0},
    {.stream_id = KDBG_STREAM_ID, .flags = 0},
};

// These mutexes are initialized in ser_driver_initialize
static static_sem_s_t read_mtx_buf;
static static_sem_s_t write_mtx_buf;
static mutex_t read_mtx;   // ensures that only one read is happening at a time
static mutex_t write_mtx;  // ensures that only one write is happening at a time

// Write buffer as a stream buffer. Initialized below in ser_driver_initialize
static static_stream_buf_s_t write_stream_buf;
static uint8_t write_buf[VEX_SERIAL_BUFFER_SIZE + 1];
static uint8_t write_scratch_buf[VEX_SERIAL_BUFFER_SIZE];  // scratch buffer
static stream_buf_t write_stream;

// We maintain a set of streams which should actually be sent over the serial
// line. This is maintained as a separate list and don't traverse through
// open files b/c enabled streams is done per ID not per file (multiple
// files may map to one stream ID)
// Initialized below in ser_driver_initialize
static struct set enabled_streams_set;

// stderr is ALWAYS guaranteed to be sent over the serial line. stdout and
// others may be disabled
static const uint32_t guaranteed_delivery_streams[] = {
    // STDIN_STREAM_ID,
    // STDOUT_STREAM_ID,  stdout isn't guaranteed delivery, but enabled by
    // default
    STDERR_STREAM_ID,
    // KDBG_STREAM_ID,
};
#define guaranteed_delivery_streams_size (sizeof(guaranteed_delivery_streams) / sizeof(*guaranteed_delivery_streams))

// global runtime config for the serial driver
static enum { E_COBS_ENABLED = 1 } ser_driver_runtime_config;

// comes from ser_daemon
extern int32_t inp_buffer_read(uint32_t timeout);

/******************************************************************************/
/**                              Output queue                                **/
/**                                                                          **/
/** vexSerialWriteBuffer doesn't seem to be very thread safe, so the system  **/
/** daemon flushes an intermediary buffer once before vexBackgroundProcessing**/
/** calls to write add to the queue. Flushing is optimized because we have   **/
/** underlying access to the write buffer, as opposed to calling queue_recv  **/
/** a bunch of times                                                         **/
/******************************************************************************/
void ser_output_flush(void) {
	size_t len = stream_buf_recv(write_stream, write_scratch_buf, vexSerialWriteFree(1), 0);
	uint32_t ret = vexSerialWriteBuffer(1, write_scratch_buf, len);
	if (ret != len) {
		display_error("WARNING: some serial data has been dropped");
	}
}

bool ser_output_write(const uint8_t* buffer, size_t size, bool noblock) {
	return stream_buf_send(write_stream, buffer, size, noblock ? 0 : TIMEOUT_MAX);
}

/******************************************************************************/
/**                         newlib driver functions                          **/
/******************************************************************************/
int ser_read_r(struct _reent* r, void* const arg, uint8_t* buffer, const size_t len) {
	// arg isn't used since serial reads aren't stream-based
	size_t read = 0;
	int32_t c;
	if (!mutex_take(read_mtx, TIMEOUT_MAX)) {
		r->_errno = EACCES;
		return 0;
	}
	while (read < len) {
		// the logic inside this loop is a bit funky but seems to make newlib behave
		// properly
		c = inp_buffer_read(0);
		// Don't get rid of the literal type suffix, it ensures optimiziations don't
		// break this condition
		if (c == -1L) {
			if (read) {
				break;
			} else {
				continue;
			}
		}
		*(buffer++) = (uint8_t)c;
		read++;
		if (c == '\n') {
			break;
		}
	}
	mutex_give(read_mtx);
	*buffer = 0;
	return read;
}

int ser_write_r(struct _reent* r, void* const arg, const uint8_t* buf, const size_t len) {
	const ser_file_s_t file = *(ser_file_s_t*)arg;

	if (!list_contains(guaranteed_delivery_streams, guaranteed_delivery_streams_size, file.stream_id) &&
	    !set_contains(&enabled_streams_set, file.stream_id)) {
		// the stream isn't a guaranteed delivery or hasn't been enabled so just
		// pretend like the data was shipped just fine
		return len;
	}

	if (ser_driver_runtime_config &= E_COBS_ENABLED) {
		// allocate stack space for the buffer to be cobs encoded
		const size_t cobs_len = cobs_encode_measure(buf, len, file.stream_id);
		uint8_t cobs_buf[cobs_len + 1];

		// need to guarantee writes are in order
		if (!mutex_take(write_mtx, (file.flags & E_NOBLK_WRITE) ? 0 : TIMEOUT_MAX)) {
			r->_errno = EACCES;
			return 0;
		}
		cobs_encode(cobs_buf, buf, len, file.stream_id);
		cobs_buf[cobs_len] = 0;

		bool ret = ser_output_write(cobs_buf, cobs_len + 1, file.flags & E_NOBLK_WRITE);
		mutex_give(write_mtx);

		if (!ret) {
			r->_errno = EIO;
			return 0;
		}
		return len;
	} else {
		// need to guarantee writes are in order
		if (!mutex_take(write_mtx, TIMEOUT_MAX)) {
			r->_errno = EACCES;
			return 0;
		}

		bool ret = ser_output_write(buf, len, file.flags & E_NOBLK_WRITE);

		mutex_give(write_mtx);
		if (!ret) {
			r->_errno = EIO;
			return 0;
		}
		return len;
	}
}

int ser_close_r(struct _reent* r, void* const arg) {
	// This does nothing for now, may be implemented later
	return 0;
}

int ser_fstat_r(struct _reent* r, void* const arg, struct stat* st) {
	// this is a complete implementation
	st->st_mode = S_IFCHR;
	return 0;
}

int ser_isatty_r(struct _reent* r, void* const arg) {
	// this is a complete implementation
	return 1;
}

off_t ser_lseek_r(struct _reent* r, void* const arg, off_t ptr, int dir) {
	// lseek doesn't make sense on a serial line
	r->_errno = ESPIPE;
	return -1;
}

int ser_ctl(void* const arg, const uint32_t cmd, void* const extra_arg) {
	ser_file_s_t file = *(ser_file_s_t*)arg;
	switch (cmd) {
		case SERCTL_ACTIVATE:
			if (!list_contains(guaranteed_delivery_streams, guaranteed_delivery_streams_size, (uint32_t)file.stream_id)) {
				set_add(&enabled_streams_set, (uint32_t)file.stream_id);
			}
			return 0;
		case SERCTL_DEACTIVATE:
			if (!list_contains(guaranteed_delivery_streams, guaranteed_delivery_streams_size, (uint32_t)file.stream_id)) {
				set_rm(&enabled_streams_set, (uint32_t)file.stream_id);
			}
			return 0;
		case SERCTL_BLKWRITE:
			file.flags &= ~E_NOBLK_WRITE;
			return 0;
		case SERCTL_NOBLKWRITE:
			file.flags |= E_NOBLK_WRITE;
			return 0;
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
}

/******************************************************************************/
/**                           Driver description                             **/
/******************************************************************************/
const struct fs_driver _ser_driver = {.close_r = ser_close_r,
                                      .fstat_r = ser_fstat_r,
                                      .isatty_r = ser_isatty_r,
                                      .lseek_r = ser_lseek_r,
                                      .read_r = ser_read_r,
                                      .write_r = ser_write_r,
                                      .ctl = ser_ctl};

const struct fs_driver* const ser_driver = &_ser_driver;

int ser_open_r(struct _reent* r, const char* path, int flags, int mode) {
	if (*path == '\0') {
		return STDOUT_FILENO;
	}

	if (*path == '/') {
		path++;
	}

	// check length of path - it MUST be at most 4 characters
	size_t i;
	for (i = 0; i < 4; i++) {
		if (path[i] == '\0') {
			break;
		}
	}
	if (path[i] != '\0') {  // i will the length of the path or the fifth character
		r->_errno = ENAMETOOLONG;
		return -1;
	}

	if (!strcmp(path, "sout")) {
		return STDOUT_FILENO;
	}
	if (!strcmp(path, "sin")) {
		return STDIN_FILENO;
	}
	if (!strcmp(path, "serr")) {
		return STDERR_FILENO;
	}

	ser_file_s_t* arg = kmalloc(sizeof(*arg));
	memcpy(arg->stream, path, strlen(path));
	return vfs_add_entry_r(r, ser_driver, arg);
}

// control various components of the serial driver or a file
int32_t serctl(const uint32_t action, void* const extra_arg) {
	switch (action) {
		case SERCTL_ACTIVATE:
			if (!list_contains(guaranteed_delivery_streams, guaranteed_delivery_streams_size, (uint32_t)extra_arg)) {
				set_add(&enabled_streams_set, (uint32_t)extra_arg);
			} else {
				errno = EIO;
				return -1;
			}
			return 0;
		case SERCTL_DEACTIVATE:
			if (!list_contains(guaranteed_delivery_streams, guaranteed_delivery_streams_size, (uint32_t)extra_arg)) {
				set_rm(&enabled_streams_set, (uint32_t)extra_arg);
			} else {
				errno = EIO;
				return -1;
			}
			return 0;
		case SERCTL_ENABLE_COBS:
			ser_driver_runtime_config |= E_COBS_ENABLED;
			return 0;
		case SERCTL_DISABLE_COBS:
			ser_driver_runtime_config &= ~E_COBS_ENABLED;
			return 0;
		default:
			errno = EINVAL;
			return PROS_ERR;
	}
}

// called by ser_initialize() in ser_daemon.c
// vfs_initialize() calls ser_initialize()
void ser_driver_initialize(void) {
	ser_driver_runtime_config |= E_COBS_ENABLED;  // start with cobs enabled

	read_mtx = mutex_create_static(&read_mtx_buf);
	write_mtx = mutex_create_static(&write_mtx_buf);

	set_initialize(&enabled_streams_set);
	set_add(&enabled_streams_set, STDOUT_STREAM_ID);  // 'sout' little endian

	write_stream = stream_buf_create_static(VEX_SERIAL_BUFFER_SIZE, 0, write_buf, &write_stream_buf);

	vfs_update_entry(STDIN_FILENO, ser_driver, &(RESERVED_SER_FILES[0]));
	vfs_update_entry(STDOUT_FILENO, ser_driver, &(RESERVED_SER_FILES[1]));
	vfs_update_entry(STDERR_FILENO, ser_driver, &(RESERVED_SER_FILES[2]));
	vfs_update_entry(KDBG_FILENO, ser_driver, &(RESERVED_SER_FILES[3]));
}
