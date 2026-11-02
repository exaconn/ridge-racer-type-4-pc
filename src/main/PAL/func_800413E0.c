#include "types.h"
#include "m2c_macros.h"

extern int func_8001ECE8();
extern s32 D_800AC5DC;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;

void func_800413E0(void) {
    func_8001ECE8(0, 0, 0, 2);
    D_800AC5DC = 0x80;
    D_800F4C6A = 1;
    D_800F3A26 = 8;
    D_800F4D8A = 0;
}
