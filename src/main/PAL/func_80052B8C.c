#include "types.h"
extern int func_80052ADC();
extern struct { char p0[0x2670]; s32 x2670; char p1[0x4]; s32 x2678; char p2[0x20]; s32 x269C; char p3[0x8]; s32 x26A8; char p4[0x4]; s32 x26B0; s32 x26B4; char p5[0xC]; u16 x26C4; s16 x26C6; s16 x26C8; u16 x26CA; u16 x26CC; u16 x26CE; } D_800F4DB8;
void func_80052B8C(u16 a0, int a1) {
    if (a1 >= 2) {
        u16 v = D_800F4DB8.x26C4;
        D_800F4DB8.x26C6 = a1;
        D_800F4DB8.x26C8 = 0;
        D_800F4DB8.x26CA = a0;
        D_800F4DB8.x26CC = v;
    } else {
        func_80052ADC(a0);
        D_800F4DB8.x26C6 = 0;
        D_800F4DB8.x26C8 = 0;
        D_800F4DB8.x26CA = 0;
        D_800F4DB8.x26CC = 0;
        if (a0 == 0) {
            int r = D_800F4DB8.x269C;
            if (r != -1) {
                if (D_800F4DB8.x2670 != 1) {
                    D_800F4DB8.x2670 = 4;
                    D_800F4DB8.x2678 = 0;
                    D_800F4DB8.x26A8 = 0;
                    if (r == 2) func_80052ADC(D_800F4DB8.x26CE);
                } else {
                    if (D_800F4DB8.x26B0 < 0x20) {
                        D_800F4DB8.x26B4 = 2;
                    } else {
                        D_800F4DB8.x2670 = 4;
                        D_800F4DB8.x2678 = 0;
                        D_800F4DB8.x26A8 = 0;
                    }
                }
            }
        }
    }
}
