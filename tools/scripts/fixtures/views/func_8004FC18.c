#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct E (*)[])&D_800F4DB8)

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x18]; s16 a, b, c, d, e, f; } (*))&D_800F4DB8)

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x18]; unsigned short a; short b; short c; unsigned short d; unsigned short e; } (*))&D_800F4DB8)

#undef D_800F4DB8

extern struct {
    char pad00[0x24];
    short h24[0x1A];
    char pad0[0x1A08-0x24-0x34];
    int f1a08;
    int a1a0c[7];
    char pad2[0x1A2C - 0x1A28];
    int f1a2c;
    int f1a30;
    char pad3[0x2864 - 0x1A34];
    int f2864;
    int f2868;
} D_800F4DB8;

extern void func_80052B8C(int, short);

void func_8004FC18(short arg0, short arg1) {
    if (arg1 >= 2) {
        D_800F4DB8.b = arg1;
        D_800F4DB8.c = 0;
        D_800F4DB8.d = arg0;
        D_800F4DB8.e = D_800F4DB8.a;
    } else {
        D_800F4DB8.a = arg0;
        D_800F4DB8.b = 0;
        D_800F4DB8.c = 0;
        D_800F4DB8.d = 0;
        D_800F4DB8.e = 0;
        if (arg0 == 0) {
            func_8004FB00();
        }
    }
    func_80052B8C(arg0 & 0xFFFF, arg1);
}
