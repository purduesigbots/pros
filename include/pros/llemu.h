#ifndef _PROS_LLEMU_H_
#define _PROS_LLEMU_H_

// TODO:? Should there be weak symbols for the C api in here as well?

#include "stdint.h"

/******************************************************************************/
/**                        LLEMU Conditional Include                         **/
/**                                                                          **/
/**   When the libvgl versions of llemu.h is present, common.mk will         **/
/**   define a macro which lets this file know that liblvgl's llemu.h   is   **/
/**   present. If it is, we conditionally include it so that it gets         **/
/**   included into api.h.                                                   **/
/******************************************************************************/
#ifdef _PROS_INCLUDE_LIBLVGL_LLEMU_H
#include "liblvgl/llemu.h"
#endif

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif//__cplusplus

/**
 * Displays a formatted string on the emulated three-button LCD screen.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO  - The LCD has not been initialized. Call lcd_initialize() first.
 * EINVAL - The line number specified is not in the range [0-7]
 *
 * \param line
 *        The line on which to display the text [0-7]
 * \param fmt
 *        Format string
 * \param ...
 *        Optional list of arguments for the format string
 *
 * \return True if the operation was successful, or false otherwise, setting
 * errno values as specified above.
 */
bool __attribute__((weak)) lcd_print(int16_t line, const char* fmt, ...)  {
    return false;
}

#ifdef __cplusplus
} // namespace c
} // namespace pros
} // extern "C"
#endif//__cplusplus

#endif // _PROS_LLEMU_H_
