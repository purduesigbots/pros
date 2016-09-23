/*
 * encoder_isr.s - Interrupt service routines for encoder ISRs
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

.section .text._encoderISRBottom, "x"
.global _encoderISRBottom
.syntax unified
.thumb
.thumb_func

/* Invoked when the "bottom" port changes state */
_encoderISRBottom:
	push {r4, r5, lr}
	/* Save our pin so digitalRead will not smash it */
	mov r4, r0
	/* Read in our current value (bottom pin) */
	bl digitalRead
	mov r5, r0
	/* Load offset from _sensorState (pin starts at 1, so -16 moves it one element back to 0) */
	ldr	r3, =(_sensorState - 16)
	add	r4, r3, r4, lsl #4
	/* Check flags for the twos bit (enabled) and exit if false */
	ldrh r1, [r4, #4]
	tst r1, #2
	beq 2f
	/* Read in the other port (top pin) */
	ldrb r0, [r4, #6]
	bl digitalRead
	/* Load flags again and compare with reverse flag */
	ldrh r1, [r4, #4]
	and r1, r1, #1
	cmp r1, r0
	/* Load with either -1 or 1 according to reverse flag */
	ite	ne
	movne r2, #-1
	moveq r2, #1
	/* Test on initial value and swap if true */
	cbnz r5, 1f
	negs r2, r2
1:
	/* Find pin again */
	ldrb r0, [r4, #6]
	/* Recompute offset */
	ldr	r3, =(_sensorState - 16)
	add	r3, r3, r0, lsl #4
	ldr r0, [r3]
	/* Add offset and store again */
	adds r0, r0, r2
	str	r0, [r3]
2:
	/* Exit routine */
	pop {r4, r5, pc}

.section .text._encoderISRTop, "x"
.global _encoderISRTop
.syntax unified
.thumb
.thumb_func

/* Invoked when the "top" port changes state */
_encoderISRTop:
	push {r4, r5, lr}
	/* Save our pin so digitalRead will not smash it */
	mov r4, r0
	/* Read in our current value (top pin) */
	bl digitalRead
	mov r5, r0
	/* Load offset from _sensorState (pin starts at 1, so -16 moves it one element back to 0) */
	ldr	r3, =(_sensorState - 16)
	add	r4, r3, r4, lsl #4
	/* Check flags for the twos bit (enabled) and exit if false */
	ldrh r1, [r4, #4]
	tst r1, #2
	beq 2f
	/* Read in the other port (bottom pin) */
	ldrb r0, [r4, #7]
	bl digitalRead
	/* Load flags again and compare with reverse flag */
	ldrh r1, [r4, #4]
	and r1, r1, #1
	cmp r1, r5
	/* Load with either -1 or 1 according to reverse flag */
	ite	ne
	movne r2, #-1
	moveq r2, #1
	/* Test on initial value and swap if true */
	cbz r0, 1f
	negs r2, r2
1:
	/* Read in the count */
	ldr r0, [r4]
	/* Add offset and store again */
	adds r0, r0, r2
	str	r0, [r4]
2:
	/* Exit routine */
	pop {r4, r5, pc}
