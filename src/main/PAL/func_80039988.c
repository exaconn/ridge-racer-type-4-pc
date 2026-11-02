#include "types.h"
#include "m2c_macros.h"

extern int func_80013890();
extern int func_8002E4F4();
extern int func_80031E3C();
extern int func_80033A74();
extern int func_80037110();
extern int func_8003A198();
extern void func_8003E928(int,int,int,int,int,int,int,int,int,int,...);
extern s32 D_800ABEB0;
extern M2C_UNK D_800ABEF8;
extern s32 D_800AC5DC;
extern s16 D_800AD50A;
extern s32 D_800F2DE4;

void func_80039988(void) {
    func_80013890(5);
    func_8003A198();
    func_80033A74(1);
    func_8002E4F4(&D_800ABEF8);
    M2C_FIELD(&D_800ABEF8, s32 *, 0x28) = 0x134;
    M2C_FIELD(&D_800ABEF8, s32 *, 0x24) = 0xA;
    M2C_FIELD(&D_800ABEF8, s32 *, 0x54) = 0xA00;
    func_80037110();
    func_8003E928(0, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
    func_8003E928(1, 0x74, 0x58, 0x1C, 0x20, 5, 0, 1, 0, 0x37);
    func_8003E928(2, 0x74, 0x80, 0x1C, 0x20, 5, 0, 1, 0, 0x38);
    D_800ABEB0 = 0;
    D_800F2DE4 = 0;
    D_800AD50A = 0x122;
    D_800AC5DC = 0x80;
    func_80031E3C();
}
