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

s32 _param(s32 a) {
    *D_800ABBC8 = a | 0x10000000;
    return *D_800ABBC4 & 0xFFFFFF;
}
