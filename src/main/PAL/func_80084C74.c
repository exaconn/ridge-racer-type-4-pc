/* CC1_FLAGS: -mno-split-addresses */
#include "rr/spu.h"

u32 func_80084C74(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sum;
    s32 diff;
    s32 q;
    s32 shift;
    s32 rem;
    /* ASM_BARRIER: Keep the adjusted remainder in $v1 before the halfword sign-extension shift. */
    register s32 rem2 __asm__("$3");
    /* ASM_BARRIER: Keep the negated shift count in $a0 for the rounding shift. */
    register s32 neg_shift __asm__("$4");
    /* ASM_BARRIER: Keep the scaled product in $a1 across the final shift and mask. */
    register u32 res __asm__("$5");

    arg3 += arg1;
    sum = arg3 & 0xFFFF;
    diff = (s16)((arg2 + ((u32)sum >> 7)) - arg0);
    arg3 = ({
        /* ASM_BARRIER: Keep the masked sum in $v1 for both the octave shift and fine mask. */
        register s32 s __asm__("$3") = sum;
        __asm__("" : "+r"(s));
        s & 0x7F;
    });
    q = diff / 12;
    shift = q - 2;
    /* ASM_BARRIER: Compute initial shift before the remainder check. */
    __asm__("" : "+r"(shift));
    rem = diff - (q * 12);
    if ((s16)rem < 0) {
        rem2 = rem + 12;
        shift = q - 3;
    } else {
        rem2 = rem;
    }
    /* ASM_BARRIER: Keep the unadjusted remainder live in $a0 so the positive path copies into $v1 in the delay slot. */
    __asm__("" : "+r"(rem2) : "r"(rem));
    res = (s32)(D_800A4D58[(s16)rem2] * D_800A4D70[(u16)arg3]) >> 16;
    if ((s16)shift >= 0) {
        res = 0x3FFF;
    } else {
        neg_shift = -(s16)shift;
        res += 1 << (neg_shift - 1);
        res >>= neg_shift;
    }
    return (u16)res;
}
