#include "types.h"
#include "m2c_macros.h"

extern s32 D_800A0220;
extern s32 D_8010C1F0;

void func_8004D7B4(s32 arg0) {
    D_8010C1F0 = arg0;
    D_800A0220 = 0;
}
