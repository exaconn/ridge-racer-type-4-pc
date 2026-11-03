#include "types.h"
#include "game/sound/sndrst2.h"

extern int func_8004FE7C(), func_800857A4(), func_80085AEC();
extern struct { char p0[4]; int f4; char p1[8]; int f10; char p2[4]; short f18; char p3[0xA]; short f24; char p4[0x1996]; unsigned short f19BC, f19BE, f19C0, f19C2; int f19C4; short f19C8; } D_800F4DB8;
extern struct { char p0[0x31]; unsigned char f31; short f32; } D_800AD528;
extern short D_800F4D44;
extern s16 D_800A043C[];

void func_800517F8(int a0, int a1, int a2, int a3);

void func_80051554(void)
{
    unsigned short s1, s0;
    short t1, t2;
    int v0, v1, a0, a1, i;
    if (D_800F4DB8.f10 == 0) {
        s1 = D_800F4DB8.f19BC;
        s0 = D_800F4DB8.f19C0;
    } else {
        s1 = D_800F4DB8.f19BC + D_800F4DB8.f19BE;
        s0 = D_800F4DB8.f19C0 + D_800F4DB8.f19C2;
        if ((short)s1 >= 0x80) s1 = 0x7F;
        if ((short)s0 >= 0x80) s0 = 0x7F;
    }
    if (D_800AD528.f32 != 0) {
        v0 = ((short)s1 + (short)s0) / 2;
        s1 = v0;
        s0 = v0;
    }
    if (D_800F4DB8.f4 != 0) {
        s0 = 0;
        s1 = 0;
    }
    v1 = (short)s0 * D_800F4DB8.f18;
    if (v1 < 0) v1 += 0x7F;
    a0 = (short)s1 * D_800F4DB8.f18;
    t1 = v1 >> 7;
    if (a0 < 0) a0 += 0x7F;
    v1 = t1 * D_800AD528.f31;
    t2 = a0 >> 7;
    if (v1 < 0) v1 += 0xF;
    v0 = t2 * D_800AD528.f31;
    s0 = (unsigned)v1 >> 4;
    if (v0 < 0) v0 += 0xF;
    s1 = (unsigned)v0 >> 4;
    if (D_800F4D44 == 5) {
        s0 = 0;
        s1 = 0;
    }
    if ((short)s1 + (short)s0 > 0) {
        i = 0;
        do {
            func_8004FE7C(i + 8, (short)s1, (short)s0);
            i++;
        } while (i < 2);
        if (D_800F4DB8.f19C4 == 0) {
            func_800857A4(8, D_800F4DB8.f24, D_800F4DB8.f19C8, 0, 0x3C, 0, (short)s1, (short)s0);
            func_800857A4(9, D_800F4DB8.f24, D_800F4DB8.f19C8, 1, 0x3C, 0, (short)s1, (short)s0);
            D_800F4DB8.f19C4 = 1;
        }
    } else if (D_800F4DB8.f19C4 != 0) {
        func_80085AEC(8);
        func_80085AEC(9);
        D_800F4DB8.f19C4 = 0;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x19F8]; s16 a, b; s32 c, d, e; } (*))&D_800F4DB8)

void func_800517D8(void) {
    D_800F4DB8.a = 0;
    D_800F4DB8.b = 0;
    D_800F4DB8.c = 0;
    D_800F4DB8.d = 0;
    D_800F4DB8.e = 0;
}

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define func_80085AEC ((int (*)())func_80085AEC)
#define D_800F4DB8 (*(struct { char p0[0x19F8]; s16 x19F8; s16 x19FA; s32 x19FC; s32 x1A00; s32 x1A04; } (*))&D_800F4DB8)

void func_800517F8(int a0, int a1, int a2, int a3) {
    int x = D_800A043C[(s16)a3] * (s16)a2;
    int y = D_800A043C[(s16)(0x7F - a3)] * (s16)a2;
    D_800F4DB8.x19FC = a1;
    D_800F4DB8.x19F8 = y / 16129;
    D_800F4DB8.x19FA = x / 16129;
    if (a0 != D_800F4DB8.x1A04) {
        D_800F4DB8.x1A04 = a0;
        D_800F4DB8.x1A00 = 0;
    }
    if (a2 == 0) {
        func_80085AEC(6);
        D_800F4DB8.x1A00 = 0;
    }
}

#undef func_80085AEC
#undef D_800F4DB8
