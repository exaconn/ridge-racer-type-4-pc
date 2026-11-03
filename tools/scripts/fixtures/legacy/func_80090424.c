/* GCC_VERSION: 2.7.2 */
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

s32 func_80090460(s32 a) {
    if (a < 0x801) {
        if (a < 0x401) return D_800A68C0[a];
        return D_800A68C0[0x800 - a];
    }
    if (a < 0xC01) return -D_800A58C0[a];
    return -D_800A68C0[0x1000 - a];
}

