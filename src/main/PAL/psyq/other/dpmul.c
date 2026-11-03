/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "psyq/other/dpmul.h"

typedef struct {
    u32 lo;
    s32 hi;
} RrWide64;

typedef union {
    u64 bits;
    RrWide64 s;
} DpUnion;

extern RrWide64 func_8007DB54(RrWide64 left, RrWide64 right);
extern RrWide64 func_8007DC04(s32 direction, RrWide64 value, s32 count);
extern s32 _comp_mant(RrWide64 left, RrWide64 right);
extern RrWide64 _dbl_shift_us(s32 direction, RrWide64 value, s32 count);
extern RrWide64 func_8007E494(RrWide64 value);
extern s32 func_8007E9C4(s32 category, s32 operation);

u64 func_8007DCC4(u64 a_in, u64 b_in) {
    DpUnion a;
    DpUnion b;
    s32 s0;
    s32 s1;
    DpUnion r;
    u32 s4;
    RrWide64 sp18;
    RrWide64 sp20;
    RrWide64 sp28;
    RrWide64 sp30;
    RrWide64 sp38;

    a.bits = a_in;
    b.bits = b_in;

    s1 = ((a.s.hi >> 20) & 0x7FF) - ((b.s.hi >> 20) & 0x7FF) + 0x3FE;
    s4 = (a.s.hi & 0x80000000) ^ (b.s.hi & 0x80000000);

    if ((b.s.hi & 0x7FFFFFFF) == 0 && b.s.lo == 0) {
        r.s.hi = -1;
        if (s4 == 0) {
            r.s.hi = 0x7FFFFFFF;
        }
        r.s.lo = -1;
    } else if ((a.s.hi & 0x7FFFFFFF) == 0 && a.s.lo == 0) {
        r.s.hi = s4;
        r.s.lo = 0;
    } else {
        sp30.hi = (a.s.hi & 0xFFFFF) | 0x100000;
        sp30.lo = a.s.lo;
        sp38.hi = (b.s.hi & 0xFFFFF) | 0x100000;
        sp38.lo = b.s.lo;

        if (_comp_mant(sp30, sp38) < 0) {
            sp30 = func_8007DC04(0, sp30, 1);
            s1--;
        }

        sp20.hi = 0x200000;
        sp20.lo = 0;
        sp18.lo = 0;
        sp18.hi = 0;

        do {
            if (_comp_mant(sp30, sp38) >= 0) {
                sp18.hi |= sp20.hi;
                sp18.lo |= sp20.lo;
                sp30 = func_8007DB54(sp30, func_8007E494(sp38));
            }
            sp30 = func_8007DC04(0, sp30, 1);
            sp20 = _dbl_shift_us(1, sp20, 1);
        } while (sp20.hi != 0 || sp20.lo != 0);

        if (s1 >= 0) {
            s0 = 1;
            sp28.hi = 0;
            *(volatile u32 *)&sp28.lo = s0;
            sp18 = func_8007DB54(sp18, sp28);
            s1++;
            sp18 = _dbl_shift_us(1, sp18, s0);
        } else {
            s32 n = -s1;
            sp28.hi = 0;
            *(volatile u32 *)&sp28.lo = 1 << n;
            sp18 = func_8007DB54(sp18, sp28);
            s1 = 0;
            sp18 = _dbl_shift_us(1, sp18, n + 1);
        }

        sp18.hi &= 0xFFEFFFFF;

        if (s1 >= 0x7FF) {
            func_8007E9C4(0x22, 0xF);
            r.s.hi = s4 ? 0xFFF00000 : 0x7FF00000;
            r.s.lo = 0;
        } else {
            r.s.hi = (s4 | (s1 << 20)) | sp18.hi;
            r.s.lo = sp18.lo;
        }
    }

    return r.bits;
}
