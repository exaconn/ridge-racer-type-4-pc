#include "types.h"
#include "psyq/other/dpmul.h"

typedef struct {
    u32 lo;
    s32 hi;
} Wide64;

typedef union {
    u64 bits;
    Wide64 s;
} DpUnion;

extern Wide64 _add_mant_d(Wide64 left, Wide64 right);
extern Wide64 _dbl_shift(s32 direction, Wide64 value, s32 count);
extern s32 _comp_mant(Wide64 left, Wide64 right);
extern Wide64 _dbl_shift_us(s32 direction, Wide64 value, s32 count);
extern Wide64 _mainasu(Wide64 value);
extern s32 _err_math(s32 category, s32 operation);

u64 __divdf3(u64 a_in, u64 b_in) {
    DpUnion a;
    DpUnion b;
    s32 s0;
    s32 s1;
    DpUnion r;
    u32 s4;
    Wide64 sp18;
    Wide64 sp20;
    Wide64 sp28;
    Wide64 sp30;
    Wide64 sp38;

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
            sp30 = _dbl_shift(0, sp30, 1);
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
                sp30 = _add_mant_d(sp30, _mainasu(sp38));
            }
            sp30 = _dbl_shift(0, sp30, 1);
            sp20 = _dbl_shift_us(1, sp20, 1);
        } while (sp20.hi != 0 || sp20.lo != 0);

        if (s1 >= 0) {
            s0 = 1;
            sp28.hi = 0;
            sp28.lo = s0;
            sp18 = _add_mant_d(sp18, sp28);
            s1++;
            sp18 = _dbl_shift_us(1, sp18, s0);
        } else {
            s32 n = -s1;
            sp28.hi = 0;
            sp28.lo = 1 << n;
            sp18 = _add_mant_d(sp18, sp28);
            s1 = 0;
            sp18 = _dbl_shift_us(1, sp18, n + 1);
        }

        sp18.hi &= 0xFFEFFFFF;

        if (s1 >= 0x7FF) {
            _err_math(0x22, 0xF);
            r.s.hi = s4 ? 0xFFF00000 : 0x7FF00000;
            r.s.lo = 0;
        } else {
            r.s.hi = (s4 | (s1 << 20)) | sp18.hi;
            r.s.lo = sp18.lo;
        }
    }

    return r.bits;
}
