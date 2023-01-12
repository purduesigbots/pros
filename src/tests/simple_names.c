/**
 * \file tests/simple_names.c
 *
 * Test that all the PROS_USE_SIMPLE_NAMES definitions compile correctly
 *
 * Do not actually run this test - just make sure it compiles
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "main.h"

void opcontrol() {
	// ADI Simple Names
	adi_port_set_config(1, ADI_ANALOG_IN);
	adi_port_set_config(1, ADI_ANALOG_OUT);
	adi_port_set_config(1, ADI_DIGITAL_IN);
	adi_port_set_config(1, ADI_DIGITAL_OUT);
	adi_port_set_config(1, ADI_SMART_BUTTON);
	adi_port_set_config(1, ADI_SMART_POT);
	adi_port_set_config(1, ADI_LEGACY_BUTTON);
	adi_port_set_config(1, ADI_LEGACY_LINE_SENSOR);
	adi_port_set_config(1, ADI_LEGACY_LIGHT_SENSOR);
	adi_port_set_config(1, ADI_LEGACY_GYRO);
	adi_port_set_config(1, ADI_LEGACY_ACCELEROMETER);
	adi_port_set_config(1, ADI_LEGACY_SERVO);
	adi_port_set_config(1, ADI_LEGACY_PWM);
	adi_port_set_config(1, ADI_LEGACY_ENCODER);
	adi_port_set_config(1, ADI_LEGACY_ULTRASONIC);
	adi_port_set_config(1, ADI_TYPE_UNDEFINED);
	adi_port_set_config(1, ADI_ERR);

	// Misc Simple Names
	controller_is_connected(CONTROLLER_MASTER);
	controller_is_connected(CONTROLLER_PARTNER);
	controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_X);
	controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_Y);
	controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_X);
	controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_Y);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_L1);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_L2);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_R1);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_R2);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_UP);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_DOWN);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_LEFT);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_RIGHT);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_X);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_B);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_Y);
	controller_get_digital(CONTROLLER_MASTER, DIGITAL_A);

	// Motors Simple Names
	int result = motor_get_faults(1) & MOTOR_FAULT_NO_FAULTS;
	result = motor_get_faults(1) & MOTOR_FAULT_MOTOR_OVER_TEMP;
	result = motor_get_faults(1) & MOTOR_FAULT_DRIVER_FAULT;
	result = motor_get_faults(1) & MOTOR_FAULT_OVER_CURRENT;
	result = motor_get_faults(1) & MOTOR_FAULT_DRV_OVER_CURRENT;
	result = motor_get_flags(1) & MOTOR_FLAGS_NONE;
	result = motor_get_flags(1) & MOTOR_FLAGS_BUSY;
	result = motor_get_flags(1) & MOTOR_FLAGS_ZERO_VELOCITY;
	result = motor_get_flags(1) & MOTOR_FLAGS_ZERO_POSITION;
	motor_set_brake_mode(1, MOTOR_BRAKE_COAST);
	motor_set_brake_mode(1, MOTOR_BRAKE_BRAKE);
	motor_set_brake_mode(1, MOTOR_BRAKE_HOLD);
	motor_set_brake_mode(1, MOTOR_BRAKE_INVALID);
	motor_set_encoder_units(1, MOTOR_ENCODER_DEGREES);
	motor_set_encoder_units(1, MOTOR_ENCODER_ROTATIONS);
	motor_set_encoder_units(1, MOTOR_ENCODER_COUNTS);
	motor_set_encoder_units(1, MOTOR_ENCODER_INVALID);
	motor_set_gearing(1, MOTOR_GEARSET_36);
	motor_set_gearing(1, MOTOR_GEARSET_18);
	motor_set_gearing(1, MOTOR_GEARSET_06);
	motor_set_gearing(1, MOTOR_GEARSET_6);
	motor_set_gearing(1, MOTOR_GEARSET_INVALID);

	// RTOS Simple Names
	int res = task_get_state(CURRENT_TASK) & TASK_STATE_RUNNING;
	res = task_get_state(CURRENT_TASK) & TASK_STATE_READY;
	res = task_get_state(CURRENT_TASK) & TASK_STATE_BLOCKED;
	res = task_get_state(CURRENT_TASK) & TASK_STATE_SUSPENDED;
	res = task_get_state(CURRENT_TASK) & TASK_STATE_DELETED;
	res = task_get_state(CURRENT_TASK) & TASK_STATE_INVALID;
	task_notify_ext(CURRENT_TASK, 0, NOTIFY_ACTION_NONE, NULL);
	task_notify_ext(CURRENT_TASK, 0, NOTIFY_ACTION_BITS, NULL);
	task_notify_ext(CURRENT_TASK, 0, NOTIFY_ACTION_INCR, NULL);
	task_notify_ext(CURRENT_TASK, 0, NOTIFY_ACTION_OWRITE, NULL);
	task_notify_ext(CURRENT_TASK, 0, NOTIFY_ACTION_NO_OWRITE, NULL);

	// Vision Simple Names
	vision_object_s_t object;
	bool equal = object.type == VISION_OBJECT_NORMAL;
	equal = object.type == VISION_OBJECT_COLOR_CODE;
	equal = object.type == VISION_OBJECT_LINE;
	vision_set_zero_point(1, VISION_ZERO_TOPLEFT);
	vision_set_zero_point(1, VISION_ZERO_CENTER);
}
