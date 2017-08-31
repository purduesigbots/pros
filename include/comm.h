/*
 * comm.h - Definitions for character I/O and communications functions
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

#ifndef COMM_H_
#define COMM_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

// Begin C++ extern to C
#ifdef __cplusplus
extern "C" {
#endif

#define PRINTF_FLOAT

// EOF constant
#ifndef EOF
#define EOF ((int)-1)
#endif

// ---- "Standard" I/O definitions ----
// PROS_FILE is an integer referring to a stream; we use (invalid) pointers for compability
typedef int PROS_FILE;
// Standard input and output streams use the PC debug terminal
#define stdout ((PROS_FILE *)3)
#define stdin ((PROS_FILE *)3)
// UART 1 on the Cortex
#define uart1 ((PROS_FILE *)1)
// UART 2 on the Cortex
#define uart2 ((PROS_FILE *)2)
// Bonus UART port on the crystal output (TX only)
#define uart3 ((PROS_FILE *)4)

// ---- Simple I/O routines ----
// fcount - Return number of characters available to read on the specified stream
int fcount(PROS_FILE *stream);
// feof - Return 1 if the stream is at EOF, or 0 otherwise
int feof(PROS_FILE *fd);
// fgetc - Reads and returns one character from the specified stream, blocking until complete
int fgetc(PROS_FILE *stream);
// fgets - Read a string from the specified stream
char* fgets(char *str, int num, PROS_FILE *stream);
// fread - Read data from stream
size_t fread(void *ptr, size_t size, size_t count, PROS_FILE *stream);
// fwrite - Write data to stream
size_t fwrite(const void *ptr, size_t size, size_t count, PROS_FILE *stream);
// getchar - Reads and returns one character from "stdin"
int getchar();
// fputc - Writes one character to the specified stream and returns the input value
int fputc(int value, PROS_FILE *stream);
// putchar - Writes one character to "stdout" and returns the input value
int putchar(int value);
// print - Prints the simple string to debug terminal
void print(const char *string);
// puts - Same as "print" function, with trailing newline
int puts(const char *string);
// fprint - Prints the simple string to the specified stream
void fprint(const char *string, PROS_FILE *stream);
// fputs - Same as "fprint" function, with trailing newline
int fputs(const char *string, PROS_FILE *stream);

// ---- Formatted I/O routines ----
// WARNING: Use of the this family of functions requires at least 48 variables
// (NOT 64 bytes!) of space on the stack. Tasks using these probably should have a stack size of
// at least 128 variables, if not 256 or more.

// printf - Prints the formatted string to the debug stream (the PC terminal)
int printf(const char *formatString, ...);
// fprintf - Prints the formatted string to the specified output stream
int fprintf(PROS_FILE *stream, const char *formatString, ...);
// sprintf - Prints the formatted string to the string buffer, which must be big enough
int sprintf(char *buffer, const char *formatString, ...);
// snprintf - Prints the formatted string to the string buffer with the specified length limit
// The length limit, as per the C standard, includes the trailing null character
int snprintf(char *buffer, size_t limit, const char *formatString, ...);

// Variadic macro variations of the above, which are only useful in a handful of cases
// vfprintf - Prints the formatted string to the specified output stream
int vfprintf(PROS_FILE *stream, const char *formatString, va_list arguments);
// vsprintf - Prints the formatted string to a string buffer
int vsprintf(char *buffer, const char *formatString, va_list arguments);
// vsnprintf - Prints the formatted string to a string buffer with the given length limit
int vsnprintf(char *buffer, size_t limit, const char *formatString, va_list arguments);

// ---- USART I/O routines ----
// usartBufferInit - Initializes (empty) the USART buffers
void usartBufferInit();
// usartFlushBuffers - Clears the USART buffers
void usartFlushBuffers();
// usartInit - Initialize the specified USART interface with the given connection parameters
void usartInit(PROS_FILE *usart, unsigned int baud, unsigned int flags);
// usartShutdown - Disable the specified USART interface
void usartShutdown(PROS_FILE *usart);

// ---- LCD I/O routines ----
// lcdClear - Clears the LCD screen on the specified port
void lcdClear(PROS_FILE *lcdPort);
// lcdInit - Enables the LCD on the specified port
void lcdInit(PROS_FILE *lcdPort);
// lcdPrint - Convenience method that performs snprintf() and then lcdSetText()
void __attribute__ ((format (printf, 3, 4)))
	lcdPrint(PROS_FILE *lcdPort, unsigned char line, const char *fmt, ...);
// lcdReadButtons - Reads the button status from the LCD display and returns the buttons
// pressed as a bit mask
unsigned int lcdReadButtons(PROS_FILE *lcdPort);
// lcdSetBacklight - Turns the specified LCD backlight on or off
// The backlight will not update until the next line is sent (maybe 15ms latency)
void lcdSetBacklight(PROS_FILE *lcdPort, bool backlight);
// lcdSetText - Sets a line (1 or 2) of text on the LCD to the specified null-terminated string
void lcdSetText(PROS_FILE *lcdPort, unsigned char line, const char *buffer);
// lcdShutdown - Disable the LCD on the specified port
void lcdShutdown(PROS_FILE *lcdPort);

// End C++ extern to C
#ifdef __cplusplus
}
#endif

#endif
