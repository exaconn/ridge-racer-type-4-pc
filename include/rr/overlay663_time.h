#ifndef RR_OVERLAY663_TIME_H
#define RR_OVERLAY663_TIME_H
#include "types.h"
/* Ten-byte template 00'00"000\0. Separators and terminator are preserved.
 * All signed32 inputs are defined; digits outside the displayed range retain
 * the original unsigned-byte conversion rather than being clamped. */
extern u8 ovl663_D_80116874[10];
u8 *ovl663_func_80115EE8(s32 milliseconds);
#endif
