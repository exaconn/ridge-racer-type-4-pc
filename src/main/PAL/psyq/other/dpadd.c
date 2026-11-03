/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/wide.h"

extern RrWide64 *func_8007DC04(RrWide64 *result, s32 direction, RrWide64 value, s32 count);
extern s32 func_8007E9C4(s32 category, s32 operation);

typedef union {
    double d;
    RrWide64 s;
} DpUnion;

double func_8007D7D4(double a_in, double b_in) {
    RrWide64 sp18;
    RrWide64 sp20;
    RrWide64 sp28;
    DpUnion sp30;
    DpUnion a;
    /* ASM_BARRIER: pin b to s0/s1 */
    register DpUnion b asm("$16");
    /* ASM_BARRIER: DFmode view of s0/s1 so dbr_schedule splits movdf */
    register double b_ret asm("$16");
    s32 s4;
    s32 s6;
    u32 s7;

    sp30.s.lo = 0;
    sp30.s.hi = 0;
    s7 = 0;
    a.d = a_in;
    b.d = b_in;

    if ((a.s.hi & 0x7FFFFFFF) == 0 && a.s.lo == 0) {
        return b.d;
    }
    if ((b.s.hi & 0x7FFFFFFF) == 0 && b.s.lo == 0) {
        return a.d;
    }

    s4 = (a.s.hi >> 20) & 0x7FF;
    s6 = (b.s.hi >> 20) & 0x7FF;

    if (s6 + 0x36 < s4) {
        return a.d;
    }
    if (s4 + 0x36 < s6) {
        /* ASM_BARRIER: block beq fallthrough delay slot so it steals lui from target */
        __asm__ volatile("");
        return b_ret;
    }

    sp18.hi = (a.s.hi & 0xFFFFF) | 0x100000;
    sp18.lo = a.s.lo;
    sp20.hi = (b.s.hi & 0xFFFFF) | 0x100000;
    sp20.lo = b.s.lo;

    if (a.s.hi & 0x80000000) {
        func_8007E494(&sp18, sp18);
    }
    if (b.s.hi & 0x80000000) {
        func_8007E494(&sp20, sp20);
    }

    func_8007DC04(&sp18, 0, sp18, 9);
    func_8007DC04(&sp20, 0, sp20, 9);

    if (s6 < s4) {
        func_8007DC04(&sp20, 1, sp20, s4 - s6);
    } else {
        func_8007DC04(&sp18, 1, sp18, s6 - s4);
        s4 = s6;
    }

    func_8007DB54(&sp18, sp18, sp20);

    if (sp18.hi < 0) {
        func_8007E494(&sp18, sp18);
        s7 = 0x80000000;
    } else if (sp18.hi == 0 && sp18.lo == 0) {
        return sp30.d;
    }

    {
        /* ASM_BARRIER: keep initial 0xE0000000 mask in v1 */
        register s32 init_mask asm("$3");
        /* ASM_BARRIER: load sp18.hi into v0 before materializing 0xE0000000 in v1 */
        register s32 hi_val asm("$2") = sp18.hi;
        /* ASM_BARRIER: order sp18.hi load before init_mask lui */
        __asm__("" : "+r"(hi_val));
        init_mask = 0xE0000000;
        /* ASM_BARRIER: hide 0xE0000000 in v1 from CSE so s0 reloads via lui */
        __asm__("" : "+r"(init_mask));
        if ((hi_val & init_mask) == 0) {
            /* ASM_BARRIER: keep loop mask in s0 */
            register s32 mask_e0 asm("$16") = 0xE0000000;
            do {
                func_8007DC04(&sp18, 0, sp18, 1);
                s4--;
            } while ((sp18.hi & mask_e0) == 0);
        }
    }

    if (sp18.hi & 0x40000000) {
        /* ASM_BARRIER: materialize &sp18 in a0 for branch delay slot */
        register RrWide64 *p18 asm("$4") = &sp18;
        /* ASM_BARRIER: materialize direction=1 in a1 before stack arg=1 */
        register s32 dir1 asm("$5");
        /* ASM_BARRIER: materialize stack count=1 in v0 */
        register s32 cnt1 asm("$2");
        /* ASM_BARRIER: order a0 before a1 */
        __asm__("" : "+r"(p18));
        dir1 = 1;
        /* ASM_BARRIER: order a1 before v0 */
        __asm__("" : "+r"(dir1));
        cnt1 = 1;
        /* ASM_BARRIER: order stack store of cnt1 before loading sp18 into a2/a3 */
        __asm__("" : "+r"(cnt1), "+m"(sp18));
        func_8007DC04(p18, dir1, sp18, cnt1);
        s4++;
    }

    sp28.hi = 0;
    sp28.lo = (sp18.lo & 0x200) ? 0x100 : 0xFF;
    /* ASM_BARRIER: reload sp28.lo from stack while keeping sp28.hi=0 constant */
    __asm__("" : "+m"(sp28.lo));
    func_8007DB54(&sp18, sp18, sp28);

    if (sp18.hi & 0x40000000) {
        /* ASM_BARRIER: materialize &sp18 in a0 for branch delay slot */
        register RrWide64 *p18 asm("$4") = &sp18;
        /* ASM_BARRIER: materialize direction=1 in a1 before stack arg=1 */
        register s32 dir1 asm("$5");
        /* ASM_BARRIER: materialize stack count=1 in v0 */
        register s32 cnt1 asm("$2");
        /* ASM_BARRIER: order a0 before a1 */
        __asm__("" : "+r"(p18));
        dir1 = 1;
        /* ASM_BARRIER: order a1 before v0 */
        __asm__("" : "+r"(dir1));
        cnt1 = 1;
        /* ASM_BARRIER: order stack store of cnt1 before loading sp18 into a2/a3 */
        __asm__("" : "+r"(cnt1), "+m"(sp18));
        func_8007DC04(p18, dir1, sp18, cnt1);
        s4++;
    }

    func_8007DC04(&sp18, 1, sp18, 9);
    sp18.hi &= 0xFFEFFFFF;

    if (s4 >= 0x7FF) {
        func_8007E9C4(0x22, 0xB);
        a.s.hi = s7 ? 0xFFF00000 : 0x7FF00000;
        a.s.lo = 0;
    } else {
        a.s.hi = s7 | (s4 << 20) | sp18.hi;
        a.s.lo = sp18.lo;
    }

    return a.d;
}
