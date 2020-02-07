
#define CATCH_CONFIG_RUNNER
#define CATCH_INTERNAL_CONFIG_NO_POSIX_SIGNALS

#include <iostream>

#include "catch.hpp"
#include "main.h"


void initialize()
{
	std::cout << "Hello from initialize" << std::flush;
	const char* args[] = {"./pros"};
	Catch::Session().run(sizeof(args) / sizeof(char*), args);
	exit(0);
}
