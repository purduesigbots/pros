#ifndef _PROS_LLEMU_H_
#define _PROS_LLEMU_H_

// TODO:? Should there be weak symbols for the C api in here as well?


/******************************************************************************/
/**                        LLEMU Conditional Include                         **/
/**                                                                          **/
/**   When the libvgl versions of llemu.h is present, common.mk will         **/
/**   define a macro which lets this file know that liblvgl's llemu.h   is   **/
/**   present. If it is, we conditionally include it so that it gets         **/
/**   included into api.h.                                                   **/
/******************************************************************************/
#ifdef _PROS_INCLUDE_LIBLVGL_LLEMU_H
#include "pros/llemu.h"
#endif

#endif // _PROS_LLEMU_H_
