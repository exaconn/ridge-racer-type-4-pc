#ifndef RR_TIMING_H
#define RR_TIMING_H

#include "types.h"

/* VBlank counter storage. The wait routine compares its signed 32-bit view. */
extern volatile u32 D_800A6390;
s32 func_8008B154(s32 mode);
void func_8008B2CC(s32 target, s32 budget);

#endif
