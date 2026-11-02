#include "types.h"
#include "m2c_macros.h"

extern s16 D_800AD50C;
extern s32 D_800F4CE0;
extern s32 D_8010E730;

void func_80070440(void) {
    D_800AD50C = 1;
    D_800F4CE0 = -0x24;
    D_8010E730 = 0;
}
