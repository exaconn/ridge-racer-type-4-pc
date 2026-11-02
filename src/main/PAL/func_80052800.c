#include "types.h"
extern int func_8008C3B0();extern int func_80083CD4();extern int func_8008C12C();extern int func_80053A18();extern int func_80052ADC();extern int D_8010E220;
extern struct { char p0[0x2658]; u8 x2658; char p1[0xB]; s32 x2664; char p2[0x8]; s32 x2670; s32 x2674; s32 x2678; char p3[0x20]; s32 x269C; char p4[0x8]; s32 x26A8; char p5[0xC]; s32 x26B8; s32 x26BC; char p6[0x4]; s16 x26C4; s16 x26C6; s16 x26C8; s16 x26CA; char p7[0x19C]; s32 x2868; } D_800F4DB8;
void func_80052800(void) {
    D_800F4DB8.x2658 = 0xC8;
    func_8008C3B0(0xE, &D_800F4DB8.x2658, 0);
    func_80083CD4(0, 0x7F, 0x7F);
    func_8008C12C((int)func_80053A18);
    D_800F4DB8.x2664 = -1;
    D_800F4DB8.x2670 = -1;
    D_800F4DB8.x2674 = 0;
    D_800F4DB8.x2678 = 0;
    D_800F4DB8.x269C = -1;
    D_800F4DB8.x26A8 = 0;
    D_800F4DB8.x26B8 = 0;
    D_800F4DB8.x26BC = 0;
    D_800F4DB8.x26C4 = 0;
    D_800F4DB8.x26C6 = 0;
    D_800F4DB8.x26C8 = 0;
    D_800F4DB8.x26CA = 0;
    func_80052ADC(0x7F);
    D_800F4DB8.x2868 = -1;
    D_8010E220 = 0;
}
