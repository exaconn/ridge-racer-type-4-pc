#include "types.h"

extern s64 func_8007D7D4(s64, s64);
extern s64 func_8007DCC4(s64, s64);
extern s32 func_8007E0B4(s64);
extern s64 func_8007E1B4(s32);
extern s32 func_8007E2D4(s64, s64);
extern s32 func_8007E3B4(s64, s64);
extern s64 func_8007E504(s64, s64);
extern s64 func_8007E904(s64);
extern s64 func_8007E944(s64, s64);

void func_8006F2E0(s32 a0, s32 a1, s32 a2) {
    s64 d2, d0, d1, d6, t0, d4, d2_out;
    /* ASM_BARRIER: Move d2 into $a0/$a1 so $s0/$s1 is freed for 1.0 constant before func_8007E504. */
    register s64 a0_d2 asm("$4");
    /* ASM_BARRIER: Load constant into $a2/$a3 before $s0/$s1 constant setup. */
    register s64 a2_c asm("$6");
    s64 c1, c4096, t_mul;
    s64 c_max, c_min;
    s32 r0, r1;

    d2 = func_8007E1B4(a2);
    d0 = func_8007E1B4(a0);
    d1 = func_8007E1B4(a1);
    a0_d2 = d2;
    a2_c = 0x40F0000000000000LL;
    c1 = 0x3FF0000000000000LL;
    /* ASM_BARRIER: Materialize 1.0 in $s0/$s1 after $a0..$a3 setup and before func_8007E504. */
    __asm__("" : "=r"(c1), "=r"(a0_d2), "=r"(a2_c) : "0"(c1), "1"(a0_d2), "2"(a2_c));
    d6 = func_8007E504(a0_d2, a2_c);
    d4 = func_8007DCC4(0x4170000000000000LL, func_8007E504(d6, func_8007E944(func_8007DCC4(c1, d1), func_8007DCC4(c1, d0))));
    t0 = func_8007DCC4(d6, d0);
    d2_out = func_8007E504(t0, func_8007E904(d4));
    a2_c = 0x40DFFFC000000000LL;
    /* ASM_BARRIER: Load 0x40DFFFC000000000LL into $a2/$a3 before moving d4 to $a0/$a1. */
    __asm__("" : "=r"(a2_c) : "0"(a2_c), "r"(d2_out));
    if (func_8007E2D4(d4, a2_c) > 0 || func_8007E3B4(d4, 0xC0E0000000000000LL) < 0) {
        /* ASM_BARRIER: Low word of $v0/$v1 return value for split argument setup. */
        register s32 v0_lo asm("$2");
        /* ASM_BARRIER: High word of $v0/$v1 return value for delay-slot move to $a1. */
        register s32 v1_hi asm("$3");
        /* ASM_BARRIER: Low word of $a0/$a1 argument. */
        register s32 a0_lo asm("$4");
        if (func_8007E2D4(d4, 0LL) > 0) {
            d4 = 0x40DFFFC000000000LL;
        } else {
            d4 = 0xC0E0000000000000LL;
        }
        d2_out = func_8007E504(t0, func_8007E904(d4));
        func_8007D7D4(d2_out, func_8007E504(d4, func_8007DCC4(d6, d1)));
        /* ASM_BARRIER: Bind $v0/$v1 from func_8007D7D4. */
        __asm__("" : "=r"(v0_lo), "=r"(v1_hi));
        a0_lo = v0_lo;
        a2_c = 0x3F30000000000000LL;
        c4096 = 0x40B0000000000000LL;
        /* ASM_BARRIER: Materialize 4096.0 in $s0/$s1 before func_8007E504 while leaving $a1 = $v1 for the call delay slot. */
        __asm__("" : "=r"(c4096), "=r"(a0_lo), "=r"(a2_c), "=r"(v1_hi) : "0"(c4096), "1"(a0_lo), "2"(a2_c), "3"(v1_hi));
        t_mul = ((s64 (*)(s32, s32, s64))func_8007E504)(a0_lo, v1_hi, a2_c);
        d2_out = func_8007D7D4(d2_out, func_8007E504(func_8007E944(c4096, t_mul), c4096));
    }
    c_max = 0x41DFFFFFFFC00000LL;
    /* ASM_BARRIER: Materialize INT_MAX double constant before argument setup. */
    __asm__ volatile("" : "=r"(c_max) : "0"(c_max) : "$4", "$5");
    if (func_8007E2D4(d2_out, c_max) > 0) {
        d2_out = c_max;
    }
    c_min = 0xC1E0000000000000LL;
    /* ASM_BARRIER: Materialize INT_MIN double constant before argument setup. */
    __asm__ volatile("" : "=r"(c_min) : "0"(c_min) : "$4", "$5");
    if (func_8007E3B4(d2_out, c_min) < 0) {
        d2_out = c_min;
    }
    r0 = func_8007E0B4(d4);
    r1 = func_8007E0B4(d2_out);
    __asm__ volatile("ctc2 %0, $27" : : "r"(r0));
    __asm__ volatile("ctc2 %0, $28" : : "r"(r1));
}
