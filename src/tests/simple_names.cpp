/**
 * \file tests/simple_names.cpp
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
	pros::ADIPort test1 (1, ADI_ANALOG_IN);
 	pros::ADIPort test2 (1, ADI_ANALOG_OUT);
 	pros::ADIPort test3 (1, ADI_DIGITAL_IN);
 	pros::ADIPort test4 (1, ADI_DIGITAL_OUT);
 	pros::ADIPort test5 (1, ADI_SMART_BUTTON);
 	pros::ADIPort test6 (1, ADI_SMART_POT);
 	pros::ADIPort test7 (1, ADI_LEGACY_BUTTON);
 	pros::ADIPort test8 (1, ADI_LEGACY_LINE_SENSOR);
 	pros::ADIPort test9 (1, ADI_LEGACY_LIGHT_SENSOR);
 	pros::ADIPort test10 (1, ADI_LEGACY_GYRO);
 	pros::ADIPort test11 (1, ADI_LEGACY_ACCELEROMETER);
 	pros::ADIPort test12 (1, ADI_LEGACY_SERVO);
 	pros::ADIPort test13 (1, ADI_LEGACY_PWM);
 	pros::ADIPort test14 (1, ADI_LEGACY_ENCODER);
 	pros::ADIPort test15 (1, ADI_LEGACY_ULTRASONIC);
 	pros::ADIPort test16 (1, ADI_TYPE_UNDEFINED);
 	pros::ADIPort test17 (1, ADI_ERR);

 	// Misc Simple Names
	pros::Controller master (CONTROLLER_MASTER);
	pros::Controller partner (CONTROLLER_PARTNER);
 	master.get_analog(ANALOG_LEFT_X);
 	master.get_analog(ANALOG_LEFT_Y);
 	master.get_analog(ANALOG_RIGHT_X);
 	master.get_analog(ANALOG_RIGHT_Y);
 	master.get_digital(DIGITAL_L1);
 	master.get_digital(DIGITAL_L2);
 	master.get_digital(DIGITAL_R1);
 	master.get_digital(DIGITAL_R2);
 	master.get_digital(DIGITAL_UP);
 	master.get_digital(DIGITAL_DOWN);
 	master.get_digital(DIGITAL_LEFT);
 	master.get_digital(DIGITAL_RIGHT);
 	master.get_digital(DIGITAL_X);
 	master.get_digital(DIGITAL_B);
 	master.get_digital(DIGITAL_Y);
 	master.get_digital(DIGITAL_A);

 	// Motors Simple Names
	pros::Motor motor (1);
 	int result = motor.get_faults() & MOTOR_FAULT_NO_FAULTS;
 	result = motor.get_faults() & MOTOR_FAULT_MOTOR_OVER_TEMP;
 	result = motor.get_faults() & MOTOR_FAULT_DRIVER_FAULT;
 	result = motor.get_faults() & MOTOR_FAULT_OVER_CURRENT;
 	result = motor.get_faults() & MOTOR_FAULT_DRV_OVER_CURRENT;
 	result = motor.get_flags() & MOTOR_FLAGS_NONE;
 	result = motor.get_flags() & MOTOR_FLAGS_BUSY;
 	result = motor.get_flags() & MOTOR_FLAGS_ZERO_VELOCITY;
 	result = motor.get_flags() & MOTOR_FLAGS_ZERO_POSITION;
 	motor.set_brake_mode(MOTOR_BRAKE_COAST);
 	motor.set_brake_mode(MOTOR_BRAKE_BRAKE);
 	motor.set_brake_mode(MOTOR_BRAKE_HOLD);
 	motor.set_brake_mode(MOTOR_BRAKE_INVALID);
 	motor.set_encoder_units(MOTOR_ENCODER_DEGREES);
 	motor.set_encoder_units(MOTOR_ENCODER_ROTATIONS);
 	motor.set_encoder_units(MOTOR_ENCODER_COUNTS);
 	motor.set_encoder_units(MOTOR_ENCODER_INVALID);
 	motor.set_gearing(MOTOR_GEARSET_36);
 	motor.set_gearing(MOTOR_GEARSET_18);
 	motor.set_gearing(MOTOR_GEARSET_06);
 	motor.set_gearing(MOTOR_GEARSET_6);
 	motor.set_gearing(MOTOR_GEARSET_INVALID);

 	// RTOS Simple Names
	pros::Task task (CURRENT_TASK);
 	int res = task.get_state() & TASK_STATE_RUNNING;
 	res = task.get_state() & TASK_STATE_READY;
 	res = task.get_state() & TASK_STATE_BLOCKED;
 	res = task.get_state() & TASK_STATE_SUSPENDED;
 	res = task.get_state() & TASK_STATE_DELETED;
 	res = task.get_state() & TASK_STATE_INVALID;
 	task.notify_ext(0, NOTIFY_ACTION_NONE, NULL);
 	task.notify_ext(0, NOTIFY_ACTION_BITS, NULL);
 	task.notify_ext(0, NOTIFY_ACTION_INCR, NULL);
 	task.notify_ext(0, NOTIFY_ACTION_OWRITE, NULL);
 	task.notify_ext(0, NOTIFY_ACTION_NO_OWRITE, NULL);

 	// Vision Simple Names
 	pros::vision_object_s_t object;
 	bool equal = object.type == VISION_OBJECT_NORMAL;
 	equal = object.type == VISION_OBJECT_COLOR_CODE;
 	equal = object.type == VISION_OBJECT_LINE;
	pros::Vision vision (1);
 	vision.set_zero_point(VISION_ZERO_TOPLEFT);
 	vision.set_zero_point(VISION_ZERO_CENTER);
 }
