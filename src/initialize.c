#include "main.h"

void on_center_button() {
  static bool pressed = false;
  pressed = !pressed;
  if (pressed) {
    lcd_set_text(2, "I was pressed!");
  } else {
    lcd_clear_line(2);
  }
}

void initialize() {
  lcd_initialize();
  lcd_set_text(1, "Hello Beta Testers!");

  lcd_register_btn1_cb(on_center_button);
}

// the following functions don't work presently because comp. control
// hasn't been fully implemented
void disabled() {}
void competition_initialize() {}
