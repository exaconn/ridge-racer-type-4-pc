/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/timing.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

#undef D_800A6350

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

#undef D_800A6350

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

#undef D_800A6350

extern s32 func_800963DC(const char *, ...);

extern s32 func_80096F84(s32), func_80096F94(s32,s32);

void func_8008B2CC(s32 a0, s32 a1) {
    volatile u32 cnt = (u32)a1 << 15;
    u32 m;
    if ((s32)D_800A6390 < a0) {
        /* ASM_BARRIER: keeps -1 in a register hoisted before the goto loop (original has li v1,-1 before the loop label) */
        __asm__("" : "=r"(m) : "0"(-1));
loop:
        cnt=cnt-1;
        if (cnt == m) {
            char *page=(char *)0x80010000U;
            /* ASM_BARRIER: Materialize the message page before its call-delay offset. */
            __asm__("" : "=r"(page) : "0"(page));
            func_800963DC(page+0x17F8);
            func_80096F84(0);
            func_80096F94(3, 0);
            return;
        }
        if ((s32)D_800A6390 < a0) goto loop;
    }
}
