/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/gpu.h"
#include "rr/draw_environment.h"

int func_800940E0(int a0, int a1, int a2);
u32 func_80094100(s32 a0, s32 a1);
u32 func_80094198(s32 a0, s32 a1);
unsigned func_80094230(int a, int b);
u32 func_8009424C(RrGpuRectangleFields *a0);

int func_800940E0(int a0, int a1, int a2) {
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

u32 func_80094100(s32 a0, s32 a1) {
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
    a0=(s32)0xE3000000U;
    v |= a0;
    return u | v;
}

u32 func_80094198(s32 a0, s32 a1) {
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

unsigned func_80094230(int a, int b) { return 0xE5000000 | (b & 0x7FF) << 11 | (a & 0x7FF); }

u32 func_8009424C(RrGpuRectangleFields *a0)
{
  int v[4];
  int new_var;
  unsigned r;
  if (a0 == 0)
  {
    r = 0;
  }
  else
  {
    v[0] = ((u8 *)&a0->x)[0] >> 3;
    v[2] = ((-a0->w) & 0xFF) >> 3;
    v[1] = ((u8 *)&a0->y)[0] >> 3;
    v[3] = ((-a0->h) & 0xFF) >> 3;
    r = (((v[1] << 15) | ((v[0] << 10) | (new_var = 0xE2000000))) | (v[3] << 5)) | v[2];
  }
  return r;
}
