#include "types.h"
#include "m2c_macros.h"

extern int func_8002A344();
extern int func_8003687C();
extern s32 D_800AC5DC;
extern s32 D_800F2DE4;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;

void func_800369E8(void) {
    func_8002A344(0x10006);
    func_8003687C(0);
    D_800F2DE4 = 0;
    D_800AC5DC = 0x80;
    D_800F4C6A = 1;
    D_800F3A26 = 0xA;
    D_800F4D8A = 0;
}
