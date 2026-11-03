#include "types.h"
extern int func_80052ADC();extern int func_80052A30();extern s32 D_800F745C;
extern struct { char p0[0x2664]; s32 x2664; char p1[0x8]; s32 x2670; s32 x2674; char p2[0x1C]; s32 x2694; char p3[0x8]; s32 x26A0; s32 x26A4; char p4[0x14]; s32 x26BC; char p5[0xE]; u16 x26CE; } D_800F4DB8;
void func_80053254(void) {
    switch (D_800F4DB8.x26A4) {
    case 0:
        func_80052ADC(D_800F4DB8.x26CE);
        D_800F4DB8.x2664 = D_800F4DB8.x2694;
        D_800F4DB8.x2674 = 0;
        D_800F4DB8.x26A4++;
        break;
    case 1:
        if (D_800F4DB8.x2674 == 0) {
            if (D_800F4DB8.x26BC == 1) func_80052A30(D_800F4DB8.x26A0);
            else D_800F4DB8.x2670 = -1;
            D_800F745C = 0;
        }
        break;
    }
}
