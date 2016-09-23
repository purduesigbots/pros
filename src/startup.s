/*
 * startup.s - Global PROS startup routines to initialize C data, then start the kernel
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

.section .text.ISR_Reset, "x"
.global ISR_Reset
.syntax unified
.thumb
.thumb_func

ISR_Reset:
	/* Initialize RAM data */
	ldr r2, =_sidata
	ldr r3, =_sdata
	ldr r1, =_edata
	b InitCheck
InitLoop:
	ldr r0, [r2], #4
	str r0, [r3], #4
InitCheck:
	cmp	r3, r1
	bcc InitLoop
	/* Clear BSS data to zero */
	ldr r3, =_sbss
	ldr r2, =_ebss
	mov r1, #0
	b BSSCheck
BSSLoop:
	str r1, [r3], #4
BSSCheck:
	cmp	r3, r2
	bcc BSSLoop
	/* Initialize the heap */
	ldr r0, =_heapbegin
	ldr r1, =_heapEnd
	str r0, [r1]
	/* Start the PROS */
	b startKernel

/* Vector table */
.section .isr_vector, "a", %progbits
.global VectorTable

VectorTable:
	.word _estack
	.word ISR_Reset
	.word ISR_NMI
	.word ISR_HardFault
	.word ISR_MemManage
	.word ISR_BusFault
	.word ISR_UsageFault
	.word 0
	.word 0
	.word 0
	.word 0
	.word ISR_SVC
	.word ISR_DebugMon
	.word 0
	.word ISR_PendSV
	.word ISR_SysTick
	.word ISR_WWDG
	.word ISR_PVD
	.word ISR_TAMPER
	.word ISR_RTC
	.word ISR_FLASH
	.word ISR_RCC
	.word ISR_EXTI0
	.word ISR_EXTI1
	.word ISR_EXTI2
	.word ISR_EXTI3
	.word ISR_EXTI4
	.word ISR_DMA1_Channel1
	.word ISR_DMA1_Channel2
	.word ISR_DMA1_Channel3
	.word ISR_DMA1_Channel4
	.word ISR_DMA1_Channel5
	.word ISR_DMA1_Channel6
	.word ISR_DMA1_Channel7
	.word ISR_ADC1_2
	.word ISR_USB_HP_CAN1_TX
	.word ISR_USB_LP_CAN1_RX0
	.word ISR_CAN1_RX1
	.word ISR_CAN1_SCE
	.word ISR_EXTI9_5
	.word ISR_TIM1_BRK
	.word ISR_TIM1_UP
	.word ISR_TIM1_TRG_COM
	.word ISR_TIM1_CC
	.word ISR_TIM2
	.word ISR_TIM3
	.word ISR_TIM4
	.word ISR_I2C1_EV
	.word ISR_I2C1_ER
	.word ISR_I2C2_EV
	.word ISR_I2C2_ER
	.word ISR_SPI1
	.word ISR_SPI2
	.word ISR_USART1
	.word ISR_USART2
	.word ISR_USART3
	.word ISR_EXTI15_10
	.word ISR_RTCAlarm
	.word ISR_USBWakeUp
	.word ISR_TIM8_BRK
	.word ISR_TIM8_UP
	.word ISR_TIM8_TRG_COM
	.word ISR_TIM8_CC
	.word ISR_ADC3
	.word ISR_FSMC
	.word ISR_SDIO
	.word ISR_TIM5
	.word ISR_SPI3
	.word ISR_UART4
	.word ISR_UART5
	.word ISR_TIM6
	.word ISR_TIM7
	.word ISR_DMA2_Channel1
	.word ISR_DMA2_Channel2
	.word ISR_DMA2_Channel3
	.word ISR_DMA2_Channel4_5
