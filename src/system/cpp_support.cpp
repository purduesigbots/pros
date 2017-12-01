#include <cstddef>
#include <cstdlib>
#include <stdexcept>

#include "ifi/v5_api.h"

#include "rtos/FreeRTOS.h"
#include "rtos/task.h"

void* operator new(size_t size) {
	return malloc(size);
}

void* operator new[](size_t size) {
	return malloc(size);
}

void operator delete(void* p) {
	free(p);
}

void operator delete[](void* p) {
	free(p);
}

extern "C" void task_fn_wrapper(task_fn_t fn, void* args) {
	try {
		fn(args);
	} catch (std::runtime_error re) {
		vexDisplayString(7, "caught runtime error: %s", re.what());
	} catch (...) {
		vexDisplayString(7, "caught an unknown error");
	}
}