#include "rr_asm.h"
#include "rr/cd.h"
/* CC1_FLAGS: -mno-split-addresses */
s32 func_8008DBB4(u8 *a0, u32 a1) {
    volatile unsigned char *p;
    *D_800A674C = 0;
    *D_800A6758 = 0x80;
    *D_800A6780 = 0x20943;
    /* Ordinary write views retain the original setup and teardown scheduling. */
    *(u32 *)D_800A675C = 0x1323;
    *D_800A6784 |= 0x8000;
    *D_800A6788 = a0;
    *D_800A678C = a1 | 0x10000;
    p = D_800A674C;
    /* ASM_BARRIER: orig has the load-delay nop BEFORE the loop label; maspsx emits it inside the loop */
    RR_NOP_DEP();
    do {
    } while (!(*p & 0x40));
    *D_800A6790 = 0x11000000;
    if (*D_800A6790 & 0x01000000) {
        do {
        } while (*D_800A6790 & 0x01000000);
    }
    *(u32 *)D_800A675C = 0x1325;
    return 0;
}
