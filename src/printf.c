/**
 * Optimized printf/sprintf/snprintf for ARM platforms (works on x86 too)
 * Fully reentrant and ISR safe, no dynamic memory allocation on the heap
 * The exact semantics of some format strings differ from the industry standard
 * printf() family to speed up operation. See the API documentation for details
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
#include <stdarg.h>
#include <limits.h>
#include <string.h>

#ifdef PRINTF_FLOAT
#include <math.h>

// 2 digit constant character lookup table
static const char * two_digits[100] = {
	"00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
	"10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
	"20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
	"30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
	"40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
	"50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
	"60", "61", "62", "63", "64", "65", "66", "67", "68", "69",
	"70", "71", "72", "73", "74", "75", "76", "77", "78", "79",
	"80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
	"90", "91", "92", "93", "94", "95", "96", "97", "98", "99"
};
#endif

// Lots of data goes into the padding flags
// Left-align
#define PAD_RIGHT 1
// Zero pad
#define PAD_ZERO 2
// Display as signed (not applicable for double)
#define PAD_SIGNED 4
// Always display leading sign
#define PAD_LEADING_PLUS 8
// Number is negative (internal for printi)
#define PAD_NEGATIVE 16
// After decimal point (internal for format)
#define PAD_AFTER_DECIMAL 32

#define IS_DIGIT(x) ((x) >= '0' && (x) <= '9')
#define PRINT_BUF_LEN 12

// Data for snprintf() and sprintf()
typedef struct {
	char *buffer;
	uint32_t count;
} _sPrintfData;

// Data for fprintf()
typedef struct {
	PROS_FILE *stream;
	uint32_t count;
} _fPrintfData;

// Prints a character to debug output
static void _printToStream(void* in, char c) {
	_fPrintfData *data = (_fPrintfData*)in;
	if (c) {
		fputc((int)c, data->stream);
		data->count++;
	}
}

// Prints a character to an output buffer
static void _printToBuffer(void* in, char c) {
	_sPrintfData *data = (_sPrintfData*)in;
	*(data->buffer)++ = c;
	if (c)
		data->count++;
}

// Prints a character to an output buffer with limit
static void _printNToBuffer(void* in, char c) {
	_sPrintfData *data = (_sPrintfData*)in;
	if (!c)
		*(data->buffer)++ = '\0';
	else if (data->count) {
		*(data->buffer)++ = c;
		data->count--;
	}
}

// Quick and easy string length method
static uint32_t _strlen(char *str) {
	uint32_t slen = 0;
	if (str != NULL)
		while (*str != 0) {
			slen++;
			str++;
		}
	return slen;
}

// Prints a string with optional zero left/right padding to at least the required width
static void prints(void (*outputFn)(void*, char), void *data, const char *string,
		uint32_t width, uint32_t pad) {
	char padchar = ' ';
	if (width > 0) {
		uint32_t len = _strlen((char*)string);
		// If length greater than field width, print all
		if (len >= width)
			width = 0;
		else
			width -= len;
		// Zero-pad?
		if (pad & PAD_ZERO)
			padchar = '0';
	}
	// If aligning to the right, pad leading
	if (!(pad & PAD_RIGHT))
		for (; width > 0; width--)
			outputFn(data, padchar);
	// Output data
	for (; *string; ++string)
		outputFn(data, *string);
	// Output trailing pad if aligning left
	for (; width > 0; --width)
		outputFn(data, padchar);
}

// Prints an integer in the specified numeric base
static void printi(void (*outputFn)(void*, char), void *data, int32_t num, uint32_t base,
		uint32_t width, uint32_t pad) {
	char print_buf[PRINT_BUF_LEN], *s;
	uint32_t remain, value = (uint32_t)num;
	if ((pad & PAD_SIGNED) && num < 0) {
		pad |= PAD_NEGATIVE;
		value = (uint32_t)(-num);
	}
	// Make sure print_buf is NULL-term
	s = print_buf + PRINT_BUF_LEN - 1;
	*s = '\0';
	do {
		remain = value % base;
		// Divide down to next place
		value /= base;
		// Store value
		if (remain >= 10)
			remain += ('A' - 10);
		else
			remain += '0';
		*--s = remain;
	} while (value);
	if (pad & PAD_NEGATIVE) {
		// Re-add the negative sign
		if (width && (pad & PAD_ZERO)) {
			outputFn(data, '-');
			width--;
		} else
			*--s = '-';
	} else if (pad & PAD_LEADING_PLUS) {
		if (width && (pad & PAD_ZERO)) {
			outputFn(data, '+');
			width--;
		} else
			*--s = '+';
	}
	prints(outputFn, data, s, width, pad);
}

#ifdef PRINTF_FLOAT
// divmod10 alg from: http://forum.arduino.cc/index.php?topic=167414.0
void _divmod10(uint64_t in, uint64_t* div, uint64_t* mod)
{
    // q = in * 0.8;
    uint64_t q = (in >> 1) + (in >> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);  // not needed for 16 bit version
    q = q + (q >> 32);

    // q = q / 8;  ==> q =  in *0.1;
    q = q >> 3;

    // determine error
    uint64_t  r = in - ((q << 3) + (q << 1));   // r = in - q*10;
    *div = q + (r > 9);
    if (r > 9) *mod = r - 10;
    else *mod = r;
}

inline static void _printd_handle_large_exp(void (*outputFn)(void*, char), void* data,
    uint64_t mantissa, int64_t exponent)
{
    uint16_t ffp[64];

    // Sets the entire ffp array to zero
    memset(ffp, 0, 64);

    uint64_t integer_val = mantissa;
    uint32_t j = 0;

    // Boil the exponent down to less than 16
    uint32_t q = exponent - 52;

    while (q >= 16U) {
        j++;
        q -= 16U;
    }

    uint32_t p = 0;

    // Force it into position, the double shift allows optimization with one word
    if (q > 11U) {
        p = (uint32_t)((integer_val>> 32U) >> (32U - q));
    }

    integer_val <<= q;

    int i = j;
    j += 4U;

    // Load directly, this is pretty cheap
    for (; i < j; i++) {
        ffp[i] = (uint16_t)(integer_val & 0xFFFFULL);
        integer_val >>= 16;
    }

    // Extended precision overlay
    if (p) {
        ffp[i] = (uint16_t)p;
        j++;
    }

    uint16_t partials[80];

    // Rolldown loop
    p = 0U;
    do {
        uint32_t q = 0U;
        int i = j;
        do {
            i--;
            // Accumulate the last q
            q = (q << 16) + (uint32_t)ffp[i];

            uint32_t d = q / 10000U;

            // If we erased the top, move it down one
            if (d == 0 && i == j-1)
                j--;

            ffp[i] = (uint16_t)d;
            q -= d * 10000U;
        } while (i);

        // Store partial
        partials[p++] = (uint16_t)q;

    } while (j);

    // Write partials high to low
    while (p) {
        const char * str;
        p--;

        uint32_t q = partials[p];
        uint32_t d = q / 100U;

        // First 2 digits
        str = two_digits[d];
        if (j || d > 9)
            outputFn(data, str[0]);
        if (j || d)
            outputFn(data, str[1]);

        // Second 2 digits
        q -= 100 * d;
        str = two_digits[q];

        if (j || d || q > 9)
            outputFn(data, str[0]);

        outputFn(data, str[1]);

        j++;
    }
}

static void _printd_get_floating_values(double value,
    uint64_t* mantissa, int32_t* exponent, int8_t* negative)
{
    // Union is used to convert from double to uint64_t
    union {
		double dval;
		uint64_t ival;
	} input;

	// Tear down into parts (theoretically should be 0 cycles if optimized)
	input.dval = value;
	uint64_t entire_float = input.ival;

	// These operations are cheap and one time
	if (entire_float & 0x8000000000000000ULL)
        *negative = 1;
    else
        *negative = 0;

    // Split the exponent off
	*exponent = (int16_t)((entire_float & 0x7FF0000000000000ULL) >> 52) - 1023;
	*mantissa = (entire_float & 0x000FFFFFFFFFFFFFULL) | 0x0010000000000000ULL;
}

// Returns 0 if there was NO rounding over an integer line. (Ie, 0.99 -> 1.00)
// Returns 1 if was rounding
inline static int _printd_get_decimals(char* output,
    uint64_t mantissa, int64_t exponent, int widthAfter)
{
    uint64_t sum = 0;
    uint64_t decimal = 0;

    // Split off decimal part from the mantissa
    // Once again, these instructions are cheap and one-time
    if (exponent >= 52 || exponent <= -76)
        // Double variables are purely integer if in this range
        decimal = 0;
    else if (exponent >= -12)
        decimal = mantissa << (uint32_t)(12 + exponent);
    else if (exponent > -76)
        decimal = mantissa >> (uint32_t)(-exponent - 12);

    // Get the decimal value
    uint64_t decimal_val = 500000000000000000ULL;

    for (int i = 0; i < 52 && decimal != 0ULL; i++) {
        if (decimal & 0x8000000000000000ULL)
            sum += decimal_val;
        // Everything in this loop is cheap: shift is 2 cycles per (lsr and ror)
        // the if is 2 ("tst x, #0x80000000", "itt ne"), the potential addition is 2
        // ("addne x, x, y" "adcne x, x, y")
        decimal_val >>= 1;
        decimal <<= 1;
    }

    // Dividing by ten using 64 bits is very very ugly, but we can do better
    for (int i = 0; i < 18; i++) {

        // Rounding correctly
        if (i == 15 - widthAfter)
        {
            sum += 500ULL;
        }

        uint64_t _none_;
        _divmod10(sum, &decimal_val, &_none_);

        // Multiply 64 bits is bad but not horrible
        output[i] = '0' + (char)(sum - decimal_val * 10ULL);
        sum = decimal_val;
    }

    return decimal_val;
}

inline static void _printd_handle_small_exp(void (*outputFn)(void*, char), void* data,
    uint64_t mantissa, int64_t exponent, int roundingOverInt, int totalWidth, uint32_t pad)
{
    // Shift until the decimal portion of the mantissa are removed from
    // the floating point number
    // Ex. If the floating value is 123.456, integer_val = 123
    uint64_t integer_val = mantissa >> (uint32_t)(52U - exponent);

    // This is for the case were the decimal rounded over an integer.
    // For instance, 1.9999 rounded to 2.0000, so "1" must be incremented to "2"
    if (roundingOverInt > 0)
        integer_val++;

    int index = 0;
    char output[20];

    // Saves the integer to a character array.
    // Ie. 123 will be saved as '3', '2', 1', ...
    do {
        uint64_t _mod;
        _divmod10(integer_val, &integer_val, &_mod);

        output[index++] = _mod + '0';
    } while (integer_val);

    // Determine pad-before character
    char pc = (pad & PAD_ZERO) ? '0' : ' ';

    // Insert padding characters
    for (int i = index; i < totalWidth; i++)
        outputFn(data, pc);

    // Since the data is saved in the char array backwards,
    // print it out to the screen in reverse order.
    for (index = index - 1; index >= 0; index--)
        outputFn(data, output[index]);
}

// Prints a floating point number with the given precision
static void printd(void (*outputFn)(void*, char), void *data, double value,
		uint32_t widthTotal, uint32_t widthAfter, uint32_t pad) {

	int32_t exponent;
	uint64_t mantissa;
	int8_t negative;

 	_printd_get_floating_values(value, &mantissa, &exponent, &negative);

	if (negative){
		outputFn(data, '-');
		widthTotal--;
	} else if (pad & PAD_LEADING_PLUS) {
		outputFn(data, '+');
		widthTotal--;
	}

	if (exponent == (int16_t)1024) {
		const char *inf;
		// Handle Inf and Nan
		if (mantissa == 0x0)
			inf = "Inf";
		else
			inf = "NaN";

		prints(outputFn, data, inf, widthTotal, ' ');

        return;
	}

	// Get decimal points values.  If there was some rounding over
	// an integer value, it will be dealt in the handle_small_exp value.
	char decimal_output[20];
	int rounding_over_int =
			_printd_get_decimals(decimal_output, mantissa, exponent, widthAfter);

	// Adjust the total width to be equal to width not used by decimals and period
	widthTotal -= (widthAfter + 1);

	if (widthTotal > 308) widthTotal = 0;

	// Print leading decimals if the floating point is greater then 1
    // Otherwise print a zero
    if (exponent <= 52) {
      //uint32_t totalIntegerWidth = widthTotal - widthAfter - 1;

      _printd_handle_small_exp(outputFn,
          data, mantissa, exponent,
          rounding_over_int, widthTotal, pad);
    }
    // An exponent of greater then 52 means there is no decimal point for this number
    // This must be handled differently then a simple bitshift TODO: why?
    else {
      _printd_handle_large_exp(outputFn, data, mantissa, exponent);
    }

	// Round to nearest 15 decimals, if we divide by 1000 we have 15 good places
	outputFn(data, '.');

	// Output the decimal part
	for (int i = 0; i < widthAfter; i++)
			outputFn(data, decimal_output[17 - i]);
}
#endif

// Formats text to the specified output buffer, or to the debug stream if out is NULL
static void format(void (*outputFn)(void*, char), void *data, const char *fmt, va_list args) {
	uint32_t widthBefore, widthAfter, pad;
	// Used in some cases
	const char *s;
#ifdef PRINTF_FLOAT
	double dval;
#endif
	for (; *fmt; fmt++) {
		if (*fmt == '%') {
			// Default widths
			widthBefore = pad = 0;
			widthAfter = 6;
			fmt++;
			if (*fmt == '\0')
				break;
			if (*fmt == '%')
				// %%
				outputFn(data, *fmt);
			else {
				// Left-align
				if (*fmt == '-') {
					fmt++;
					pad = PAD_RIGHT;
				}
				// Always add sign
				if (*fmt == '+') {
					fmt++;
					pad |= PAD_LEADING_PLUS;
				}
				// Zero pad
				if (*fmt == '0') {
					fmt++;
					pad |= PAD_ZERO;
				}
				// Determine number of digits before and after decimal point
				while (*fmt == '.' || IS_DIGIT(*fmt))
					if (*fmt == '.') {
						pad |= PAD_AFTER_DECIMAL;
						widthAfter = 0;
						fmt++;
					} else {
						// Compile widths before and after decimal point
						if (pad & PAD_AFTER_DECIMAL)
							widthAfter = (widthAfter * 10) + (*fmt - '0');
						else
							widthBefore = (10 * widthBefore) + (*fmt - '0');
						fmt++;
					}
				// Long-double and long-decimal is implied on a 32-bit platform
				if (*fmt == 'l')
					fmt++;
				switch (*fmt) {
				case 's':
					// String
					s = (const char*)va_arg(args, char*);
					prints(outputFn, data, s ? s : "NULL", widthBefore, pad);
					break;
				case 'd':
					// Signed decimal
					printi(outputFn, data, va_arg(args, int), 10, widthBefore, pad | PAD_SIGNED);
					break;
				case 'u':
					// Unsigned decimal
					printi(outputFn, data, va_arg(args, unsigned int), 10, widthBefore, pad);
					break;
				case 'x':
				case 'X':
					// Hexadecimal (unfortunately, always uppercase)
					printi(outputFn, data, va_arg(args, unsigned int), 16, widthBefore, pad);
					break;
				case 'p':
					// Pointer address
					printi(outputFn, data, va_arg(args, unsigned int), 16, 8, pad | PAD_ZERO);
					break;
				case 'c':
					// Character
					outputFn(data, (char)va_arg(args, int));
					break;
#ifdef PRINTF_FLOAT
				case 'f':
					// Floating point double
					dval = va_arg(args, double);
					printd(outputFn, data, dval, widthBefore, widthAfter, pad);
					break;
#else
				case 'f':
					// Floating point ignore to avoid breaking %f format strings and faulting
					(void)(va_arg(args, uint64_t));
					break;
#endif
				case '\0':
					// End of string
					break;
				default:
					// Invalid, print character
					outputFn(data, '%');
					outputFn(data, *fmt);
					break;
				}
			}
		} else
			outputFn(data, *fmt);
	}
	// Zero-terminate
	outputFn(data, '\0');
}

// The next few functions do exactly as one might believe they should as declared in <stdio.h>

void fprint(const char *str, PROS_FILE *stream) {
	char c;
	while ((c = *str++))
		fputc(c, stream);
}

void print(const char *str) {
	fprint(str, stdout);
}

int fputs(const char *str, PROS_FILE *stream) {
	char c;
	uint32_t count = 0;
	while ((c = *str++)) {
		fputc((int)c, stream);
		count++;
	}
	fputc('\n', stream);
	// Required for library compatibility
	return (int)count;
}

int puts(const char *str) {
	return fputs(str, stdout);
}

int vfprintf(PROS_FILE *stream, const char *fmt, va_list args) {
	_fPrintfData data;
	data.stream = stream;
	data.count = 0;
	format(_printToStream, (void*)(&data), fmt, args);
	// Required for library compatibility
	return (int)data.count;
}

int vsprintf(char *out, const char *fmt, va_list args) {
	_sPrintfData data;
	data.buffer = out;
	data.count = 0;
	format(_printToBuffer, (void*)(&data), fmt, args);
	// Required for library compatibility
	return (int)data.count;
}

int vsnprintf(char *out, size_t size, const char *fmt, va_list args) {
	int ret = 0;
	_sPrintfData data;
	data.buffer = out;
	// Allow room for null terminator
	if (size > 0) {
		size--;
		data.count = (uint32_t)size;
		// Leave room for null terminator at end
		format(_printNToBuffer, (void*)(&data), fmt, args);
		// Required for library compatibility
		ret = (int)(size - data.count);
	}
	return ret;
}

int fprintf(PROS_FILE *stream, const char *fmt, ...) {
	va_list args;
	int count;
	va_start(args, fmt);
	count = vfprintf(stream, fmt, args);
	va_end(args);
	return count;
}

int printf(const char *fmt, ...) {
	va_list args;
	int count;
	va_start(args, fmt);
	count = vfprintf(stdout, fmt, args);
	va_end(args);
	return count;
}

int sprintf(char *out, const char *fmt, ...) {
	int count;
	va_list args;
	va_start(args, fmt);
	count = vsprintf(out, fmt, args);
	va_end(args);
	return (int)count;
}

int snprintf(char *out, size_t size, const char *fmt, ...) {
	int count;
	va_list args;
	va_start(args, fmt);
	count = vsnprintf(out, size, fmt, args);
	va_end(args);
	return count;
}
