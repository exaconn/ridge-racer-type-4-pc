#include "types.h"
#include "m2c_macros.h"

extern s32 D_800A0218;
extern s32 D_800A021C;
extern s32 D_800A0220;
extern s32 D_800A0224;
extern s32 D_800ACC18;
extern s32 D_8010C1F0;

void func_8004E008(void) {
    D_800A021C = 0;
    D_800A0220 = 0;
    D_800A0224 = 0;
    D_800A0218 = 0;
    D_8010C1F0 = D_800ACC18;
}
