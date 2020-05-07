#include "system/user_functions.h"
#include "kapi.h"
#include "system/hot.h"

// how this all works...
// system daemon starts an autonomous task which calls user_autonomous()
// user_autonomous will invoke a hot-linked autonomous if one is available
// The invoked autonomous may actually just invoke user_cpp_autonomous
// which will invoke a C routine which calls C++ autonomous routine

// Our weak functions call C++ links of these functions, allowing users to only optionally extern "C" the task functions
// these are implemented in cpp_support.cpp
// FUNC(cpp_autonomous) expands to:
// extern void cpp_autonomous();
#define FUNC(NAME) extern void NAME();
#include "system/user_functions/cpp_list.h"
#undef FUNC

// default implementations of the different competition modes attempt to call
// the C++ linkage version of the function
// FUNC(autonomous) exapnds to:
// __attribute__((weak)) void autonomous() { user_cpp_autonomous(); }
#define FUNC(NAME)                    \
	__attribute__((weak)) void NAME() { \
		user_cpp_##NAME();                \
	}
#include "system/user_functions/c_list.h"
#undef FUNC

// FUNC(cpp_autonomous) exapnds to:
// void user_cpp_autonomous() {
//   if(HOT_TABLE && HOT_TABLE->functions.cpp_autonomous) {
//     HOT_TABLE->functions.cpp_autonomous();
//   } else {
//     cpp_autonomous();
//   }
// }
#define FUNC(NAME)                                \
	void user_##NAME() {                            \
		if (HOT_TABLE && HOT_TABLE->functions.NAME) { \
			HOT_TABLE->functions.NAME();                \
		} else {                                      \
			NAME();                                     \
		}                                             \
	}
#include "system/user_functions/list.h"
#undef FUNC
