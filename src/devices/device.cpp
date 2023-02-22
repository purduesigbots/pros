#include "pros/device.hpp"

#include "pros/apix.h"

namespace pros {
inline namespace v5 {

	std::uint8_t Device::get_port(void) {
		return _port;
	}

	
	pros::DeviceType Device::get_type() {
        return ((DeviceType) pros::c::registry_get_plugged_type(_port));
    }

	Device::Device(const std::uint8_t port) : _port(port) {}


}  // namespace v5
}  // namespace p