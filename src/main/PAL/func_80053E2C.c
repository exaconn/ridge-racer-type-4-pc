#include "types.h"
extern int func_8001ECE8();
extern int func_80096434();
extern int D_800AC5DC;
extern struct { s32 a; char p[0x14]; s32 b; } D_800F2AB8;
void func_80053E2C(void) {
    func_8001ECE8(0, 0, 0, 3);
    D_800AC5DC = 0x80;
    D_800F2AB8.a = 0;
    func_80096434(&D_800F2AB8, 0x32C);
    D_800F2AB8.b = 0x100;
}
