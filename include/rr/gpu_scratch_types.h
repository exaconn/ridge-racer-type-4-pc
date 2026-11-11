#ifndef RR_GPU_SCRATCH_TYPES_H
#define RR_GPU_SCRATCH_TYPES_H

#include "rr/layout.h"

struct GpuDmaTag;
/* Packet cursor and current ordering table. On the PS1 these were the first
 * two scratchpad words; on the host they are the ordinary global GpuScratch,
 * reached through RR_GPU_SCRATCH. Scratch views keep an 8-byte placeholder
 * (u8 gpu_slot00[8]) at offset 0 so the remaining PS1 offsets are unchanged. */
typedef struct {
    u8 *cursor;
    struct GpuDmaTag *order;
} GpuScratchPointers;
extern GpuScratchPointers GpuScratch;
#define RR_GPU_SCRATCH (&GpuScratch)

#endif
