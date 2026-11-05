/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/trig.h"

s32 rsin(s32 a) {
    int r;
    if (a >= 0) {
        r = sin_1(a & 0xFFF);
    } else {
        r = -sin_1((0U - (u32)a) & 0xFFFU);
    }
    return r;
}
