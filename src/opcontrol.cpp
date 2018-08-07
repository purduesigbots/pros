#include "main.h"

using namespace pros::literals;

void opcontrol() {
  pros::Controller master(pros::E_CONTROLLER_MASTER);
  auto left_mtr = 1_mtr;
  pros::Motor right_mtr(2);
  while (true) {
    pros::lcd::print(0, "%d %d %d",
                     (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
                     (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
                     (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
    int left = master.get_analog(ANALOG_LEFT_Y);
    int right = master.get_analog(ANALOG_RIGHT_Y);

    left_mtr = left;
    right_mtr = right;
    pros::delay(20);
  }
}
