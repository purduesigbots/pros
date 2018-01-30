#include "ifi/v5_api.h"
#include "kapi.h"

int32_t controller_is_connected(controller_id_e_t id) {
	return vexControllerConnectionStatusGet(id);
}

int32_t controller_get_analog(controller_id_e_t id, controller_analog_e_t channel) {
	return vexControllerGet(id, channel);
}

int32_t controller_get_digital(controller_id_e_t id, controller_digital_e_t button) {
	// the buttons enum starts at 4, the correct place for the libv5rts
	return vexControllerGet(id, button);
}
