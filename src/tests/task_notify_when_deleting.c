#include "main.h"
#include "pros/apix.h"

void target_task(void* ignore) {
	lcd_print(0, "%s says hello", task_get_name(NULL));
	task_delay(1000);
	lcd_print(0, "I don't feel so good - %s", task_get_name(NULL));
}

void notify_task(void* ignore) {
	lcd_set_text(2, "I don't know - I don't know what's happening");
	task_notify_take(true, TIMEOUT_MAX);
	lcd_set_text(4, "God damn you all to hell!");
}

void opcontrol() {
	task_t peter = task_create(target_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Peter Parker");
	task_notify_when_deleting(peter, NULL, 1, E_NOTIFY_ACTION_BITS);

	task_notify_take(true, TIMEOUT_MAX);
	lcd_set_text(1, "Are you alright?");

	task_t peter2 = task_create(notify_task, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Peter Parker's Son");
	// task_delay(1000);
	task_notify_when_deleting(NULL, peter2, 1, E_NOTIFY_ACTION_INCR);
	// task_delay(1000);
	lcd_set_text(3, "Goodbye, cruel world!");
	// task_delay(1000);
}
