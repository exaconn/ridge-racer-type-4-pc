#include "types.h"
extern int func_80052ADC();extern int func_80052B8C();extern int func_80052B48();
extern struct { char p0[0x2670]; s32 x2670; char p1[0x4]; s32 x2678; char p2[0x20]; s32 x269C; s32 x26A0; char p3[0x4]; s32 x26A8; char p4[0x4]; s32 x26B0; s32 x26B4; char p5[0x4]; s32 x26BC; u16 x26C0; char p6[0xC]; u16 x26CE; } D_800F4DB8;
#define D D_800F4DB8
void func_80052A30(int a0) {
    D.x2670 = 1;
    D.x26BC = 1;
    if (a0 == 1) D.x26A0 = a0;
    else D.x26A0 = 0;
    D.x2678 = 0;
    D.x26A8 = 0;
    D.x26B0 = 0;
    D.x26B4 = 0;
    if (D.x269C == 2) func_80052ADC(D.x26CE);
    else func_80052B8C(D.x26C0, 0);
    func_80052B48(0);
}
