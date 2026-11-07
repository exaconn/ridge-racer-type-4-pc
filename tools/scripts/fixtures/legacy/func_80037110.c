#include "types.h"
#include "m2c_macros.h"

extern int SetModelTexturePage();
extern int InitBadgeRow();
extern int InitRect16Row();
extern int InitImageRow();
extern s32 D_800F2EC8;

void func_80037110(void) {
    SetModelTexturePage(0x19);
    D_800F2EC8 = 0;
    InitBadgeRow(0, 0x10, 0x158, 0x120, 0x3C, 0, 0x103);
    InitRect16Row(0, 0x10, 0x48, 0x120, 0x120, 0, 0);
    InitImageRow(0, -1, 0, 1);
}
