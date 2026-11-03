/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/trig.h"

s32 func_80090460(s32 a) {
    if (a < 0x801) {
        if (a < 0x401) return D_800A68C0[a];
        return D_800A68C0[0x800 - a];
    }
    if (a < 0xC01) return -D_800A58C0[a];
    return -D_800A68C0[0x1000 - a];
}
