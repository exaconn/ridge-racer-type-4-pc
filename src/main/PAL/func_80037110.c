#include "types.h"
#include "m2c_macros.h"

extern int func_80014A8C();
extern int func_8003D228();
extern int func_8003DC9C();
extern int func_8003FAE4();
extern s32 D_800F2EC8;

void func_80037110(void) {
    func_80014A8C(0x19);
    D_800F2EC8 = 0;
    func_8003D228(0, 0x10, 0x158, 0x120, 0x3C, 0, 0x103);
    func_8003DC9C(0, 0x10, 0x48, 0x120, 0x120, 0, 0);
    func_8003FAE4(0, -1, 0, 1);
}
