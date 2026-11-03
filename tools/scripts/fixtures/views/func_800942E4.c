/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/gpu.h"

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

/* legacy typing of shared globals in this part of the unit */
#define func_80096444 ((int (*)())func_80096444)

#undef func_80096444

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((void *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABBC8 ((s32 *)D_800ABBC8)

#undef D_800ABBC8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABBC8 ((u32 *)D_800ABBC8)

#undef D_800ABBC8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABBC4 ((int *)D_800ABBC4)

#define D_800ABBC8 ((int *)D_800ABBC8)

#undef D_800ABBC4

#undef D_800ABBC8

s32 func_800942E4(u8 *address, s32 count) {
    u32 offset;
    u32 *configuration=D_800ABBE4;
    *configuration |= 0x08000000;
    *D_800ABBE0 = 0;
    offset = (u32)count * 4;
    offset -= 4;
    address=(u8 *)((unsigned long)address+(long)(s32)offset);
    *D_800ABBD8 = address;
    *D_800ABBDC = count;
    /* The final control store occupies the call delay slot in the original. */
    *(u32 *)D_800ABBE0 = 0x11000002;
    func_80095360();
    if (*D_800ABBE0 & 0x01000000) {
        /* ASM_BARRIER: Retire the first mask and prevent hoisting the loop mask across the busy gate. */
        __asm__("" : : : "$3", "$17");
        {
            u32 mask = 0x01000000;
            do {
                if (func_80095394() != 0) return -1;
            } while (*D_800ABBE0 & mask);
        }
    }
    return count;
}
