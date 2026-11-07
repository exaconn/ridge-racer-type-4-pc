#ifndef RR_TIMING_H
#define RR_TIMING_H

#include "types.h"

/* VBlank counter storage. The wait routine compares its signed 32-bit view. */
extern volatile u32 Vcount;
s32 VSync(s32 mode);
void v_wait(s32 target, s32 budget);

#endif
