/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/gpu.h"

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

/* legacy typing of shared globals in this part of the unit */
#define PsyqMemcpy ((int (*)())PsyqMemcpy)

#undef PsyqMemcpy

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((void *)GpuFuncTable)

#undef GpuFuncTable

/* legacy typing of shared globals in this part of the unit */
#define GPU_STATUS ((s32 *)GPU_STATUS)

#undef GPU_STATUS

/* legacy typing of shared globals in this part of the unit */
#define GPU_STATUS ((u32 *)GPU_STATUS)

#undef GPU_STATUS

/* legacy typing of shared globals in this part of the unit */
#define GPU_DATA ((int *)GPU_DATA)

#define GPU_STATUS ((int *)GPU_STATUS)

#undef GPU_DATA

#undef GPU_STATUS

s32 _otc(u8 *address, s32 count) {
    u32 offset;
    u32 *configuration=DPCR;
    *configuration |= 0x08000000;
    *DMA6_CHCR = 0;
    offset = (u32)count * 4;
    offset -= 4;
    address=(u8 *)((unsigned long)address+(long)(s32)offset);
    *DMA6_MADR = address;
    *DMA6_BCR = count;
    /* The final control store occupies the call delay slot in the original. */
    *(u32 *)DMA6_CHCR = 0x11000002;
    set_alarm();
    if (*DMA6_CHCR & 0x01000000) {
        /* ASM_BARRIER: Retire the first mask and prevent hoisting the loop mask across the busy gate. */
        __asm__("" : : : "$3", "$17");
        {
            u32 mask = 0x01000000;
            do {
                if (get_alarm() != 0) return -1;
            } while (*DMA6_CHCR & mask);
        }
    }
    return count;
}
