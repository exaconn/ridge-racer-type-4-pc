#include "types.h"
#include "m2c_macros.h"

extern int func_80092CB8();
extern s16 D_800AC5D0;
extern s32 D_800AC5DC;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;
extern s32 D_8010BF7C;

void func_8001C0F8(void) {
    D_800AC5DC = 0x80;
    D_800AC5D0 = 0;
    D_8010BF7C = 0;
    func_80092CB8(1);
    D_800F4C6A = 0;
    D_800F3A26 = 2;
    D_800F4D8A = 0;
}
