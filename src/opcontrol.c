#include "main.h"

void opcontrol() {
	size_t jinx_ctr = 0;
	jinx_var_t ctr_var = jinx_create_variable(sizeof jinx_ctr, "ctr", "I", NULL);
	jinx_track_s_t* track_var = jinx_track_variable(ctr_var, &jinx_ctr);
	while (true) {
		jinx_ctr++;
		// dprintf(file, "hello world\n");
		delay(50);
	}
}
