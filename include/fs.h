/*
 * fs.c - PROS File System functions
 *
 * Copyright (c) 2011-2016, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#ifndef PROS_FS_H_
#define PROS_FS_H_

#include <comm.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//#define NO_FILESYSTEM

#ifndef NO_FILESYSTEM

// Determine the flash bank size
#define NUM_PAGES 128U
#define PAGE_SIZE 2048U

// Maximum allowable file name length
#define MAX_LEN 8U

// Maximum number of simultaneously open files
#define MAX_FILES 4U

// Parameters for the fsSeek function family
#ifndef SEEK_SET
#define	SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define	SEEK_CUR 1
#endif
#ifndef SEEK_END
#define	SEEK_END 2
#endif

/**
 * Closes the specified file descriptor.
 *
 * @param fd the descriptor to close
 */
void fclose(PROS_FILE *fd);
/**
 * Delete the specified file if it exists and is not currently open.
 *
 * The file will actually be erased from memory on the next re-boot.
 *
 * @param file the file name to erase
 * @return 0 if the file was deleted, or 1 if the file could not be found
 */
int fdelete(const char *file);
/**
 * Checks to see if the specified file is at its end. Only works for files in Read mode;
 * for files in any other mode, 1 is always returned.
 *
 * @param fd the channel to check
 * @return 0 if the file is not at EOF, or 1 otherwise.
 */
int fsEof(PROS_FILE *fd);
/**
 * Flushes the data on the specified file channel.
 *
 * @param fd the channel to flush
 * @return 0 if the data was successfully written out to Flash, EOF otherwise
 * The file data is still invalid until properly closed!
 */
int fflush(PROS_FILE *fd);
/**
 * Returns the number of bytes left in the file until EOF. Only applies to files open for
 * Read mode.
 *
 * @param fd the file channel to check
 * @return the number of bytes left, or 0 if this value could not be determined
 */
uint32_t fsLeft(PROS_FILE *fd);
/**
 * fopen() function for file system
 *
 * @param file the file name
 * @param mode the file mode
 * @return a pointer to file data, or NULL if the file could not be opened
 */
PROS_FILE * fopen(const char *file, const char *mode);
/**
 * fread() function for file system
 *
 * @param fd the channel to read
 * @return the next byte in the file, or EOF if no byte can be read (end of file, in write
 * mode...)
 */
int fsRead(PROS_FILE *fd);
/**
 * Scans the file system and builds a RAM structure describing the files
 * currently on the file system and the free space available.
 *
 * @param trim true if TRIM should occur, false otherwise
 */
void fsScan(bool trim);
/**
 * fseek() function for file system
 *
 * @param fd the channel to modify
 * @param offset the location within the stream to seek
 * @param origin the reference location for offset: SEEK_CUR, SEEK_SET, or SEEK_END
 * @return 0 if the seek was successful, or 1 otherwise
 */
int fseek(PROS_FILE *fd, long int offset, int origin);
/**
 * ftell() function for file system
 *
 * @param fd the channel to check
 * @return the offset of the stream, or -1 if the offset could not be determined
 */
long int ftell(PROS_FILE *fd);
/**
 * fwrite() function for file system
 *
 * @param fd the channel to write
 * @param value the byte to write
 * @return value if the byte was successfully written and EOF otherwise
 */
int fsWrite(PROS_FILE *fd, int value);

#endif

#ifdef __cplusplus
}
#endif

#endif
