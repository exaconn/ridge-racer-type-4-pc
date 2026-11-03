/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/trig.h"

s32 func_80090424(s32 a) {
    int r;
    if (a >= 0) {
        r = func_80090460(a & 0xFFF);
    } else {
        r = -func_80090460((0U - (u32)a) & 0xFFFU);
    }
    return r;
}
