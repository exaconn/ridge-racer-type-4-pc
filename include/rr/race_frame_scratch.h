#ifndef RR_RACE_FRAME_SCRATCH_H
#define RR_RACE_FRAME_SCRATCH_H

#include "rr/gpu_scratch_types.h"

/* Race overlays share the GPU pointers and the published frame value at 0x78.
 * Native fixtures retain full pointers; PAL offsets are checked on target. */
typedef struct {
    RrGpuScratchPointers pointers;
    u8 reserved08[0x70];
    s32 word78;
} RrRaceFrameScratch;

RR_ASSERT_SIZE(RrRaceFrameScratch, 0x7C);
RR_ASSERT_OFFSET(RrRaceFrameScratch, pointers, 0);
RR_ASSERT_OFFSET(RrRaceFrameScratch, word78, 0x78);

#endif
