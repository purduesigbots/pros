/*
 * periph.h - Higher-level peripheral manipulation for the Cortex (I2C, USART, ...)
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

#ifndef PERIPH_H_
#define PERIPH_H_

#include <cortex.h>
#include <stdarg.h>
#include <sys/types.h>

// Begin C++ extern to C
#ifdef __cplusplus
extern "C" {
#endif

// The Cortex is configured at 72 MHz (72 clocks per microsecond)
#define CYCLES_PER_MICROSECOND 72

// Pin/port configuration for ioSetDirection
// Analog input
#define DDR_INPUT_ANALOG 0x00
// Floating input
#define DDR_INPUT_FLOATING 0x04
// Input with weak pull down
#define DDR_INPUT_PULLDOWN 0x08
// Input with weak pull up
#define DDR_INPUT_PULLUP 0x0A
// General-purpose output
#define DDR_OUTPUT 0x01
// Open-drain output
#define DDR_OUTPUT_OD 0x05
// Alternate function output
#define DDR_AFO 0x09
// Alternate function open-drain output
#define DDR_AFO_OD 0x0D

// I/O pin counts (maple compatible alias)
// 27 available digital I/O; the motor ports are not on the Cortex and are thus excluded
#define BOARD_NR_GPIO_PINS 27
// 8 available analog I/O
#define BOARD_NR_ADC_PINS 8
// 12 available I/O for the advanced sensors
#define BOARD_NR_DIGITAL_IO 12

// Pinning definitions for ioSetDirection
// Digital inputs 1-12
#define PIN_DIGITAL_1 GPIOE, 9
#define PIN_DIGITAL_2 GPIOE, 11
#define PIN_DIGITAL_3 GPIOC, 6
#define PIN_DIGITAL_4 GPIOC, 7
#define PIN_DIGITAL_5 GPIOE, 13
#define PIN_DIGITAL_6 GPIOE, 14
#define PIN_DIGITAL_7 GPIOE, 8
#define PIN_DIGITAL_8 GPIOE, 10
#define PIN_DIGITAL_9 GPIOE, 12
#define PIN_DIGITAL_10 GPIOE, 7
#define PIN_DIGITAL_11 GPIOD, 0
#define PIN_DIGITAL_12 GPIOD, 1
// Speaker port (NOT 5V tolerant!)
#define PIN_SP GPIOA, 4
// Analog inputs 1-8
#define PIN_ANALOG_1 GPIOA, 0
#define PIN_ANALOG_2 GPIOA, 1
#define PIN_ANALOG_3 GPIOA, 2
#define PIN_ANALOG_4 GPIOA, 3
#define PIN_ANALOG_5 GPIOC, 2
#define PIN_ANALOG_6 GPIOC, 3
#define PIN_ANALOG_7 GPIOC, 0
#define PIN_ANALOG_8 GPIOC, 1
// Analog #9 is on the speaker port (NOT 5V tolerant!)
#define PIN_ANALOG_9 PIN_SP
// Communications ports (these pins are NOT 5V tolerant!)
#define PIN_UART1_TX GPIOD, 5
#define PIN_UART1_RX GPIOD, 6
#define PIN_UART2_TX GPIOC, 10
#define PIN_UART2_RX GPIOC, 11
#define PIN_I2C1_SCL GPIOB, 8
#define PIN_I2C1_SDA GPIOB, 9

// Constants for ioSetInterrupt - Which edges to register interrupt?
#define INTERRUPT_EDGE_RISING 1
#define INTERRUPT_EDGE_FALLING 2
#define INTERRUPT_EDGE_BOTH 3

// I2C address
#define I2C_OWN_ADDR 0x10

// Serial port definitions
// 8 data bits (typical)
#define SERIAL_DATABITS_8 ((uint16_t)0x0000)
// 9 data bits
#define SERIAL_DATABITS_9 ((uint16_t)0x1000)
// 1 stop bit (typical)
#define SERIAL_STOPBITS_1 ((uint16_t)0x0000)
// 2 stop bits
#define SERIAL_STOPBITS_2 ((uint16_t)0x2000)
// No parity (typical)
#define SERIAL_PARITY_NONE ((uint16_t)0x0000)
// Even parity
#define SERIAL_PARITY_EVEN ((uint16_t)0x0400)
// Odd parity
#define SERIAL_PARITY_ODD ((uint16_t)0x0600)

// Type definition for interrupt handlers
typedef void (*InterruptHandler)(unsigned char pin);

// Type definition for encoder information
// WARNING: This structure is used in the assembly source code "encoder_isr.s". Changes made
// here may have unintended results and will require adjustments accordingly.
typedef struct {
	// Sensor value field; read by the appropriate poll function
	volatile int32_t value;
	volatile uint16_t flags;
	// Dual-port sensors like the sonar or encoder will use these to link the ports together
	volatile uint8_t portTop;
	volatile uint8_t portBottom;
	// Event trigger function
	volatile InterruptHandler eventTrigger;
	// Last value, calibrated value, or some other type of value storage
	volatile uint32_t lastValue;
} Sensor_TypeDef;

typedef struct {
	// Sensor value field; read by the appropriate poll function
	volatile int32_t value;
	volatile uint8_t flags;
	volatile uint8_t RESERVED;
	// Calibrated value from analogCalibrate()
	volatile uint16_t calibValue;
	// Last value, calibrated value, or some other type of value storage
	volatile uint32_t lastValue;
} Analog_TypeDef;

// Encoder information is stored as an opaque pointer to a structure in memory
typedef void * Encoder;

// Ultrasonic information is stored as an opaque pointer to a structure in memory
typedef void * Ultrasonic;

// Gyro information is stored as an opaque pointer to a structure in memory
typedef void * Gyro;

// Pin lookup tables
extern const uint8_t _adcChannelTable[BOARD_NR_ADC_PINS];
extern const GPIO_TypeDef* const _pinLookupTable[BOARD_NR_GPIO_PINS];
extern const uint8_t _pinIndexTable[BOARD_NR_GPIO_PINS];

// Sensor status tables
extern Sensor_TypeDef _sensorState[BOARD_NR_DIGITAL_IO];
extern Analog_TypeDef _analogState[BOARD_NR_ADC_PINS];

// Schedules a task to occur on the high-resolution timer tick in the given number of us
// us must be at least two and less than 60000 for proper operation
static INLINE void _highResSchedule(uint8_t channel, uint16_t diff) {
	/* In the future, for the user high-res timer (probably a separate one like TIM2), we will
	 * do what this timer used to do
	if (timedTasks[0] == 0) {
		timedTasks[0] = task;
		TIM8->CCR1 = TIM8->CNT + diff;
		TIM8->SR &= ~TIM_SR_CC1IF;
		TIM8->DIER |= TIM_DIER_CC1IE;
	}*/
	switch (channel) {
	case 0:
		// TIM8 CCR 1
		TIM8->CCR1 = TIM8->CNT + diff;
		TIM8->SR = ~TIM_SR_CC1IF & 0x1EFF;
		TIM8->DIER |= TIM_DIER_CC1IE;
		break;
	case 1:
		// TIM8 CCR 2
		TIM8->CCR2 = TIM8->CNT + diff;
		TIM8->SR = ~TIM_SR_CC2IF & 0x1EFF;
		TIM8->DIER |= TIM_DIER_CC2IE;
		break;
	case 2:
		// TIM8 CCR 3
		TIM8->CCR3 = TIM8->CNT + diff;
		TIM8->SR = ~TIM_SR_CC3IF & 0x1EFF;
		TIM8->DIER |= TIM_DIER_CC3IE;
		break;
	case 3:
		// TIM8 CCR 4
		TIM8->CCR4 = TIM8->CNT + diff;
		TIM8->SR = ~TIM_SR_CC4IF & 0x1EFF;
		TIM8->DIER |= TIM_DIER_CC4IE;
		break;
	}
}

// ioGetInput - Gets the digital value (1 or 0) of a pin configured as a digital input
static INLINE bool ioGetInput(GPIO_TypeDef* port, uint32_t pin) {
	// Shift right that many bits, then mask everything but the ones
	return ((port->IDR >> (pin & 0x0F)) & 0x01) != 0;
}

// ioGetOutput - Gets the current value (1 or 0) of a pin configured as a digital output
static INLINE bool ioGetOutput(GPIO_TypeDef* port, uint32_t pin) {
	// Shift right that many bits, then mask everything but the ones
	return ((port->ODR >> (pin & 0x0F)) & 0x01) != 0;
}

// ioSetOutput - Sets the digital value (1 or 0) of a pin configured as a digital output
static INLINE void ioSetOutput(GPIO_TypeDef* port, uint32_t pin, bool value) {
	if (value)
		// Atomic bit set
		port->BSRR = ((uint32_t)0x00000001) << (pin & 0x0F);
	else
		// Atomic bit reset
		port->BRR = ((uint32_t)0x00000001) << (pin & 0x0F);
}

// Library functions
// adcOff - Stops the ADC, use before reconfiguring
void adcOff();
// adcOn - Starts the ADC
void adcOn();
// adcRead - Reads a channel 0-15 from the ADC
uint16_t adcRead(uint32_t channel);
// adcSetChannels - Sets the channels sampled by the ADC (ADC must be off!)
void adcSetChannels(uint32_t num, ...);
// analogCalibrate - Calibrates analog sensor on specified channel
int analogCalibrate(unsigned char channel);
// analogReadCalibrated - Read analog sensor offset on specified channel
int analogReadCalibrated(unsigned char channel);
// analogReadCalibratedHR - Read analog sensor offset with high resolution on specified channel
int analogReadCalibratedHR(unsigned char channel);

// encoderGet - Gets the value of the encoder
int encoderGet(Encoder enc);
// encoderInit - Initializes and enables a quadrature encoder on two digital ports
Encoder encoderInit(unsigned char portTop, unsigned char portBottom, bool reverse);
// encoderReset - Resets the encoder to zero
void encoderReset(Encoder enc);
// encoderShutdown - Stops and disables the encoder
void encoderShutdown(Encoder enc);

// gyroGetAngle - Gets the current gyro angle in degrees
int gyroGet(Gyro g);
// gyroInit - Initialize and calibrate the gyro - call in initialize()
Gyro gyroInit(unsigned char port, unsigned short multiplier);
// gyroReset - Resets the gyro angle to zero
void gyroReset(Gyro g);
// gyroShutdown - Stop the gyro, if starting by using gyroStart()
void gyroShutdown(Gyro g);

// i2cRead - Reads the specified number of data bytes from the specified address
bool i2cRead(uint8_t addr, uint8_t *data, uint16_t count);
// i2cReadRegister - Reads the specified amount of data from the given register address on
// the specified I2C address
bool i2cReadRegister(uint8_t addr, uint8_t reg, uint8_t *value, uint16_t count);
// i2cSetAddress - Sets the Cortex's I2C address; must be used when I2C is off
void i2cSetAddress(uint8_t addr);
// i2cWrite - Writes the specified number of data bytes to the specified address
bool i2cWrite(uint8_t addr, uint8_t *data, uint16_t count);
// i2cWriteRegister - Writes the specified data to a register on the specified I2C address
bool i2cWriteRegister(uint8_t addr, uint8_t reg, uint16_t value);

// imeInitializeAll - Initializes all IMEs and returns the number of IMEs thus initialized
unsigned int imeInitializeAll();
// imeGet - Gets the current 32-bit count of the specified IME address
bool imeGet(unsigned char address, int *value);
// imeGetVelocity - Gets the current velocity of the specified IME in units of rpm of encoder
// Divide by 30.056 (269), 39.2 (393), or 24.5 (393 HS) to get output shaft RPM
bool imeGetVelocity(unsigned char address, int *value);
// imeReset - Resets the specified IME's counters to zero
bool imeReset(unsigned char address);
// imeShutdown - Shuts down all IMEs on the chain
void imeShutdown();

// ioClearInterrupt - Disables interrupts on the specified pin
void ioClearInterrupt(unsigned char pin);
// ioMultiSetDirection - Configures the specified pins in the bit mask to the given type
void ioMultiSetDirection(GPIO_TypeDef* port, uint16_t pinMask, uint32_t type);
// ioSetDirection - Configures the pin as an input or output with a variety of settings
void ioSetDirection(GPIO_TypeDef* port, uint32_t pin, uint32_t type);
// ioSetInterrupt - Sets up an interrupt to occur on the specified pin, and resets count & time
void ioSetInterrupt(unsigned char pin, unsigned char edges, InterruptHandler handler);

// motorControlGet - Gets the last sent PWM value of a channel 1..10 from 0 to 255
uint8_t motorControlGet(uint32_t channel);
// motorControlSet - Sets the PWM value of a channel 1..10 from 0 to 255
void motorControlSet(uint32_t channel, uint8_t value);
// motorControlStop - Stops all motors
void motorControlStop();

// Arduino-esque methods for high-level port/pin manipulation
// digitalRead - Gets the digital value (1 or 0) of a pin configured as a digital input
bool digitalRead(unsigned char pin);
// digitalWrite - Sets the digital value (1 or 0) of a pin configured as a digital output
void digitalWrite(unsigned char pin, bool value);
// pinMode - Configures the pin as an input or output with a variety of settings
void pinMode(unsigned char pin, unsigned char mode);

// pwmSetEnable - Enables or disables PWM on the specified pin. If PWM is enabled, the initial
// value for a channel is the last value set by pwmWrite() or 0 if never set before
void pwmSetEnable(uint32_t pin, bool enable);
// pwmSetMaximum - Sets the maximum PWM value for all channels from 1 to 65535
void pwmSetMaximum(uint16_t max);
// pwmSetInvert - Inverts the PWM driver on the specified pin to be LOW for the "on" period and
// HIGH for the "off" period
void pwmSetInvert(uint32_t pin, bool invert);
// pwmSetPeriod - Sets the period of each PWM cycle (1 / frequency) in microseconds. This
// affects all four PWM channels equally. The maximum PWM value is configured to 65535
void pwmSetPeriod(uint32_t period_us);
// pwmSetScaler - Sets the PWM clock prescaler for all channels from 1 to 65535
void pwmSetScaler(uint16_t scaler);
// pwmSetPeriodPrecise - Sets the period of each PWM cycle as precisely as possible in
// microseconds. This affects all four PWM channels equally. The maximum PWM value is
// determined automatically and returned as an integer from 1 to 65535
uint16_t pwmSetPeriodPrecise(uint32_t period_us);
// pwmWrite - Sets PWM period "on" value from 0 to configured maximum value (default 65535)
void pwmWrite(uint32_t pin, uint16_t value);

// speakerInit - Prepares speaker for use and enables interrupt
// Speaker library increases RAM usage by ~1K and FLASH usage by ~4K, be warned!
void speakerInit();
// speakerPlayArray - Play up to 3 RTTTL tracks simultaneously
void speakerPlayArray(const char * * rttl);
// speakerPlayRtttl - Play a single RTTTL string encoded track
// To play polyphonic sound, use speakerPlayArray
void speakerPlayRtttl(const char *rttl);
// speakerShutdown - Power down the speaker, kill interrupt, stop tones
void speakerShutdown();

// End C++ extern to C
#ifdef __cplusplus
}
#endif

#endif
