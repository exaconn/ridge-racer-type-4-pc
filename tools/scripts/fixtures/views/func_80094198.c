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

u32 get_ce(s32 a0, s32 a1) {
    /* ASM_BARRIER: Preserve the original coordinate result register. */
    register s32 v __asm__("$2");
    /* ASM_BARRIER: Preserve the original clamped y value register. */
    register s32 w __asm__("$5");
    /* ASM_BARRIER: Preserve the original packed y value register. */
    register s32 u __asm__("$3");
    s32 c;
    s32 t;
    s32 coordinate;
    register s32 shifted=(s32)((u32)a0<<16);
    coordinate=shifted>>16;
    if (coordinate >= 0) {
        c = D_800ABAC4 - 1 < coordinate;
        /* ASM_BARRIER: Preserve the separate unsigned limit reload after the signed comparison. */
        __asm__ volatile("" : "=r"(c) : "0"(c) : "memory");
        t = *(u16 *)&D_800ABAC4;
        if (c) v = t - 1; else v = a0;
    } else v=0;
    a0=v;
    shifted=(s32)((u32)a1<<16);
    coordinate=shifted>>16;
    if (coordinate >= 0) {
        c = D_800ABAC6 - 1 < coordinate;
        /* ASM_BARRIER: Preserve the separate unsigned limit reload after the signed comparison. */
        __asm__ volatile("" : "=r"(c) : "0"(c) : "memory");
        t = *(u16 *)&D_800ABAC6;
        u = a1 & 0x3FF;
        if (c) {
            /* ASM_BARRIER: Preserve the original y value on the unclamped path. */
            __asm__("" : : : "$5");
            w = t - 1;
            goto L;
        }
    } else {
        w = 0;
L:
        u = w & 0x3FF;
    }
    u <<= 10;
    v=a0 & 0x3FF;
    a0=(s32)0xE4000000U;
    v |= a0;
    return u | v;
}
