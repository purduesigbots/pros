#include <stdexcept>
#include <string>

#include "v5_api.h"

#include "rtos/FreeRTOS.h"
#include "rtos/task.h"

extern "C" void throw_it() {
	throw std::runtime_error("RT err in function");
}

extern "C" void test_exceptions() {
	vexDisplayErase();
	vexDisplayString(0, "Starting test");
	throw_it();
	vexDisplayString(3, "it didn't work");
}
