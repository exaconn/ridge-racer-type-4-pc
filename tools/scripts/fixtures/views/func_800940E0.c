/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/draw_environment.h"

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

int get_mode(int a0, int a1, int a2) {
    int v1;
    register int v0 asm("$2");
    v1 = 0xE1000000;
    if (a1 != 0) {
        v1 = 0xE1000200;
    }
    v0 = a2 & 0x9FF;
    if (a0 != 0) {
        v0 |= 0x400;
    }
    return v1 | v0;
}
