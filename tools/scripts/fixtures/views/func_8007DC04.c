/* GCC_VERSION: 2.7.2 */

#include "rr/wide.h"

typedef RrWide64 U;

U _dbl_shift(int dir, U v, int n) {
    register int i asm("$4");
    if (dir == 0) {
        if (n > 0) { i = 0; do {
            v.hi = (u32)v.hi << 1;
            if (v.lo & 0x80000000) v.hi |= 1;
            v.lo <<= 1;
            i++;
        } while (i < n); }
    } else {
        if (n > 0) { i = 0; do {
            v.lo >>= 1;
            if (v.hi & 1) v.lo |= 0x80000000;
            v.hi >>= 1;
            i++;
        } while (i < n); }
    }
    return v;
}
