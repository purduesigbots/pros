/**
 * \file devices/vdml_motors.cpp
 *
 * Contains functions for interacting with the V5 Motors.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"
#include "pros/motors.hpp"
#include "vdml/vdml.h"

namespace pros {
inline namespace v5 {
using namespace pros::c;


Motor::Motor(const std::int8_t port, const pros::v5::MotorGears gearset, const pros::v5::MotorUnits encoder_units)
    : Device(port, DeviceType::motor), _port(port) {
	if (gearset != pros::v5::MotorGears::invalid) {
		set_gearing(gearset);
	}
	if (encoder_units != pros::v5::MotorEncoderUnits::invalid) {
		set_encoder_units(encoder_units);
	}
}

std::int32_t Motor::operator=(std::int32_t voltage) const {
	return motor_move(_port, voltage);
}

std::int32_t Motor::move(std::int32_t voltage) const {
	return motor_move(_port, voltage);

}

std::int32_t Motor::move_absolute(const double position, const std::int32_t velocity) const {
	return motor_move_absolute(_port, position, velocity);
}

std::int32_t Motor::move_relative(const double position, const std::int32_t velocity) const {
	return motor_move_relative(_port, position, velocity);
}

std::int32_t Motor::move_velocity(const std::int32_t velocity) const {
	return motor_move_velocity(_port, velocity);
}

std::int32_t Motor::move_voltage(const std::int32_t voltage) const {
	return motor_move_voltage(_port, voltage);
}

std::int32_t Motor::brake(void) const {
	return motor_brake(_port);
}

std::int32_t Motor::modify_profiled_velocity(const std::int32_t velocity) const {
	return motor_modify_profiled_velocity(_port, velocity);
}

double Motor::get_actual_velocity(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR_F;
	}
	return motor_get_actual_velocity(_port);
}
std::vector<double> Motor::get_actual_velocity_all(void) const {
	std::vector<double> return_vector;
	return_vector.push_back(motor_get_actual_velocity(_port));
	return return_vector;
}

pros::v5::MotorBrake Motor::get_brake_mode(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return pros::v5::MotorBrake::invalid;
	}
	return static_cast<pros::v5::MotorBrake>(motor_get_brake_mode(_port));
}

std::vector<pros::v5::MotorBrake> Motor::get_brake_mode_all() const {
	std::vector<pros::v5::MotorBrake> return_vector;
	return_vector.push_back(static_cast<pros::v5::MotorBrake>(motor_get_brake_mode(_port)));
	return return_vector;
}

std::int32_t Motor::get_current_draw(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_current_draw(_port);
}
std::vector<std::int32_t> Motor::get_current_draw_all(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.push_back(motor_get_current_draw(_port));
	return return_vector;
}
std::int32_t Motor::get_current_limit(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_current_limit(_port);
}
std::vector<std::int32_t> Motor::get_current_limit_all(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.push_back(motor_get_current_limit(_port));
	return return_vector;
}

std::int32_t Motor::is_over_current(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_is_over_current(_port);
}

std::vector<std::int32_t> Motor::is_over_current_all(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.push_back(motor_is_over_current(_port));

	return return_vector;
}

std::int32_t Motor::get_direction(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	int ret = motor_get_direction(_port);
	ret = _port >= 0 ? ret : ret * -1;
	return ret;
}
std::vector<std::int32_t> Motor::get_direction_all(void) const {
	std::vector<std::int32_t> return_vector;
	int ret = motor_get_direction(_port);
	ret = _port >= 0 ? ret : ret * -1;
	return_vector.push_back(ret);
	return return_vector;
}

double Motor::get_efficiency(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR_F;
	}
	return motor_get_efficiency(_port);
}
std::vector<double> Motor::get_efficiency_all(void) const {
	std::vector<double> return_vector;
	return_vector.push_back(motor_get_efficiency(_port));
	return return_vector;
}

pros::v5::MotorUnits Motor::get_encoder_units(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return pros::v5::MotorUnits::invalid;
	}
	return static_cast<pros::v5::MotorUnits>(motor_get_encoder_units(_port));
}

std::vector<pros::v5::MotorUnits> Motor::get_encoder_units_all(void) const {
	std::vector<pros::v5::MotorUnits> return_vector;
	return_vector.push_back(static_cast<pros::v5::MotorUnits>(motor_get_encoder_units(_port)));
	return return_vector;
}

std::uint32_t Motor::get_faults(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_faults(_port);
}

std::vector<std::uint32_t> Motor::get_faults_all(void) const {
	std::vector<std::uint32_t> return_vector;
	return_vector.push_back(motor_get_faults(_port));
	return return_vector;
}

std::uint32_t Motor::get_flags(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_flags(_port);
}

std::vector<std::uint32_t> Motor::get_flags_all(void) const {
	std::vector<std::uint32_t> return_vector;
	return_vector.push_back(motor_get_flags(_port));
	return return_vector;
}

pros::v5::MotorGears Motor::get_gearing(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return pros::v5::MotorGears::invalid;
	}
	return static_cast<pros::v5::MotorGears>(motor_get_gearing(_port));
}
std::vector<pros::v5::MotorGears> Motor::get_gearing_all(void) const {
	std::vector<pros::v5::MotorGears> return_vector;
	return_vector.push_back(static_cast<pros::v5::MotorGears>(motor_get_gearing(_port)));
	return return_vector;
}

std::int32_t Motor::get_raw_position(std::uint32_t* const timestamp, std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_raw_position(_port, timestamp);
}

std::vector<std::int32_t> Motor::get_raw_position_all(std::uint32_t* const timestamp) const {
	std::vector<std::int32_t> return_vector;
	return_vector.push_back(motor_get_raw_position(_port, timestamp));
	return return_vector;
}

std::int32_t Motor::is_over_temp(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_is_over_temp(_port);
}

std::vector<std::int32_t> Motor::is_over_temp_all(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.push_back(motor_is_over_temp(_port));
	return return_vector;
}

double Motor::get_position(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR_F;
	}
	return motor_get_position(_port);
}
std::vector<double> Motor::get_position_all(void) const {
	std::vector<double> return_vector;
	return_vector.push_back(motor_get_position(_port));
	return return_vector;
}

double Motor::get_power(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_power(_port);
}

std::vector<double> Motor::get_power_all(void) const {
	std::vector<double> return_vector;
	return_vector.push_back(motor_get_power(_port));
	return return_vector;
}

std::int32_t Motor::is_reversed(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return _port < 0;
}
std::vector<std::int32_t> Motor::is_reversed_all(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.push_back(_port < 0);
	return return_vector;
}

double Motor::get_temperature(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR_F;
	}
	return motor_get_temperature(_port);
}

std::vector<double> Motor::get_temperature_all(void) const {
	std::vector<double> return_vector;
	return_vector.push_back(motor_get_temperature(_port));
	return return_vector;
}

double Motor::get_target_position(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_target_position(_port);
}

std::vector<double> Motor::get_target_position_all(void) const {
	std::vector<double> return_vector;
	return_vector.push_back(motor_get_target_position(_port));
	return return_vector;
}

double Motor::get_torque(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_torque(_port);
}
std::vector<double> Motor::get_torque_all(void) const {
	std::vector<double> return_vector;
	return_vector.push_back(motor_get_torque(_port));
	return return_vector;
}
std::int32_t Motor::get_target_velocity(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_target_velocity(_port);
}
std::vector<std::int32_t> Motor::get_target_velocity_all(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.push_back(motor_get_target_velocity(_port));
	return return_vector;
}

std::int32_t Motor::get_voltage(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_voltage(_port);
}
std::vector<std::int32_t> Motor::get_voltage_all(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.push_back(motor_get_voltage(_port));
	return return_vector;
}

std::int32_t Motor::get_voltage_limit(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_get_voltage_limit(_port);
}
std::vector<std::int32_t> Motor::get_voltage_limit_all(void) const {
	std::vector<std::int32_t> return_vector;
	return_vector.push_back(motor_get_voltage_limit(_port));
	return return_vector;
}
std::int8_t Motor::get_port(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR_BYTE;
	}
	return _port;
}

std::vector<std::int8_t> Motor::get_port_all(void) const {
	std::vector<std::int8_t> return_vector;
	return_vector.push_back(_port);
	return return_vector;
}

std::int32_t Motor::tare_position(const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_tare_position(_port);
}

std::int32_t Motor::set_brake_mode(const pros::motor_brake_mode_e_t mode, const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_set_brake_mode(_port, mode);
}

std::int32_t Motor::set_brake_mode(const pros::v5::MotorBrake mode, const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_set_brake_mode(_port, static_cast<pros::motor_brake_mode_e_t>(mode));
}

std::int32_t Motor::set_current_limit(const std::int32_t limit, const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_set_current_limit(_port, limit);
}

std::int32_t Motor::set_encoder_units(const pros::motor_encoder_units_e_t units, const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_set_encoder_units(_port, units);
}

std::int32_t Motor::set_encoder_units(const pros::v5::MotorUnits units, const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_set_encoder_units(_port, static_cast<motor_encoder_units_e_t>(units));
}

std::int32_t Motor::set_gearing(const motor_gearset_e_t gearset, const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_set_gearing(_port, gearset);
}

std::int32_t Motor::set_gearing(const pros::v5::MotorGear gearset, const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_set_gearing(_port, static_cast<motor_gearset_e_t>(gearset));
}

std::int32_t Motor::set_zero_position(const double position, const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_set_zero_position(_port, position);
}

std::int32_t Motor::set_reversed(const bool reverse, const std::uint8_t index) {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	std::int8_t abs_port = std::abs(_port);
	if (reverse) {
		_port = -abs_port;
	} else {
		_port = abs_port;

	}
	return PROS_SUCCESS;
}

std::int32_t Motor::set_voltage_limit(const std::int32_t limit, const std::uint8_t index) const {
	if (index != 0) {
		errno = EOVERFLOW;
		return PROS_ERR;
	}
	return motor_set_voltage_limit(_port, limit);
}

std::int32_t Motor::tare_position_all(void) const {
	return motor_tare_position(_port);
}

std::int32_t Motor::set_brake_mode_all(const pros::motor_brake_mode_e_t mode) const {
	return motor_set_brake_mode(_port, mode);
}

std::int32_t Motor::set_brake_mode_all(const pros::v5::MotorBrake mode) const {
	return motor_set_brake_mode(_port, static_cast<pros::motor_brake_mode_e_t>(mode));
}

std::int32_t Motor::set_current_limit_all(const std::int32_t limit) const {
	return motor_set_current_limit(_port, limit);
}

std::int32_t Motor::set_encoder_units_all(const pros::motor_encoder_units_e_t units) const {
	return motor_set_encoder_units(_port, units);
}

std::int32_t Motor::set_encoder_units_all(const pros::v5::MotorUnits units) const {
	return motor_set_encoder_units(_port, static_cast<motor_encoder_units_e_t>(units));
}

std::int32_t Motor::set_gearing_all(const motor_gearset_e_t gearset) const {
	return motor_set_gearing(_port, gearset);
}

std::int32_t Motor::set_gearing_all(const pros::v5::MotorGear gearset) const {
	return motor_set_gearing(_port, static_cast<motor_gearset_e_t>(gearset));
}

std::int32_t Motor::set_zero_position_all(const double position) const {
	return motor_set_zero_position(_port, position);
}

std::int32_t Motor::set_reversed_all(const bool reverse) {
	std::int8_t abs_port = std::abs(_port);
	if (reverse) {
		_port = -abs_port;
	} else {
		_port = abs_port;
	}
	return PROS_SUCCESS;
}

std::int32_t Motor::set_voltage_limit_all(const std::int32_t limit) const {
	return motor_set_voltage_limit(_port, limit);
}

std::int8_t Motor::size() const {
	return 1;
}

std::ostream& operator<<(std::ostream& os, pros::Motor& motor) {
	os << "Motor [";
	os << "port: " << motor.get_port();
	os << ", brake mode: " << (int)motor.get_brake_mode();
	os << ", current draw: " << motor.get_current_draw();
	os << ", current limit: " << motor.get_current_limit();
	os << ", direction: " << motor.get_direction();
	os << ", efficiency: " << motor.get_efficiency();
	os << ", encoder units: " << (int)motor.get_encoder_units();
	os << ", gearing: " << (int)motor.get_gearing();
	os << ", over temp: " << motor.is_over_temp();
	os << ", position: " << motor.get_position();
	os << ", reversed: " << motor.is_reversed();
	os << ", temperature: " << motor.get_temperature();
	os << ", torque: " << motor.get_torque();
	os << ", voltage: " << motor.get_voltage();
	os << "]";
	return os;
}

}  // namespace v5
namespace literals {
const pros::Motor operator"" _mtr(const unsigned long long int m) {
	return pros::Motor(m);
}
const pros::Motor operator"" _rmtr(const unsigned long long int m) {
	return pros::Motor(-m);
}
}  // namespace literals
}  // namespace pros
