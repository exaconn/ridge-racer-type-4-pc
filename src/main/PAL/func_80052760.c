#include "types.h"
extern int func_8008C3B0();extern int func_8008DE84();extern int func_8008C6A4();extern char D_800A0784[];extern char D_800A07C4[];
extern struct { char p0[0x2634]; s32 x2634; s32 x2638; char p1[0x4]; s32 x2640; } D_800F4DB8;
void func_80052760(void) {
    struct { u8 pad[8]; u8 b[8]; u8 c[24]; } L;
    D_800F4DB8.x2638 = -1;
    D_800F4DB8.x2640 = -1;
    if (func_8008C3B0(1, 0, L.b) != 0) {
        u32 v = L.b[0];
        if ((v & 0x10) == 0) {
            if ((v & 1) == 0) {
                func_8008DE84(L.c, D_800A0784);
                D_800F4DB8.x2634 = func_8008C6A4(L.c) & 0xFFFFFF;
                D_800F4DB8.x2638 = (int)D_800A07C4;
            }
        }
    }
}
