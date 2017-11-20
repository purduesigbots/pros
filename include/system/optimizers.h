#pragma once

// See https://stackoverflow.com/q/109710 for discussion
#define likely(cond) __builtin_expect(!!(cond), 1)
#define unlikely(cond) __builtin_expect(!!(cond), 0)
