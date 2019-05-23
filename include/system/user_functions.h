#pragma once

#define FUNC(F) void user_ ##F();
#include "system/user_functions/list.h"
#undef FUNC
