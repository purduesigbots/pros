#include <cstdio>
#include <iostream>

// called by _exit in newlib_stubs.c
extern "C" void flush_output_streams() {
    fflush(stdout);
    std::cout.flush();
}