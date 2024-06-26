#include "main.h"

[[gnu::noinline]] static void thing_1(uint8_t i) {
    printf("thing_1(%i)\n", i);
    if (i == 0) {
        asm volatile("mov r0, #0 \n\tSTR sp, [r0]\n\t");
    } else {
        thing_1(i - 1);
        printf("%s", "Never prints!");
    }
}



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  printf("%s", "Hello world!");
	thing_1(10);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {}