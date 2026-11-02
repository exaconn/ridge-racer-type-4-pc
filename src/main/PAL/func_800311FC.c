#include "types.h"
#include "m2c_macros.h"

extern int func_8001ECE8();
extern s32 D_800AC5DC;
extern s16 D_800AD50A;
extern s32 D_800F2DE4;

void func_800311FC(void) {
    func_8001ECE8(0, 0, 0, 2);
    D_800F2DE4 = 0;
    D_800AC5DC = 0x80;
    D_800AD50A = 0x10C;
}
