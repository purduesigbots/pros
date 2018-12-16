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

#include <comm.h>
#include <periph.h>
#include <stdlib.h>
#include <limits.h>
#include <fs.h>
#include <string.h>

#ifndef NO_FILESYSTEM

/**
 * Return pointer to given memory page. Skips the first 128 K (64 pages).
 * Assumes memory can be mapped into linear address space (not AVR!)
 */
static INLINE void * _fs_pointer(const uint32_t page) {
	return (void *)((FLASH_BASE + PAGE_SIZE * 64U) + page * PAGE_SIZE);
}

/**
 * Enable programming mode for the file system.
 */
static void _fs_prgm_on() {
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
}

/**
 * Disable programming mode for the file system.
 */
static void _fs_prgm_off() {
	FLASH->CR = FLASH_CR_LOCK;
}

/**
 * Erase a page of memory.
 */
static void _fs_erase_page(uint32_t page) {
	FLASH->CR = FLASH_CR_PER;
	// Select page
	FLASH->AR = (uint32_t)_fs_pointer(page);
	// Start operation
	FLASH->CR = FLASH_CR_PER | FLASH_CR_STRT;
	// Stall until BSY is reset (there is no sense sleeping here as IRQs cannot run while
	// the erase is in progress)
	while (FLASH->SR & FLASH_SR_BSY);
	// Verify erasure?
}

/**
 * Program a (half)word to memory.
 */
static void _fs_prgm_word(uint32_t page, uint32_t offs, uint16_t value) {
	FLASH->CR = FLASH_CR_PG;
	// Ensure offs is even
	//offs &= ~0x01;
	// Select address by computing an offset from the desired page
	uint8_t *addr = (uint8_t*)_fs_pointer(page) + offs;
	*((uint16_t *)addr) = value;
	// Stall until BSY is reset
	while (FLASH->SR & FLASH_SR_BSY);
}

// Flags for file status
#define FLAG_FILE_PRESENT 0x01U
#define FLAG_FILE_CONTINUE 0x02U
#define FLAG_FILE_DELETED 0x04U
#define FLAG_FILE_NOUSE 0x08U

// Offsets within start-of-file record of critical data
#define FILE_OFFSET_MAGIC 0x0000U
#define FILE_OFFSET_LEN 0x0004U
#define FILE_OFFSET_STATUS 0x0002U
#define FILE_OFFSET_NAME 0x0008U

// DE DE (little endian) = an illegal instruction (so we can't FP on code pages)
#define FILE_MAGIC 0xDEDE

// Flags for the file open/close status
#define FILE_FLAG_RD 0x0001U
#define FILE_FLAG_WR 0x0002U

// Helper macros (assume little endian!)
#define GET_UINT16(_page, _offset) (*((uint16_t *)(&(_page)[(_offset)])))
#define GET_UINT32(_page, _offset) (*((uint32_t *)(&(_page)[(_offset)])))

// Since PROS uses the first few file descriptors for serial and PC I/O, the
// first file opened must begin after these reserved numbers
#define FILE_CHANNEL_1 ((uint32_t)uart3 + 1U)

typedef struct {
	// Both offset and len must be 32-bit wide to accomodate files over 64K
	// These may be narrowed on some processors to 16-bit
	uint32_t offset;
	uint32_t len;
	// Page number of file
	uint16_t page;
	// File flags
	uint16_t flags;
} FilePointer;

// Filesystem status structure
struct {
	// Page status
	uint8_t page[NUM_PAGES];
	// Open file status
	FilePointer files[MAX_FILES];
	// One page of data is buffered when writing
	uint8_t data[PAGE_SIZE];
} fs;

/*
 * Calculates the longest free block, and returns its start index.
 *
 * @return the longest contiguous blank page range starting index
 */
static uint32_t longestBlock() {
	uint32_t i;
	uint32_t len = 0, blockLen = 0, page = 0;
	for (i = 0; i < NUM_PAGES; i++) {
		if (fs.page[i] == 0) {
			len++;
			// Edge case?
			if (i == NUM_PAGES - 1 && len > blockLen) {
				blockLen = len;
				page = i - len + 1;
			}
		} else {
			// Calculate longest block
			if (len > blockLen) {
				blockLen = len;
				page = i - len;
			}
			len = 0;
		}
	}
	return page;
}

/*
 * Looks for a file in the file system with the given name.
 *
 * @return the matching page number if a match is found, or UINT_MAX otherwise
 */
static uint32_t lookForFile(const char *name) {
	uint32_t i;
	for (i = 0; i < NUM_PAGES; i++) {
		uint8_t *page = (uint8_t *)_fs_pointer(i);
		if (fs.page[i] == FLAG_FILE_PRESENT) {
			// The page has a file, not deleted, not a continuation
			char *str = (char *)(&page[FILE_OFFSET_NAME]);
			if (strncmp(name, str, MAX_LEN) == 0)
				// The file names match
				return i;
		}
	}
	// Return error code
	return UINT_MAX;
}

/*
 * Opens the file for reading.
 *
 * @param idx the file descriptor index
 * @param page the flash page to start reading
 * @param len the length of the file in bytes
 */
static void openForReading(uint32_t idx, uint32_t page, uint32_t len) {
	fs.files[idx].flags = FILE_FLAG_RD;
	fs.files[idx].len = len;
	fs.files[idx].offset = 0;
	fs.files[idx].page = page;
}

/*
 * Opens the file for writing.
 *
 * @param file the file name, up to 8 characters long, null terminated
 * @param idx the file descriptor index
 * @param page the flash page to start writing
 */
static void openForWriting(const char *file, uint32_t idx, uint32_t page) {
	char c, nameBuff[8];
	uint32_t i;
	// Set the flags
	fs.files[idx].flags = FILE_FLAG_WR;
	fs.files[idx].len = 0;
	fs.files[idx].offset = 0;
	fs.files[idx].page = page;
	// Update page records
	fs.page[page] = FLAG_FILE_PRESENT;
	for (i = 0; i < MAX_LEN; i++) {
		c = *file;
		// Pad name out to 8 bytes
		if (c) {
			nameBuff[i] = c;
			file++;
		} else
			nameBuff[i] = (char)0x00;
	}
	// Can write, program the status + name into NV now
	_fs_prgm_on();
	_fs_prgm_word(page, 0, FILE_MAGIC);
	// Status bits = 0xFFFF, len = 0xFFFFFFFF for now
	for (i = 0; i < (MAX_LEN >> 1); i++)
		_fs_prgm_word(page, FILE_OFFSET_NAME + (i << 1), ((uint16_t *)nameBuff)[i]);
	_fs_prgm_off();
}

/**
 * Closes the specified file descriptor.
 *
 * @param fd the descriptor to close
 */
void fclose(PROS_FILE *fd) {
	uint32_t idx = (uint32_t)fd - FILE_CHANNEL_1;
	if (idx < MAX_FILES) {
		// Index in bounds
		uint16_t flags = fs.files[idx].flags;
		if (flags & FILE_FLAG_RD)
			// Just close the file
			fs.files[idx].flags = 0U;
		else if (flags & FILE_FLAG_WR) {
			uint32_t page = fs.files[idx].page, len = fs.files[idx].len, offs;
			// Round length up to the next even # if necessary
			if (len & 0x01)
				fsWrite(fd, (char)0xFF);
			// Flush last page, close the file
			fflush(fd);
			offs = fs.files[idx].offset;
			if (offs < len) len = offs;
			// Write the final length to the file
			_fs_prgm_on();
			_fs_prgm_word(page, FILE_OFFSET_LEN, len & 0xFFFFU);
			_fs_prgm_word(page, FILE_OFFSET_LEN + 2, (len >> 16));
			_fs_prgm_off();
			// Close the file
			fs.files[idx].flags = 0U;
		}
	}
}

/**
 * Delete the specified file if it exists and is not currently open.
 *
 * The file will actually be erased from memory on the next re-boot.
 *
 * @param file the file name to erase
 * @return 0 if the file was deleted, or 1 if the file could not be found
 */
int fdelete(const char *file) {
	uint16_t len, page = lookForFile(file);
	uint32_t i;
	uint8_t *flash;
	if (page < NUM_PAGES) {
		// Already open?
		for (i = 0; i < MAX_FILES; i++)
			if (fs.files[i].flags != 0U && fs.files[i].page == page)
				return 1;
		// Found index, jump on it
		flash = (uint8_t *)_fs_pointer(page);
		len = (GET_UINT16(flash, FILE_OFFSET_LEN) + 15) / PAGE_SIZE;
		_fs_prgm_on();
		// Mark the pages as needing TRIM
		_fs_prgm_word(page, FILE_OFFSET_STATUS, 0xA50A);
		_fs_prgm_off();
		fs.page[page++] = FLAG_FILE_DELETED;
		for (; len && page < NUM_PAGES; len--) {
			// Mark the rest of the file invalid for this run
			fs.page[page++] = FLAG_FILE_DELETED | FLAG_FILE_CONTINUE;
		}
		return 0;
	}
	return 1;
}

/**
 * Checks to see if the specified file is at its end. Only works for files in Read mode;
 * for files in any other mode, 1 is always returned.
 *
 * @param fd the channel to check
 * @return 0 if the file is not at EOF, or 1 otherwise.
 */
int fsEof(PROS_FILE *fd) {
	uint32_t idx = (uint32_t)fd - FILE_CHANNEL_1;
	if (idx < MAX_FILES) {
		// Index in bounds
		uint16_t flags = fs.files[idx].flags;
		if (flags & FILE_FLAG_RD)
			return (fs.files[idx].offset >= fs.files[idx].len) ? 1 : 0;
		// Else, can't determine
	}
	return 1;
}

/**
 * Flushes the data on the specified file channel.
 *
 * @param fd the channel to flush
 * @return 0 if the data was successfully written out to Flash, EOF otherwise
 *     The file data is still invalid until properly closed!
 */
int fflush(PROS_FILE *fd) {
	uint32_t idx = (uint32_t)fd - FILE_CHANNEL_1;
	if (idx < MAX_FILES) {
		// Index in bounds
		uint16_t flags = fs.files[idx].flags;
		if (flags & FILE_FLAG_WR) {
			uint16_t *dp = (uint16_t *)(&fs.data[0]);
			// Calculate current position in flash memory
			uint32_t offs = fs.files[idx].offset, len = fs.files[idx].len;
			uint32_t page = (offs + 16U) / PAGE_SIZE;
			uint32_t remain = (len - offs) & 0xFFFFFFFEU;
			offs = (offs + 16U) - page * PAGE_SIZE;
			page += fs.files[idx].page;
			// Program the buffer to the current location; if the buffer has an odd size,
			// leave one byte behind
			_fs_prgm_on();
			for (; remain; remain -= 2) {
				if (offs == 0) {
					if (page >= NUM_PAGES || fs.page[page] != 0U) {
						// No space to write the rest of the buffer
						fs.files[idx].offset = (len - remain) & 0xFFFFFFFEU;
						_fs_prgm_off();
						return EOF;
					} else
						// Update the page table
						fs.page[page] = FLAG_FILE_PRESENT | FLAG_FILE_CONTINUE;
				}
				_fs_prgm_word(page, offs, *dp++);
				offs += 2;
				// Wrap around to the next page
				if (offs >= PAGE_SIZE) {
					page++;
					offs = 0;
				}
			}
			_fs_prgm_off();
			// Reset data size to just the odd part
			fs.files[idx].offset = len & 0xFFFFFFFEU;
			return 0;
		} else if (flags & FILE_FLAG_RD)
			// No-op
			return 0;
	}
	return EOF;
}

/**
 * Returns the number of bytes left in the file until EOF. Only applies to files open for
 * Read mode.
 *
 * @param fd the file channel to check
 * @return the number of bytes left, or 0 if this value could not be determined
 */
uint32_t fsLeft(PROS_FILE *fd) {
	uint32_t idx = (uint32_t)fd - FILE_CHANNEL_1;
	if (idx < MAX_FILES) {
		// Index in bounds
		uint16_t flags = fs.files[idx].flags;
		if (flags & FILE_FLAG_RD)
			return fs.files[idx].len - fs.files[idx].offset;
		// Else, can't determine
	}
	return 0;
}

/**
 * fopen() function for file system
 *
 * @param file the file name
 * @param mode the file mode
 * @return a pointer to file data, or NULL if the file could not be opened
 */
PROS_FILE * fopen(const char *file, const char *mode) {
	uint32_t i, page;
	if (strcmp(mode, "r") == 0) {
		// READ
		page = lookForFile(file);
		if (page < NUM_PAGES) {
			uint8_t *flash = (uint8_t *)_fs_pointer(page);
			uint32_t len = GET_UINT32(flash, FILE_OFFSET_LEN);
			// Look for an open file with flags == 0
			for (i = 0; i < MAX_FILES; i++)
				if (fs.files[i].flags == 0U) {
					// Mine mine mine!
					openForReading(i, page, len);
					return (PROS_FILE *)(FILE_CHANNEL_1 + i);
				}
			// Maximum file limit exceeded!
		}
	} else if (strcmp(mode, "w") == 0) {
		// WRITE - no append possible due to the structure of the Flash
		// Erases the file's contents!
		fdelete(file);
		// Look for the largest contiguous block of free space
		page = longestBlock();
		if (fs.page[page] == 0U) {
			// Allocate FD
			for (i = 0; i < MAX_FILES; i++) {
				uint16_t flags = fs.files[i].flags;
				if (flags == 0U) {
					// Mine mine mine!
					openForWriting(file, i, page);
					return (PROS_FILE *)(FILE_CHANNEL_1 + i);
				} else if (flags & FILE_FLAG_WR)
					// Only one write buffer
					break;
			}
		}
	}
	return NULL;
}

/**
 * fread() function for file system
 *
 * @param fd the channel to read
 * @return the next byte in the file, or EOF if no byte can be read (end of file, in write
 * mode...)
 */
int fsRead(PROS_FILE *fd) {
	uint32_t idx = (uint32_t)fd - FILE_CHANNEL_1;
	if (idx < MAX_FILES) {
		if (fs.files[idx].flags & FILE_FLAG_RD) {
			// Advance offset, return char
			uint32_t offs = fs.files[idx].offset;
			if (offs < fs.files[idx].len) {
				uint8_t value;
				// Calculate position in flash memory
				uint32_t page = (offs + 16U) / PAGE_SIZE;
				fs.files[idx].offset = offs + 1;
				offs = (offs + 16U) - page * PAGE_SIZE;
				value = ((uint8_t *)_fs_pointer(fs.files[idx].page + page))[offs];
				// No sign extension will occur
				return (int)(uint32_t)value;
			}
		}
	}
	// Match stdio
	return EOF;
}

/**
 * Scans the file system and builds a RAM structure describing the files
 * currently on the file system and the free space available.
 *
 * @param trim true if TRIM should occur, false otherwise
 */
void fsScan(bool trim) {
	// Walk each page looking for headers
	uint32_t i, len;
	uint16_t magic, status;
	uint8_t flags;
	for (i = 0; i < NUM_PAGES; i++) {
		uint8_t *page = (uint8_t *)_fs_pointer(i);
		magic = GET_UINT16(page, FILE_OFFSET_MAGIC);
		// Grab the file length
		len = GET_UINT32(page, FILE_OFFSET_LEN);
		if (magic == FILE_MAGIC) {
			// Start of a valid file, but possibly erased and needing TRIM
			status = GET_UINT16(page, FILE_OFFSET_STATUS);
			// Sanity check, the file will be erased it its length makes no sense
			if (len >= PAGE_SIZE * NUM_PAGES) {
				len = 1U;
				status = 0;
			}
			// status = 0xFFFF when created, anything else means TRIMmed
			if (status == 0xFFFF) {
				fs.page[i] = FLAG_FILE_PRESENT;
				flags = FLAG_FILE_PRESENT | FLAG_FILE_CONTINUE;
			} else if (!trim) {
				// TRIM disabled
				fs.page[i] = FLAG_FILE_NOUSE;
				flags = FLAG_FILE_NOUSE;
			} else {
				fs.page[i] = 0x00;
				flags = 0x00;
				// True trim of page
				_fs_prgm_on();
				_fs_erase_page(i);
			}
			// Skip a number of pages corresponding to the length + header
			len = (len + 15) / PAGE_SIZE;
			while (i < NUM_PAGES && len != 0) {
				i++;
				if (!flags)
					// Continue to TRIM the old file
					_fs_erase_page(i);
				fs.page[i] = flags;
				len--;
			}
			// Shut off programming voltage if it was TRIMming
			if (!flags)
				_fs_prgm_off();
		} else if (magic == 0xFFFF && len == 0xFFFFFFFF)
			// Page is (likely!?) blank
			fs.page[i] = 0x00;
		else
			// Page cannot be used (probably has code)
			fs.page[i] = FLAG_FILE_NOUSE;
	}
	// Clear and close all open files
	for (i = 0; i < MAX_FILES; i++)
		fs.files[i].flags = 0U;
}

/**
 * fseek() function for file system
 *
 * @param fd the channel to modify
 * @param offset the location within the stream to seek
 * @param origin the reference location for offset: SEEK_CUR, SEEK_SET, or SEEK_END
 * @return 0 if the seek was successful, or 1 otherwise
 */
int fseek(PROS_FILE *fd, long int offset, int origin) {
	uint32_t idx = (uint32_t)fd - FILE_CHANNEL_1;
	if (idx < MAX_FILES) {
		if (fs.files[idx].flags & FILE_FLAG_RD) {
			// Bounds check
			uint32_t len = fs.files[idx].len;
			uint32_t offs;
			if (origin == SEEK_SET)
				// From beginning
				offs = (uint32_t)offset;
			else if (origin == SEEK_CUR)
				// At current position
				offs = (uint32_t)((long int)fs.files[idx].offset + offset);
			else if (origin == SEEK_END)
				// From end
				offs = (uint32_t)((long int)len + offset);
			else
				// Bad argument
				return 1;
			if (offs >= 0 && offs < len) {
				// Set new offset
				fs.files[idx].offset = offset;
				return 0;
			}
		}
	}
	return 1;
}

/**
 * ftell() function for file system
 *
 * @param fd the channel to check
 * @return the offset of the stream, or -1 if the offset could not be determined
 */
long int ftell(PROS_FILE *fd) {
	uint32_t idx = (uint32_t)fd - FILE_CHANNEL_1;
	if (idx < MAX_FILES) {
		if (fs.files[idx].flags != 0)
			// Use current position
			return (long int)fs.files[idx].offset;
	}
	return -1L;
}

/**
 * fwrite() function for file system
 *
 * @param fd the channel to write
 * @param value the byte to write
 * @return value if the byte was successfully written and EOF otherwise
 */
int fsWrite(PROS_FILE *fd, int value) {
	uint32_t idx = (uint32_t)fd - FILE_CHANNEL_1;
	if (idx < MAX_FILES) {
		if (fs.files[idx].flags & FILE_FLAG_WR) {
			// Advance offset, store char in buffer
			uint32_t offs = fs.files[idx].offset, len = fs.files[idx].len;
			if (len >= offs + PAGE_SIZE) {
				// Flush the buffer
				if (fflush(fd) != 0) return EOF;
				offs = fs.files[idx].offset;
			}
			// Write the byte to the buffer
			fs.data[len - offs] = (uint8_t) value;
			fs.files[idx].len = len + 1;
			return value;
		}
	}
	return EOF;
}

#else

// Stub versions of important functions

void fclose(PROS_FILE *fd) {
}

int fdelete(const char *file) {
	return 1;
}

int fflush(PROS_FILE *fd) {
	return EOF;
}

PROS_FILE * fopen(const char *file, const char *mode) {
	return NULL;
}

int fseek(PROS_FILE *fd, long int offset, int origin) {
	return 1;
}

long int ftell(PROS_FILE *fd) {
	return -1L;
}

#endif
