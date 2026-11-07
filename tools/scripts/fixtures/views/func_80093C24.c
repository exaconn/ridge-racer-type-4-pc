/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "m2c_macros.h"

#include "rr/draw_environment.h"

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

void SetDrawMode(void *arg0, s32 arg1, s32 arg2, s32 arg3, RrGpuRectangleFields *arg4) {
    M2C_FIELD(arg0, s8 *, 3) = 2;
    M2C_FIELD(arg0, s32 *, 4) = get_mode(arg1, arg2, arg3 & 0xFFFF);
    M2C_FIELD(arg0, s32 *, 8) = get_tw(arg4);
}
