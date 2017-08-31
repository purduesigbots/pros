/*
 * comm.c - Communications functions for the three USARTs (Debug, UART1, UART2)
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
#include <fs.h>
#include <FreeRTOS.h>
#include <periph.h>
#include <semphr.h>
#include <taskpriv.h>

// LCD backlight on
#define LCD_BACKLIGHT ((uint8_t)0x02)
// LCD active
#define LCD_ACTIVE ((uint8_t)0x04)
// LCD second row send flag
#define LCD_ROW_2 ((uint8_t)0x01)

// Size of buffer, in bytes, for the USART ring buffers
// Can be at most 256 before overflowing the space in a byte; must be a power of two
#define USART_BUFFER_SIZE 0x40
// Maximum value that the size can have before filling
#define _USART_MAX (USART_BUFFER_SIZE - 1)

// Structure containing a ring buffer
typedef struct {
	uint8_t head;
	uint8_t tail;
	char buffer[USART_BUFFER_SIZE];
} RingBuffer_TypeDef;

typedef struct {
	volatile RingBuffer_TypeDef tx;
	volatile RingBuffer_TypeDef rx;
	Semaphore readLock;
} SerialPort_TypeDef;

typedef struct {
	// 2x16 LCD has 32 characters
	char screen[32];
	// Flag byte
	uint8_t flags;
	// Buttons
	uint8_t buttons;
	// Button state machine status
	uint16_t state;
} LCD_TypeDef;

// TX and RX buffers for USART1 (PC communication buffers), USART2 (UART port #1),
// USART3 (UART port #2)
static SerialPort_TypeDef usart[3];

// LCD buffers
static LCD_TypeDef lcd[2];

// Prototype this
uint32_t usartBufferCount(SerialPort_TypeDef* port);

// Checks to see if the ring buffer is empty (head = tail)
static INLINE uint8_t _isBufferEmpty(volatile RingBuffer_TypeDef* buffer) {
	return buffer->head == buffer->tail;
}

// Checks to see if the ring buffer is full (tail + 1 = head)
static INLINE uint8_t _isBufferFull(volatile RingBuffer_TypeDef* buffer) {
	return ((buffer->tail + 1) & _USART_MAX) == buffer->head;
}

// Removes a byte from the head of the given buffer
static char _pullByte(volatile RingBuffer_TypeDef* buffer) {
	uint8_t head = buffer->head; char value;
	value = buffer->buffer[head];
	buffer->head = (head + 1) & _USART_MAX;
	return value;
}

// Queues a byte onto the tail of the given buffer
static void _queueByte(volatile RingBuffer_TypeDef* buffer, char value) {
	uint8_t tail = buffer->tail;
	buffer->buffer[tail] = value;
	buffer->tail = (tail + 1) & _USART_MAX;
}

// Waits for a byte in the given port's RX buffer
// This does not happen during init, so it is OK to use scheduler functions
static void _waitForByte(SerialPort_TypeDef* port) {
	while (_isBufferEmpty(&(port->rx)))
		semaphoreTake(port->readLock, MAX_DELAY);
}

// Waits for space in the given port's TX buffer
// This happens infrequently, so simply sleep-waiting is acceptable
static void _waitForSpace(SerialPort_TypeDef* port) {
	while (_isBufferFull(&(port->tx)))
		_yield();
}

// fcount - Non-standard function to return estimated number of characters waiting on stream
int fcount(PROS_FILE *stream) {
	uint32_t snew = (uint32_t)stream - 1;
	if (snew < 3)
		return (int)usartBufferCount(&usart[snew]);
#ifndef NO_FILESYSTEM
	// File system check
	return fsLeft(stream);
#else
	return 0;
#endif
}

// feof - Return 1 if the stream is at EOF, or 0 otherwise
int feof(PROS_FILE *stream) {
	uint32_t snew = (uint32_t)stream - 1;
	if (snew < 3)
		return (usartBufferCount(&usart[snew]) == 0) ? 1 : 0;
#ifndef NO_FILESYSTEM
	return fsEof(stream);
#else
	return 1;
#endif
}

// fgetc - Read a character from the specified stream
int fgetc(PROS_FILE *stream) {
	uint32_t snew = (uint32_t)stream - 1;
	if (snew < 3) {
		SerialPort_TypeDef *ser = &usart[snew];
		// Wait for byte to arrive
		_waitForByte(ser);
		// Take byte off the tail
		return (int)_pullByte(&(ser->rx)) & 0xFF;
	}
#ifndef NO_FILESYSTEM
	// File system check
	return fsRead(stream);
#else
	return EOF;
#endif
}

// fgets - Read a string from the specified stream
char* fgets(char *str, int num, PROS_FILE *stream) {
	// Required by standard
	char *ptr = str;
	int value;
	if (feof(stream)) return NULL;
	// Read at most num-1 characters
	for (; num > 1 && (value = fgetc(stream)) != EOF; num--) {
		*ptr++ = (char)value;
		// Exit loop (including new line) when a new line is found
		if ((char)value == '\n') break;
	}
	// Add null terminator
	*ptr = '\0';
	return str;
}

// fputc - Write a character to the specified stream
int fputc(int value, PROS_FILE *stream) {
	uint32_t snew = (uint32_t)stream - 1;
	if (snew < 3) {
		SerialPort_TypeDef *ser = &usart[snew];
		_waitForSpace(ser);
		// Jam a byte onto the head
		_queueByte(&(ser->tx), (char)value);
		if (snew == 0)
			// Enable USART2 TXE interrupts
			USART2->CR1 |= USART_CR1_TXEIE;
		else if (snew == 1)
			// Enable USART3 TXE interrupts
			USART3->CR1 |= USART_CR1_TXEIE;
		else if (snew == 2)
			// Enable USART1 TXE interrupts
			USART1->CR1 |= USART_CR1_TXEIE;
	}
#ifndef NO_FILESYSTEM
	else
		// File system check
		return fsWrite(stream, value);
#endif
	return value;
}

// fread - Read data from stream
size_t fread(void *ptr, size_t size, size_t count, PROS_FILE *stream) {
	char *memory = (char *)ptr;
	size_t write = 0; int read;
	for (uint32_t i = 0; i < (size * count); i++)
		// Write as many as we can, break if we fail
		if ((read = fgetc(stream)) != EOF) {
			*memory++ = (char)read;
			write++;
		} else
			break;
	return write;
}

// fwrite - Write data to stream
size_t fwrite(const void *ptr, size_t size, size_t count, PROS_FILE *stream) {
	const char *memory = (const char *)ptr;
	size_t write = 0;
	for (uint32_t i = 0; i < (size * count); i++)
		// Write as many as we can, break if we fail
		if (fputc((int)(*memory++), stream) != EOF)
			write++;
		else
			break;
	return write;
}

// getchar - Gets a character from the serial PC communication buffer; blocks until read
int getchar() {
	return fgetc(stdin);
}

// putchar - Write a character to the serial PC communication buffer
int putchar(int value) {
	return fputc(value, stdout);
}

// ISR_USART1 - Buffered character I/O handler for debug communications
void ISR_USART1() {
	char value;
	bool cs = false;
	SerialPort_TypeDef *ser = &usart[2];
	if (USART1->SR & USART_SR_RXNE) {
		// Read to clear the flag
		value = (char)USART1->DR;
		// Buffer it up, if it's not full to the brim
		if (!_isBufferFull(&(ser->rx)))
			_queueByte(&(ser->rx), value);
		// Notify any receivers
		semaphoreGiveISR(ser->readLock, &cs);
	}
	if (USART1->SR & USART_SR_TXE) {
		if (_isBufferEmpty(&(ser->tx)))
			// Nothing to send, disable interrupt
			USART1->CR1 &= ~USART_CR1_TXEIE;
		else {
			value = _pullByte(&(ser->tx));
			USART1->DR = value;
		}
	}
	if (cs)
		_taskYield();
}

// lcdButtonProcess - Processes the VEX LCD buttons
static void lcdButtonProcess(char value, uint32_t index) {
	uint16_t count = lcd[index].state;
	// If 0xAA, set count to 1
	// If 0x55, set count to 2 if count == 1
	// If 0x16, set count to 3 if count == 2
	if (value == (char)0xAA)
		count = 1;
	else if (value == (char)0x55 && count == 1)
		count = 2;
	else if (value == (char)0x16 && count == 2)
		count = 3;
	else if (value == (char)0x02 && count == 3)
		// Size must be 2
		count = 4;
	else if (count == 4) {
		lcd[index].buttons = (uint8_t)value;
		count = 0;
	} else
		// Ignore the checksum and any noise we pick up
		count = 0;
	lcd[index].state = count;
}

// ISR_USART2 - Buffered character I/O handler for UART port 1
void ISR_USART2() {
	char value;
	bool cs = false;
	SerialPort_TypeDef *ser = &usart[0];
	if (USART2->SR & USART_SR_RXNE) {
		// Read to clear the flag
		value = (char)USART2->DR;
		if (lcd[0].flags & LCD_ACTIVE)
			lcdButtonProcess(value, 0);
		else {
			// Buffer it up, if it's not full to the brim
			if (!_isBufferFull(&(ser->rx)))
				_queueByte(&(ser->rx), value);
			// Notify any receivers
			semaphoreGiveISR(ser->readLock, &cs);
		}
	}
	if (USART2->SR & USART_SR_TXE) {
		if (_isBufferEmpty(&(ser->tx)))
			// Nothing to send, disable interrupt
			USART2->CR1 &= ~USART_CR1_TXEIE;
		else {
			value = _pullByte(&(ser->tx));
			USART2->DR = value;
		}
	}
	if (cs)
		_taskYield();
}

// ISR_USART3 - Buffered character I/O handler for UART port 2
void ISR_USART3() {
	char value;
	bool cs = false;
	SerialPort_TypeDef *ser = &usart[1];
	if (USART3->SR & USART_SR_RXNE) {
		// Read to clear the flag
		value = (char)USART3->DR;
		if (lcd[1].flags & LCD_ACTIVE)
			lcdButtonProcess(value, 1);
		else {
			// Buffer it up, if it's not full to the brim
			if (!_isBufferFull(&(ser->rx)))
				_queueByte(&(ser->rx), value);
			// Notify any receivers
			semaphoreGiveISR(ser->readLock, &cs);
		}
	}
	if (USART3->SR & USART_SR_TXE) {
		if (_isBufferEmpty(&(ser->tx)))
			// Nothing to send, disable interrupt
			USART3->CR1 &= ~USART_CR1_TXEIE;
		else {
			value = _pullByte(&(ser->tx));
			USART3->DR = value;
		}
	}
	if (cs)
		_taskYield();
}

// lcdClear - Clears the screen
void lcdClear(PROS_FILE *lcdPort) {
	lcdSetText(lcdPort, 1, "");
	lcdSetText(lcdPort, 2, "");
}

// lcdInit - Enables the LCD on the specified port
void lcdInit(PROS_FILE *lcdPort) {
	// No flags = 8 N 1
	if (lcdPort == uart1 || lcdPort == uart2) {
		uint32_t idx = (uint32_t)lcdPort - 1;
		usartInit(lcdPort, 19200, 0);
		// Clear LCD state, turn on
		lcd[idx].flags = LCD_ACTIVE;
		lcd[idx].state = 0;
		lcd[idx].buttons = 0;
	}
}

// lcdPrint - Convenience method that performs snprintf() and then lcdSetText()
void lcdPrint(PROS_FILE *lcdPort, unsigned char line, const char *fmt, ...) {
	char buffer[17];
	// Pass to vsnprintf
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, 17, fmt, args);
	va_end(args);
	// Ensure that there are no null-terminator issues
	buffer[16] = 0;
	lcdSetText(lcdPort, line, buffer);
}

// lcdReadButtons - Reads the button status from the LCD display and returns the buttons
// pressed as a bit mask. Does not block, always returns the last button status
unsigned int lcdReadButtons(PROS_FILE *lcdPort) {
	uint32_t port = (uint32_t)lcdPort - 1;
	if (port < 2) {
		if (lcd[port].flags & LCD_ACTIVE)
			return (uint32_t)(lcd[port].buttons);
	}
	return 0;
}

// lcdSetBacklight - Turns the specified LCD backlight on or off
// The backlight will not update until the next line is sent (maybe 15ms latency)
void lcdSetBacklight(PROS_FILE *lcdPort, bool backlight) {
	uint32_t port = (uint32_t)lcdPort - 1;
	if (port < 2) {
		if (backlight)
			lcd[port].flags |= LCD_BACKLIGHT;
		else
			lcd[port].flags &= ~LCD_BACKLIGHT;
	}
}

// _lcdDump - Dumps characters from the buffers to active LCDs (run from the IDLE TASK)
// lcdIndex == 0 for port #1 or 1 for port #2
void _lcdDump(uint32_t lcdIndex) {
	// 22 byte packets
	uint8_t flags = lcd[lcdIndex].flags, total;
	if (flags & LCD_ACTIVE) {
		// Concoct the port
		PROS_FILE *lcdPort = (PROS_FILE*)(lcdIndex + 1);
		char value, *ptr = &(lcd[lcdIndex].screen[0]);
		// Set line (alternating order)
		if (flags & LCD_ROW_2) {
			ptr += 16;
			lcd[lcdIndex].flags = flags & ~LCD_ROW_2;
		} else
			lcd[lcdIndex].flags = flags | LCD_ROW_2;
		flags &= (LCD_ROW_2 | LCD_BACKLIGHT);
		// Sync bytes
		fputc(0xAA, lcdPort);
		fputc(0x55, lcdPort);
		// Packet type 1E
		fputc(0x1E, lcdPort);
		// 18 bytes to follow
		fputc(0x12, lcdPort);
		// LCD flags
		fputc((char)flags, lcdPort);
		total = flags;
		// Run until out of space or end of buffer
		for (uint32_t i = 0; i < 16; i++) {
			value = *ptr;
			if (value) {
				// Output text
				fputc(value, lcdPort);
				total += (uint8_t)(value);
				ptr++;
			} else {
				// Pad with spaces
				fputc(' ', lcdPort);
				total += (uint8_t)' ';
			}
		}
		// Checksum
		fputc((char)((uint8_t)(-total)), lcdPort);
	}
}

// lcdSetText - Sets a line (1 or 2) of text on the LCD to the specified null-terminated string
// Up to 16 characters will actually be transmitted; this function is thread safe, but
// concurrent access to the same line of the same LCD will cause garbage text to be displayed
void lcdSetText(PROS_FILE *lcdPort, unsigned char line, const char *buffer) {
	// Blit to the buffer, automatically dumped on the idle task
	uint32_t port = (uint32_t)lcdPort - 1, i;
	line--;
	if (port < 2 && line < 2) {
		// Find pointer to location
		char *scr = &(lcd[port].screen[0]);
		if (line) scr += 16;
		for (i = 0; i < 16; i++) {
			// Space pad to 16 characters
			if (*buffer)
				*scr++ = *buffer++;
			else
				*scr++ = ' ';
		}
	}
}

// lcdShutdown - Disable the LCD on the specified port
void lcdShutdown(PROS_FILE *lcdPort) {
	if (lcdPort == uart1 || lcdPort == uart2) {
		usartShutdown(lcdPort);
		lcd[(uint32_t)lcdPort - 1].flags = 0;
	}
}

// _usartBufferInit - Clears one particular buffer and sets up its semaphore
static INLINE void _usartBufferInit(SerialPort_TypeDef* port) {
	port->rx.head = 0;
	port->rx.tail = 0;
	port->tx.head = 0;
	port->rx.tail = 0;
	port->readLock = semaphoreCreate();
}

// usartBufferCount - Determine the number of available characters on the specified USART
uint32_t usartBufferCount(SerialPort_TypeDef* port) {
	uint32_t head, tail;
	_enterCritical();
	head = (uint32_t)port->rx.head;
	tail = (uint32_t)port->rx.tail;
	_exitCritical();
	return (tail - head) & (uint32_t)_USART_MAX;
}

// usartBufferInit - Initializes (empty) the USART buffers
void usartBufferInit() {
	// PC
	_usartBufferInit(&usart[2]);
	// Ext 1
	_usartBufferInit(&usart[0]);
	// Ext 2
	_usartBufferInit(&usart[1]);
}

// usartFlushBuffers - Clears the USART output buffers (Trashes data! From ALL USARTS!)
void usartFlushBuffers() {
	__disable_irq();
	usart[0].tx.tail = usart[0].tx.head;
	usart[1].tx.tail = usart[1].tx.head;
	usart[2].tx.tail = usart[2].tx.head;
	__enable_irq();
}

// usartInit - Initialize the specified USART interface with the given connection parameters
// The interface argument can be 1 or 2 to specify UART1 and UART2 respectively
void usartInit(PROS_FILE *port, unsigned int baud, unsigned int flags) {
	// Determine correct USART
	USART_TypeDef *base;
	if (port == uart1)
		base = USART2;
	else if (port == uart2)
		base = USART3;
	else
		// Invalid interface
		return;
	_enterCritical();
	{
		base->CR1 = 0;
		// Flush buffers
		if (port == uart1) {
			usart[0].tx.tail = usart[0].tx.head;
			usart[0].rx.tail = usart[0].rx.head;
		} else {
			usart[1].tx.tail = usart[1].tx.head;
			usart[1].rx.tail = usart[1].rx.head;
		}
		// Configure base registers
		base->CR2 = flags & (USART_CR2_STOP0 | USART_CR2_STOP1);
		// Turn on USART
		base->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE |
			(flags & (USART_CR1_PCE | USART_CR1_PS | USART_CR1_M));
		base->CR3 = (uint16_t)0;
		// Set baud rate
		uint32_t brr = 225000000 / baud, mod = brr / 100, fracDiv = brr - (100 * mod);
		base->BRR = (uint16_t)(mod << 4) | (uint16_t)((((fracDiv << 4) + 50) / 100) & 0xF);
	}
	_exitCritical();
}

// usartShutdown - Disable the specified USART interface
void usartShutdown(PROS_FILE *usart) {
	// Disable transmitter, receiver, clock, and interrupts
	if (usart == uart1)
		USART2->CR1 = (uint16_t)0;
	else if (usart == uart2)
		USART3->CR1 = (uint16_t)0;
}
