/*
 * system.c - Low-level system control functions for interrupts, clocks, and peripheral init
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
#include <kernel.h>
#include <periph.h>
#include <semphr.h>

// Low-resolution clock
volatile uint32_t _clockLowRes;

// Prototype this from motorcontrol.c
void _motorApply();

// Prototype these from i2c.c
void _i2cEnd();
void _i2cInit();

// Prototype this from supervisor.c
void _svNextByte();

// Prototype this from ultrasonic.c
void _ultrasonicTimeout();

// Prototype this from io.c
extern uint16_t adcDataIn[16];

#if 0
// intDisable - Disables a Cortex peripheral interrupt [not a negative system interrupt]
static INLINE void intDisable(IRQn_Type irq) {
	NVIC->ICER[(uint32_t)irq >> 0x05] = (uint32_t)(1 << ((uint32_t)irq & 0x1F));
}
#endif

// intEnable - Enables a Cortex peripheral interrupt
static INLINE void intEnable(IRQn_Type irq) {
	NVIC->ISER[(uint32_t)irq >> 0x05] = (uint32_t)(1 << ((uint32_t)irq & 0x1F));
}

// intSetPriority - Sets the priority for an interrupt; 15 is lowest priority, 0 is highest
static INLINE void intSetPriority(IRQn_Type irq, uint32_t priority) {
	if (irq < 0)
		SCB->SHP[((uint32_t)irq & 0x0F) - 4] = (priority << 4) & 0xFF;
	else
		NVIC->IP[(uint32_t)irq] = (priority << 4) & 0xFF;
}

// initADC - Initializes the analog converter on the 8 analog sensor pins
static INLINE void initADC() {
	uint32_t temp;
	// Turn the ADC clock on
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	// Reset ADC
	temp = RCC->APB2RSTR;
	RCC->APB2RSTR = temp | RCC_APB2RSTR_ADC1RST;
	__dsb();
	// Clear reset
	RCC->APB2RSTR = temp;
	// Independent ADC, scan mode
	ADC1->CR1 = ADC_CR1_SCAN;
	// No external trigger, right-aligned, continuous conversions
	temp = ADC_CR2_NOTRIG | ADC_CR2_CONT | ADC_CR2_DMA;
	ADC1->CR2 = temp;
	// 8 channels, sampling order 0 1 2 3 12 13 10 11
	adcSetChannels(8, 0, 1, 2, 3, 12, 13, 10, 11);
	// Set all channels' sampling time to 55.5 cycles
	ADC1->SMPR1 = (uint32_t)0x00B6DB6D;
	ADC1->SMPR2 = (uint32_t)0x2DB6DB6D;
	// ADC on
	temp |= ADC_CR2_ADON;
	ADC1->CR2 = temp;
	// Reset calibration
	ADC1->CR2 = temp | ADC_CR2_RSTCAL;
	while (ADC1->CR2 & ADC_CR2_RSTCAL);
	// Begin calibration
	ADC1->CR2 = temp | ADC_CR2_CAL;
	while (ADC1->CR2 & ADC_CR2_CAL);
	// Start eternal conversion
	adcOn();
}

// initClocks - Starts the MCU clocks at the intended speed (72 MHz)
// No startup timeout is enforced since the entire Cortex will come crashing down if the clocks
// cannot get to the correct speed; might as well hang. In the future, a "clock monitor fail"
// mode might be implemented if this extremely, extremely remote possibility becomes an issue.
static INLINE void initClocks() {
	uint32_t temp;
	// Clear interrupt pending bits
	RCC->CIR = 0x009F0000;
	// Reset Sleep Control register to zero to avoid unwanted deep sleep
	SCB->SCR = 0x00000000;
	// Turn on the HSE (8 MHz)
	temp = RCC->CR;
	temp |= RCC_CR_HSEON;
	temp &= ~RCC_CR_HSEBYP;
	RCC->CR = temp;
	RCC->CFGR &= ~RCC_CFGR_SW;
	// Wait for HSE to start up
	while (!(RCC->CR & RCC_CR_HSERDY));
	temp = FLASH->ACR;
	temp &= ~FLASH_ACR_LATENCY;
	// Prefetch buffer on, 2 wait states (reason for the prefetch buffer)
	temp |= FLASH_ACR_LATENCY_2 | FLASH_ACR_PRFTBE;
	FLASH->ACR = temp;
	// APB1 is 36 MHz and APB2 is 72 MHz
	// ADC clock is 12 MHz (72 MHz / 6)
	// PLL enabled from HSE = 8 MHz * 9 = 72 MHz
	RCC->CFGR = RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_ADCPRE_DIV6 | RCC_CFGR_PLLMUL_9 |
		RCC_CFGR_PLLSRC_HSE;
	// Turn PLL on
	RCC->CR |= RCC_CR_PLLON;
	// Wait for PLL to start up
	while (!(RCC->CR & RCC_CR_PLLRDY));
	// Select PLL as system clock
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	// Wait for system clock to become the PLL
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	// Reset system clocks
	_clockLowRes = 0;
}

// initDAC - Initializes DAC to run the Wave driver
static INLINE void initDAC() {
	uint32_t temp;
	// Turn the DAC clock on
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	// Reset DAC
	temp = RCC->APB1RSTR;
	RCC->APB1RSTR = temp | RCC_APB1RSTR_DACRST;
	__dsb();
	// Clear reset
	RCC->APB1RSTR = temp;
	// Turn on DAC channel 1 and auto-connect output (in analog mode) PA4, pull from TIM6
	// (default), enable trigger and DMA
	DAC->CR = DAC_CR_DMAEN1 | DAC_CR_EN1 | DAC_CR_BOFF1 | DAC_CR_TEN1;
}

// initDMA - Initializes DMA1 for the ADC and Maple
static INLINE void initDMA() {
	// Turn the DMA1 clock on
	RCC->AHBENR |= RCC_AHBENR_DMA1EN | RCC_AHBENR_DMA2EN;
	// Transfer from ADC1's DR to the adcDataIn array with a buffer size of 8, no peripheral
	// address increment, memory address increment, 16 bits at a time, circular mode,
	// default priority
	DMA1_Channel1->CCR = DMA_CCR_SRC | DMA_CCR_MEMINC | DMA_CCR_SRC_HWORD | DMA_CCR_DST_HWORD |
		DMA_CCR_CIRC | DMA_CCR_PRI_HIGH;
	DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));
	DMA1_Channel1->CMAR = (uint32_t)(&adcDataIn);
	// Transfer from the wave array to DAC1's DR with a buffer size of ?, no peripheral
	// address increment, memory address increment, 16 bits at a time, circular mode, default
	// priority
	DMA2_Channel3->CCR = DMA_CCR_DST | DMA_CCR_MEMINC | DMA_CCR_SRC_HWORD | DMA_CCR_DST_HWORD |
		DMA_CCR_CIRC | DMA_CCR_PRI_HIGH | DMA_CCR_TCIE | DMA_CCR_HTIE;
	DMA2_Channel3->CPAR = (uint32_t)(&(DAC->DHR12R1));
	// DMA turned on in the ADC/DAC call
}

// initEXTI - Initializes the external interrupts
static INLINE void initEXTI() {
	// Mask all interrupts and events
	EXTI->IMR = 0;
	EXTI->EMR = 0;
	// PD0, PD1 supply EXTI0, EXTI1; PA2, PA3 supply EXTI2, EXTI3
	AFIO->EXTICR[0] = (uint32_t)0x0033;
	// PA4, PA5 supply EXTI4, EXTI5; PC6, PC7 supply EXTI6, EXTI7
	AFIO->EXTICR[1] = (uint32_t)0x2200;
	// PE8..11 supplies EXTI8..11
	AFIO->EXTICR[2] = (uint32_t)0x4444;
	// PE12..14 supplies EXTI12..14
	AFIO->EXTICR[3] = (uint32_t)0x0444;
	// Disable on falling and rising edges
	EXTI->FTSR = 0;
	EXTI->RTSR = 0;
	// Clear all pending external interrupts
	EXTI->PR = (uint32_t)0x0007FFFF;
}

// initI2C - Initializes the I2C port
static INLINE void initI2C() {
	uint32_t temp;
	// Initialize I2C flags
	_i2cInit();
	// Turn the I2C clock on
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	// Reset I2C
	temp = RCC->APB1RSTR;
	RCC->APB1RSTR = temp | RCC_APB1RSTR_I2C1RST;
	__dsb();
	// Clear reset
	RCC->APB1RSTR = temp;
	// OFF to configure
	I2C1->CR1 = I2C_CR1_SWRST;
	// This delay required for I2C to acknowledge the request
	for (uint32_t delay = 0; delay < 32; delay++) asm volatile("");
	I2C1->CR1 = 0;
	// Set peripheral clock frequency to 36 MHz
	I2C1->CR2 = (uint16_t)36 | I2C_CR2_ITERREN;
	// Set I2C clock to 400 KHz (Standard mode)
	I2C1->CCR = I2C_CCR_FS | ((uint16_t)90);
	I2C1->TRISE = ((uint16_t)10);
	// No reset, no error checking, no general call response, disable SMBus mode
	I2C1->CR1 |= I2C_CR1_PE | I2C_CR1_ACK;
	// Single-address mode
	i2cSetAddress(I2C_OWN_ADDR);
	I2C1->OAR2 &= ~I2C_OAR2_ENDUAL;
	// Set up GPIOB 8 and 9 [I2C SCL, I2C SDA] as alternate function outputs open-drain
	ioSetDirection(PIN_I2C1_SCL, DDR_AFO_OD);
	ioSetDirection(PIN_I2C1_SDA, DDR_AFO_OD);
}

// initInterrupts - Initializes the NVIC (interrupt system)
static INLINE void initInterrupts() {
	// Vector table is at start of Flash
	SCB->VTOR = FLASH_BASE;
	// Priority group #3 configuration
	SCB->AIRCR = SCB_AIRCR_VECTKEY | SCB_AIRCR_PRIGROUP_3;
	// IRQ channel -2 (PendSV) enable
	intSetPriority(PendSV_IRQn, 14);
	// IRQ channel -5 (SV call) enable
	intSetPriority(SVCall_IRQn, 13);
	// IRQ channel 46 (Timer #8 capture/compare) enable, very high priority
	intSetPriority(TIM8_CC_IRQn, 2);
	intEnable(TIM8_CC_IRQn);
	// IRQ channel 37 (USART #1) enable
	intSetPriority(USART1_IRQn, 12);
	intEnable(USART1_IRQn);
	// IRQ channel 38 (USART #2) enable
	intSetPriority(USART2_IRQn, 12);
	intEnable(USART2_IRQn);
	// IRQ channel 39 (USART #3) enable
	intSetPriority(USART3_IRQn, 12);
	intEnable(USART3_IRQn);
	// IRQ channel (SPI #1) enable
	intSetPriority(SPI1_IRQn, 11);
	intEnable(SPI1_IRQn);
	// IRQ channel 6 (Pin change interrupt 0) enable, high priority
	intSetPriority(EXTI0_IRQn, 3);
	intEnable(EXTI0_IRQn);
	// IRQ channel 7 (Pin change interrupt 1) enable, high priority
	intSetPriority(EXTI1_IRQn, 3);
	intEnable(EXTI1_IRQn);
	// IRQ channel 23 (Pin change interrupts 5-9) enable, high priority
	intSetPriority(EXTI9_5_IRQn, 3);
	intEnable(EXTI9_5_IRQn);
	// IRQ channel 40 (Pin change interrupts 10-15) enable, high priority
	intSetPriority(EXTI15_10_IRQn, 3);
	intEnable(EXTI15_10_IRQn);
	// IRQ channel 31 (I2C1 event) enable, very high priority not to be interrupted (it's fast)
	intSetPriority(I2C1_EV_IRQn, 2);
	intEnable(I2C1_EV_IRQn);
	// IRQ channel 32 (I2C1 error) enable, high priority
	intSetPriority(I2C1_ER_IRQn, 3);
	intEnable(I2C1_ER_IRQn);
	// IRQ channel 58 (DMA2 channel3 TX complete) enable, high priority
	intSetPriority(DMA2_Channel3_IRQn, 3);
	intEnable(DMA2_Channel3_IRQn);
	// SysTick fires every 9000 clock cycles (72M / 8 / 9K = 1K/s = 1 ms)
	SysTick->LOAD = 8999;
	// Set priority for SysTick interrupt to a low priority (second from bottom)
	intSetPriority(SysTick_IRQn, 13);
	// Reset counter to zero
	SysTick->VAL = 0;
	// Turn on, enable interrupt, select clock / 8
	SysTick->CTRL = SysTick_CTRL_TICKINT | SysTick_CTRL_ENABLE;
	// Enable the faults
	SCB->SHCSR = SCB_SHCSR_USGFAULTENA | SCB_SHCSR_BUSFAULTENA;
	__enable_irq();
	__enable_fault_irq();
}

// Bits to toggle in the reset register to reset I/O ports to defaults
#define _PORT_RESET_BITS (RCC_APB2RSTR_IOPARST | RCC_APB2RSTR_IOPBRST | \
	RCC_APB2RSTR_IOPCRST | RCC_APB2RSTR_IOPDRST | RCC_APB2RSTR_IOPERST | \
	RCC_APB2RSTR_AFIORST)

// initPorts - Initializes the GPIO ports
static INLINE void initPorts() {
	uint32_t temp;
	// Enable clocks to all I/O ports and AFIO
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN |
		RCC_APB2ENR_IOPDEN | RCC_APB2ENR_IOPEEN | RCC_APB2ENR_AFIOEN;
	// Reset the I/O ports and AFIO
	temp = RCC->APB2RSTR;
	RCC->APB2RSTR = temp | _PORT_RESET_BITS;
	__dsb();
	// Clear reset
	RCC->APB2RSTR = temp;
	// Set up GPIOA 0..4 [Analog 1, 2, 3, 4, SP] as analog inputs
	ioMultiSetDirection(GPIOA, 0x001F, DDR_INPUT_ANALOG);
	// Set up GPIOA 5, 7 [SPI1 MOSI, SCK] and 9 as alternate function outputs push-pull
	ioMultiSetDirection(GPIOA, 0x02A0, DDR_AFO);
	// Set up GPIOA 10 as a floating input
	ioSetDirection(GPIOA, 10, DDR_INPUT_FLOATING);
	// Set up GPIOA 11 as a push-pull output
	ioSetDirection(GPIOA, 11, DDR_OUTPUT);
	// All remaining GPIOA pins to pull-up inputs (power consumption)
	ioMultiSetDirection(GPIOA, 0xF140, DDR_INPUT_PULLUP);
	// Turn on GPIOB 8 & 9 [I2C SCL, SDA] pre-emptively
	ioSetDirection(PIN_I2C1_SCL, DDR_OUTPUT);
	ioSetDirection(PIN_I2C1_SDA, DDR_OUTPUT);
	ioSetOutput(PIN_I2C1_SCL, 1);
	ioSetOutput(PIN_I2C1_SDA, 1);
	// All remaining GPIOB pins to pull-up inputs (power consumption)
	ioMultiSetDirection(GPIOB, 0xFCFF, DDR_INPUT_PULLUP);
	// Set up GPIOC 0..3 [Analog 7, 8, 5, 6] as analog inputs
	ioMultiSetDirection(GPIOC, 0x000F, DDR_INPUT_ANALOG);
	// Set up GPIOC 10 [UART2 TX] as an alternate function output push-pull
	ioSetDirection(PIN_UART2_TX, DDR_AFO);
	// Set up GPIOC 11 [UART2 RX] as a floating input
	ioSetDirection(PIN_UART2_RX, DDR_INPUT_FLOATING);
	// Set up GPIOC 13..15 as input pull-down
	ioMultiSetDirection(GPIOC, 0xE000, DDR_INPUT_PULLDOWN);
	// All remaining GPIOC pins to pull-up inputs (power consumption)
	ioMultiSetDirection(GPIOC, 0x13F0, DDR_INPUT_PULLUP);
	motorControlStop();
	// Set up GPIOD 5 [UART1 TX] as an alternate function output push-pull
	ioSetDirection(PIN_UART1_TX, DDR_AFO);
	// Set up GPIOD 6 [UART1 RX] as a floating input
	ioSetDirection(PIN_UART1_RX, DDR_INPUT_FLOATING);
	// Set up GPIOD 3, 4, 7, and 8 as push-pull outputs
	ioMultiSetDirection(GPIOD, 0x0198, DDR_OUTPUT);
	// Set up GPIOD 12..15 [TIM4 CH1-4] as alternate function outputs push-pull
	ioMultiSetDirection(GPIOD, 0xF000, DDR_AFO);
	// All remaining GPIOD pins to pull-up inputs (power consumption)
	ioMultiSetDirection(GPIOD, 0x0E07, DDR_INPUT_PULLUP);
	// Set up GPIOE 0, 5, and 6 [SPI handshake?] as push-pull outputs
	ioMultiSetDirection(GPIOE, 0x0061, DDR_OUTPUT);
	// All remaining GPIOE pins [including GPIOE 3..4] to pull-up inputs (power consumption)
	ioMultiSetDirection(GPIOE, 0xFF9E, DDR_INPUT_PULLUP);
	// Remap the following: USART2, USART3, TIM4, I2C1, TIM1
	AFIO->MAPR = AFIO_MAPR_TIM4_REMAP | AFIO_MAPR_USART2_REMAP | AFIO_MAPR_USART3_REMAP |
		AFIO_MAPR_I2C1_REMAP | AFIO_MAPR_TIM1_REMAP_FULL;
	AFIO->MAPR2 = 0x0000;
	// GPIOF and GPIOG don't have actual pins, so no need to set them to pullups
}

// initSerial - Sets up the USARTs for communication through the UART ports and debug terminal
static INLINE void initSerial() {
	uint32_t temp;
	// Ready the buffers
	usartBufferInit();
	// Turn on USART 1, 2 and 3 clocks
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB1ENR_USART3EN;
	// Reset USART1
	temp = RCC->APB2RSTR;
	RCC->APB2RSTR = temp | RCC_APB2RSTR_USART1RST;
	__dsb();
	// Clear reset
	RCC->APB2RSTR = temp;
	// Reset USART2 and USART3
	temp = RCC->APB1RSTR;
	RCC->APB1RSTR = temp | (RCC_APB1RSTR_USART2RST | RCC_APB1RSTR_USART3RST);
	__dsb();
	// Clear reset
	RCC->APB1RSTR = temp;
	// Default: 8 data bits, no parity, no flow control, one stop bit
	USART2->CR1 = (uint16_t)0;
	USART3->CR1 = (uint16_t)0;
	// 115200 baud on the fast bus = 625, on the slow bus = 308
	// But they want a weird 230113 baud = 313...
	USART1->BRR = (uint16_t)313;
	// Turn on USART1
	USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

// initSPI - Sets up SPI1 for communication to the master
static INLINE void initSPI() {
	uint32_t temp;
	// Turn on SPI clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	// Reset the SPI
	temp = RCC->APB2RSTR;
	RCC->APB2RSTR = temp | RCC_APB2RSTR_SPI1RST;
	__dsb();
	// Clear reset
	RCC->APB2RSTR = temp;
	// CPOL = 0, CPHA = 1, Master mode, 16-bit data size, Software NSS, divide by 32, MSB first
	SPI1->CR1 = SPI_CR1_CPHA_1 | SPI_CR1_MSTR | SPI_CR1_16BIT | SPI_CR1_DIV32 | SPI_CR1_SPE |
		SPI_CR1_NSS_SOFT;
	// Do not enable the RXNE interrupt here as supervisor is not ready yet
	// If the interrupt is turned on too soon, it will get serviced the moment that
	// initInterrupts() calls "cpsie i", which sends garbage to the supervisor and gets a
	// red code error light.
	SPI1->CR2 = 0;
}

// initTimers - Initializes the TIM modules for the desired interrupt frequencies
static INLINE void initTimers() {
	uint32_t temp;
	// Turn on TIM4-6 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN | RCC_APB1ENR_TIM4EN | RCC_APB1ENR_TIM6EN;
	// Turn on TIM8 clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN | RCC_APB2ENR_TIM1EN;
	// Reset TIM4-6
	temp = RCC->APB1RSTR;
	RCC->APB1RSTR = temp | (RCC_APB1RSTR_TIM4RST | RCC_APB1RSTR_TIM5RST | RCC_APB1RSTR_TIM6RST);
	__dsb();
	// Clear reset
	RCC->APB1RSTR = temp;
	// Reset TIM8
	temp = RCC->APB2RSTR;
	RCC->APB2RSTR = temp | RCC_APB2RSTR_TIM8RST | RCC_APB2RSTR_TIM1RST;
	__dsb();
	// Clear reset
	RCC->APB2RSTR = temp;
	// Divide by 641, count up to 111
	// Do not set max to 127 - this is set up to match the Motor Controller 29s!
	TIM4->ARR = (uint16_t)110;
	TIM4->PSC = (uint16_t)640;
	// Enable OC1-OC4 as output compare, preload enabled, active then inactive when counting up
	// ("PWM mode 1")
	TIM4->CCMR1 = (uint16_t)0x6868;
	TIM4->CCMR2 = (uint16_t)0x6868;
	// Activate OC1-OC4 as active low signals (with the CCMR1-2 settings, this causes a right-
	// aligned active high pulse to appear on the output)
	TIM4->CCER = (uint16_t)0x3333;
	// Turn on TIM4
	TIM4->CR1 = TIM_CR1_CEN;
	// Maximum period, no divider [clocked from another timer]
	TIM5->ARR = (uint16_t)0xFFFF;
	TIM5->PSC = (uint16_t)0;
	// Enable slave mode to ITR3 (Timer 8)
	TIM5->SMCR = TIM_SMCR_SMS_EXTERNAL | TIM_SMCR_TS_ITR3;
	// Enable update interrupt
	TIM5->DIER = TIM_DIER_UIE;
	// Turn on TIM5
	TIM5->CR1 = TIM_CR1_CEN;
	// Maximum period, divide by 72
	TIM8->ARR = (uint16_t)0xFFFF;
	TIM8->PSC = (uint16_t)71;
	// On every overflow, trigger next timer
	TIM8->CR2 = TIM_CR2_MMS_UPDATE;
	// Turn on TIM8
	TIM8->CR1 = TIM_CR1_CEN;
	// Maximum period, divide by 2 (later configurable by user)
	TIM1->ARR = (uint16_t)0xFFFF;
	TIM1->PSC = (uint16_t)1;
	// Set up CCR1-4 as output compare PWM channels ("PWM mode 1")
	TIM1->CCMR1 = (uint16_t)0x6868;
	TIM1->CCMR2 = (uint16_t)0x6868;
	// Reset OC1-OC4 to off (can be enabled by the user)
	TIM1->CCER = (uint16_t)0;
	// Enable PWM outputs, set up break/dead time register
	TIM1->BDTR = (uint16_t)0x8100;
	// Turn on TIM1, enable preload of period register
	TIM1->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
	// Set up TIM6 for the DAC for the Wave driver
	// We divide 36000000 by 900 = 20 KHz update rate
	TIM6->PSC = 3;
	TIM6->ARR = 449;
	// At this rate we exhaust the 256 sample buffer in about 6.5ms
	// Since the reload takes <<1ms, we are fine on cpu usage
	TIM6->CR1 = TIM_CR1_ARPE;
	TIM6->CR2 = TIM_CR2_MMS_UPDATE;
}

// initMCU - Initialize the MCU
void initMCU() {
	initClocks();
	initPorts();
	initEXTI();
	initSPI();
	initSerial();
	initI2C();
	initTimers();
	initDMA();
	initADC();
	initDAC();
	initInterrupts();
}

// ISR_TIM8_CC - Timer 8 capture/compare (microsecond queue)
void ISR_TIM8_CC() {
	uint16_t sr = TIM8->SR & TIM8->DIER;
	if (sr & TIM_SR_CC1IF) {
		// CC1 fired one-time
		TIM8->DIER &= ~TIM_DIER_CC1IE;
		// Supervisor
		_svNextByte();
	}
	if (sr & TIM_SR_CC2IF) {
		// CC2 fired one-time
		TIM8->DIER &= ~TIM_DIER_CC2IE;
		// Apply motors
		_motorApply();
	}
	if (sr & TIM_SR_CC3IF) {
		// CC3 fired one-time
		TIM8->DIER &= ~TIM_DIER_CC3IE;
		// I2C timing event
		_i2cEnd();
	}
	if (sr & TIM_SR_CC4IF)
		// CC4 fired one-time, special handling due to recursive scheduling
		_ultrasonicTimeout();
}

// timeLowRes - Gets the low-resolution tick count (milliseconds)
clock_t timeLowRes() {
	return _clockLowRes;
}

// timeHighRes - Gets the high-resolution tick count (microseconds)
clock_t timeHighRes() {
	uint16_t highBits = TIM5->CNT, lowBits = TIM8->CNT, secondHighBits = TIM5->CNT;
	if (secondHighBits != highBits && lowBits < 0x7FFF)
		// Rollover compensation
		highBits = secondHighBits;
	return (clock_t)(((uint32_t)highBits << 16) | (uint32_t)lowBits);
}
