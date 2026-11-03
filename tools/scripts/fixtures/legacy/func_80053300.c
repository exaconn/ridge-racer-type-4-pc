#include "types.h"
extern int func_80052A30();
extern struct { char p0[0x2664]; s32 x2664; char p1[0x4]; s32 x266C; char p2[0x4]; s32 x2674; char p3[0xC]; s32 x2684; char p4[0x10]; s32 x2698; char p5[0x8]; s32 x26A4; } D_800F4DB8;
void func_80053300(void) {
    switch (D_800F4DB8.x26A4) {
    case 0:
        D_800F4DB8.x2674 = 0;
        D_800F4DB8.x26A4 = 1;
        D_800F4DB8.x2664 = D_800F4DB8.x2684;
        D_800F4DB8.x266C = D_800F4DB8.x2698;
        break;
    case 1:
        if (D_800F4DB8.x2674 == 0) {
            func_80052A30(1);
            D_800F4DB8.x26A4 = 2;
        }
        break;
    }
}
