#include "main.h"

void opcontrol() {
	pros::Vision vis(10);
	pros::delay(2000);
	pros::vision_color_code_t code;
	code = vis.create_color_code(1, 2, 3);
	printf("%d\n", code);
	while (true) {
		pros::vision_object_s_t obj_arr[5];
		vis.read_by_size(0, 5, obj_arr);
		for(int i=0; i < 5; i++) {
			printf("%o %d %d   ", obj_arr[i].signature, obj_arr[i].x_middle_coord, obj_arr[i].y_middle_coord);
		}
		printf("\n");
		pros::delay(20);
	}
}
