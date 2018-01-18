/**
 * main.h - User's main header file for global functions
 *
 * This header file allows multiple files to reference functions in other files
 * without needing to see the code (which would cause issues with multiple
 * definitions). You can include those function's prototypes here in this file
 * or create another header file and #include that here to make them visible
 * wherever you #include "main.h", which should generally be done in every
 * C/C++ file.
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include "api.h"

#ifdef __cplusplus
extern "C" {
#endif

void autonomous();
void opcontrol();
void initialize();
void disabled();
void competition_initialize();

#ifdef __cplusplus
}
#endif

#endif /* _MAIN_H_ */