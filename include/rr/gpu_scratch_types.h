#ifndef RR_GPU_SCRATCH_TYPES_H
#define RR_GPU_SCRATCH_TYPES_H

#include "rr/layout.h"

/* PAL scratchpad pointer words; native fixtures retain full pointer widths. */
typedef struct {
    u8 *cursor;
    u32 *order;
} GpuScratchPointers;

#if defined(mips)
RR_ASSERT_SIZE(GpuScratchPointers, 8);
RR_ASSERT_OFFSET(GpuScratchPointers, order, 4);
#endif

#endif
