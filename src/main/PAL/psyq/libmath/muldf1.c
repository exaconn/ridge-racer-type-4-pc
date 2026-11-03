/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/wide.h"

typedef union {
    double d;
    RrWide64 s;
} DpUnion;

extern s32 func_8007E9C4(s32 category, s32 operation);

RrWide64 *func_8007E494(RrWide64 *result,RrWide64 x);
double __muldf3(double a_in, double b_in);

RrWide64 *func_8007E494(RrWide64 *result,RrWide64 x) {
    RrWide64 one;
    one.hi=0;
    /* Qualified write views keep the real ABI-home stores and subsequent reloads. */
    *(volatile u32 *)&one.lo=1;
    *(volatile s32 *)&x.hi=~x.hi;
    *(volatile u32 *)&x.lo=~x.lo;
    func_8007DB54(&x,x,one);
    {
        u32 low=x.lo;
        s32 high=x.hi;
        result->lo=low;
        result->hi=high;
    }
    /* ASM_BARRIER: Publish both result words before forming the returned address. */
    __asm__("" : : "r"(result) : "$2", "$3", "memory");
    return result;
}

double __muldf3(double a_in, double b_in) {
    RrWide64 sp18;
    RrWide64 sp20;
    RrWide64 sp28;
    RrWide64 sp30;
    RrWide64 sp38;
    RrWide64 sp40;
    DpUnion sp48;
    DpUnion a;
    DpUnion b;
    u32 s0;
    u32 s1;
    u32 s2;
    u32 s6;
    s32 s7;
    u32 fp;

    a.d = a_in;
    b.d = b_in;
    fp = (a.s.hi & 0x80000000) ^ (b.s.hi & 0x80000000);

    if (((a.s.hi & 0x7FFFFFFF) == 0 && a.s.lo == 0) ||
        ((b.s.hi & 0x7FFFFFFF) == 0 && b.s.lo == 0)) {
        sp48.s.hi = fp;
        sp48.s.lo = 0;
        return sp48.d;
    }

    s7 = ((a.s.hi >> 20) & 0x7FF) + ((b.s.hi >> 20) & 0x7FF) - 0x3FE;
    /* ASM_BARRIER: keep exponent calculation at top of block */
    __asm__ volatile("" : : "r"(s7));

    sp28.hi = (a.s.hi & 0xFFFFF) | 0x100000;
    sp28.lo = a.s.lo;
    sp30.hi = (b.s.hi & 0xFFFFF) | 0x100000;
    sp30.lo = b.s.lo;

    _dbl_shift_us(&sp38, 1, sp28, 0x15);
    s6 = sp38.lo;
    _dbl_shift_us(&sp38, 1, sp30, 0x15);
    s2 = sp38.lo;

    s0 = sp28.lo & 0x1FFFFF;
    s1 = sp30.lo & 0x1FFFFF;

    _mul_mant_d(&sp18, s6, s2);
    _mul_mant_d(&sp40, s0, s2);
    _dbl_shift_us(&sp38, 1, sp40, 0x15);
    func_8007DB54(&sp18, sp18, sp38);
    _mul_mant_d(&sp40, s1, s6);
    _dbl_shift_us(&sp38, 1, sp40, 0x15);
    func_8007DB54(&sp18, sp18, sp38);

    {
        u32 sign_mask = 0x80000000;
        if (sp18.hi < 0) {
            sp20.hi = 0;
            sp20.lo = 0x400;
            func_8007DB54(&sp18, sp18, sp20);
            _dbl_shift_us(&sp18, 1, sp18, 0xB);
        } else {
            sp20.hi = 0;
            sp20.lo = 0x200;
            func_8007DB54(&sp18, sp18, sp20);
            if (sp18.hi & sign_mask) {
                _dbl_shift_us(&sp18, 1, sp18, 0xB);
            } else {
                _dbl_shift_us(&sp18, 1, sp18, 0xA);
                s7--;
            }
        }
    }

    sp18.hi &= 0xFFEFFFFF;

    if (s7 >= 0x7FF) {
        func_8007E9C4(0x22, 0xD);
        sp48.s.hi = fp ? 0xFFF00000 : 0x7FF00000;
        sp48.s.lo = 0;
    } else {
        sp48.s.hi = fp | (s7 << 20) | sp18.hi;
        sp48.s.lo = sp18.lo;
    }

    return sp48.d;
}
