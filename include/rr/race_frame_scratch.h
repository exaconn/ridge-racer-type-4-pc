#ifndef RR_RACE_FRAME_SCRATCH_H
#define RR_RACE_FRAME_SCRATCH_H

#include "rr/layout.h"
#include "rr/gpu_scratch_types.h"

/* Race frame scratch: the published frame value at 0x78. The first eight
 * bytes were the PS1 packet cursor / ordering-table words; on the host those
 * live in GpuScratch (RR_GPU_SCRATCH) and this view keeps a placeholder so
 * the remaining PS1 offsets are unchanged. */
typedef struct {
    u8 gpu_slot00[8];
    u8 reserved08[0x70];
    s32 word78;
} RaceFrameScratch;

RR_ASSERT_SIZE(RaceFrameScratch, 0x7C);
RR_ASSERT_OFFSET(RaceFrameScratch, word78, 0x78);

#endif
