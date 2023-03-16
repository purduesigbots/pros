/**
 * \file devices/vdml_optical.cpp
 *
 * Contains functions for interacting with the VEX Optical sensor.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pros/optical.hpp"
#include "vdml/vdml.h"

namespace pros {
inline namespace v5 {
using namespace pros::c;

Optical::Optical(std::uint8_t port) : Device(port, DeviceType::optical) {}

double Optical::get_hue(){
  return optical_get_hue(_port);
}

double Optical::get_saturation(){
  return optical_get_saturation(_port);
}

double Optical::get_brightness(){
  return optical_get_brightness(_port);
}

std::int32_t Optical::get_proximity(){
  return optical_get_proximity(_port);
}

std::int32_t Optical::set_led_pwm(std::uint8_t value){
  return optical_set_led_pwm(_port, value);
}

std::int32_t Optical::get_led_pwm(){
  return optical_get_led_pwm(_port);
}

optical_rgb_s_t Optical::get_rgb(){
  return optical_get_rgb(_port);
}

optical_raw_s_t Optical::get_raw(){
  return optical_get_raw(_port);
}

optical_direction_e_t Optical::get_gesture(){
  return optical_get_gesture(_port);
}

optical_gesture_s_t Optical::get_gesture_raw(){
  return optical_get_gesture_raw(_port);
}

std::int32_t Optical::enable_gesture(){
  return optical_enable_gesture(_port);
}

std::int32_t Optical::disable_gesture(){
  return optical_disable_gesture(_port);
}


std::ostream& operator<<(std::ostream& os, pros::Optical& optical) {
  pros::c::optical_rgb_s_t rgb = optical.get_rgb(); 
  os << "Optical [";
  os << "port: " << optical.get_port();
  os << ", hue: " << optical.get_hue();
  os << ", saturation: " << optical.get_saturation();
  os << ", brightness: " << optical.get_brightness();
  os << ", proximity: " << optical.get_proximity();
  os << ", rgb: " << "{" << rgb.red << ","<< rgb.green << "," << rgb.blue << "}"; 
  os << "]";
  return os;
}

namespace literals {
const pros::Optical operator"" _opt(const unsigned long long int o) {
  return pros::Optical(o);
}
} //

} // namespace v5
} // namespace pros
