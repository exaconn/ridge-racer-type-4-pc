/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"

extern s32 D_800ABC50;

void func_800973C4(s32 value) {
    D_800ABC50 = value;
}
