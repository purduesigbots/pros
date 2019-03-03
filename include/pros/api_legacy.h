/**
 * \file pros/api_legacy.h
 *
 * PROS 2 Legacy API header
 *
 * Contains declarations for functions that are name-compatible with the PROS 2
 * API. Some functions from the PROS 2 API are not useful or cannot be
 * implemented in PROS 3, but most common functions are available.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2019, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_API_LEGACY_H_
#define _PROS_API_LEGACY_H_

#include "api.h"

#ifdef __cplusplus
#define _NAMESPACE pros::
#define _CNAMESPACE pros::c::
#else
#define _NAMESPACE
#define _CNAMESPACE
#endif

/**
 * From adi.h
 */
#define analogCalibrate(port) adi_analog_calibrate(port)
#define analogRead(port) adi_analog_read(port)
#define analogReadCalibrated(port) adi_analog_read_calibrated(port)
#define analogReadCalibratedHR(port) adi_analog_read_calibrated_HR(port)
#define digitalRead(port) adi_digital_read(port)
#define digitalWrite(port, value) adi_digital_write(port, value)
#define pinMode(port, mode) adi_pin_mode(port, mode)
#define adiMotorSet(port, speed) adi_motor_set(port, speed)
#define adiMotorGet(port) adi_motor_get(port)
#define adiMotorStop(port) adi_motor_stop(port)
#define encoderGet(enc) adi_encoder_get(enc)
#define encoderInit(portTop, portBottom, reverse) adi_encoder_init(portTop, portBottom, reverse)
#define encoderShutdown(enc) adi_encoder_shutdown(enc)
#define ultrasonicGet(ult) adi_ultrasonic_get(ult)
#define ultrasonicInit(portEcho, portPing) adi_ultrasonic_init(portEcho, portPing)
#define ultrasonicShutdown(ult) adi_ultrasonic_shutdown(ult)

typedef _CNAMESPACE adi_encoder_t Encoder;
typedef _CNAMESPACE adi_ultrasonic_t Ultrasonic;

/**
 * From llemu.h
 */
#define lcdInit lcd_initialize
#define lcdReadButtons lcd_read_buttons
#define lcdClear lcd_clear
#define lcdClearLine lcd_clear_line
#define lcdShutdown lcd_shutdown
#define lcdPrint(line, fmt, ...) lcd_print(line, fmt, __VA_ARGS__)
#define lcdSetText(line, text) lcd_set_text(line, text)

/**
 * From misc.h
 */
#define isEnabled() (!competition_is_disabled())
#define isAutonomous competition_is_autonomous
#define isOnline competition_is_connected
#define isJoystickConnected(id) controller_is_connected(id)
#define joystickGetAnalog(id, channel) controller_get_analog(id, channel)

/**
 * From rtos.h
 */
#define taskCreate(taskCode, stackDepth, parameters, priority) \
	task_create(taskCode, parameters, priority, stackDepth, "")
#define taskDelete(task) task_delete(task)
#define taskDelay task_delay
#define taskDelayUntil(previousWakeTime, cycleTime) task_delay_until(previousWakeTime, cycleTime)
#define taskPriorityGet(task) task_get_priority(task)
#define taskPrioritySet(task, newPriority) task_priority_set(task, newPriority)
#define taskGetState(task) task_get_state(task)
#define taskSuspend(task) task_suspend(task)
#define taskResume(task) task_resume(task)
#define taskGetCount task_get_count
#define mutexCreate mutex_create
#define mutexTake(mutex, blockTime) mutex_take(mutex, blockTime)
#define mutexGive(mutex) mutex_give(mutex)

typedef _NAMESPACE task_t TaskHandle;
typedef _NAMESPACE mutex_t Mutex;

/**
 * From motors.h
 */
#define motorSet(port, speed) motor_move(port, speed)
#define motorGet(port) motor_get_voltage(port)
#define motorStop(port) motor_move(port, 0)

#undef _NAMESPACE
#undef _CNAMESPACE

#endif  // _PROS_API_LEGACY_H_
