/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 970318 */
#include "psyq/libgte/gteatan.h"

extern short ratan_tbl[]; /* ratan_tbl */

/* PsyQ libgte ratan2(y, x) */

s32 ratan2(s32 y, s32 x) {
    s32 v, r;
    int s1 = 0, s2 = 0;

    if (x < 0) {
        s1 = 1;
        x = -x;
    }
    if (y < 0) {
        s2 = 1;
        y = -y;
    }
    if (x == 0 && y == 0) {
        return 0;
    }
    if (y < x) {
        if (y & 0x7FE00000) {
            v = y / (x >> 10);
        } else {
            v = (y << 10) / x;
        }
        r = ratan_tbl[v];
    } else {
        if (x & 0x7FE00000) {
            v = x / (y >> 10);
        } else {
            v = (x << 10) / y;
        }
        r = 1024 - ratan_tbl[v];
    }
    if (s1) {
        r = 2048 - r;
    }
    if (s2) {
        r = -r;
    }
    return r;
}
