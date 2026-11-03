#include "types.h"
#include "m2c_macros.h"

#define D D_800F4DB8

typedef struct {
    char pad0[0x2664];
    int f2664;
    char pad1[0x2670 - 0x2668];
    int f2670;
    char pad2[0x2678 - 0x2674];
    int f2678;
    char pad3[0x2694 - 0x267C];
    int f2694;
    char pad4[0x269C - 0x2698];
    int f269C;
    char pad5[0x26A4 - 0x26A0];
    int f26A4;
    char pad6[0x26B0 - 0x26A8];
    int f26B0;
    int f26B4;
    int f26B8;
    char pad7[0x26C4 - 0x26BC];
    unsigned short h26C4;
    char pad8[0x26CE - 0x26C6];
    unsigned short h26CE;
} S;

extern struct { char p0[0x2670]; s32 x2670; char p1[0x4]; s32 x2678; char p2[0x20]; s32 x269C; s32 x26A0; char p3[0x4]; s32 x26A8; char p4[0x4]; s32 x26B0; s32 x26B4; char p5[0x4]; s32 x26BC; u16 x26C0; char p6[0xC]; u16 x26CE; } D_800F4DB8;
extern int func_80083CD4();
extern u8 D_800AD558;
extern int func_8008C140();
extern int func_8008F274();

void func_80052A30(int a0);
void func_80052AB8(void);
void func_80052ADC(u16 a0);
void func_80052B48(s32 arg0);
void func_80052B8C(u16 a0, int a1);
void func_80052C70();
void func_80052D50(int a0);
int func_80052D6C(void);
void func_80052DA8(void);

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

void func_80052AB8(void) {
    func_80052B8C(0, 0);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x26C4]; u16 v; } (*))&D_800F4DB8)

void func_80052ADC(u16 a0) {
    s16 s;
    if (D_800F4DB8.v != a0) {
        D_800F4DB8.v = (u8)a0;
        s = (D_800F4DB8.v * D_800AD558) / 16;
        func_80083CD4(0, s, s);
    }
}

#undef D_800F4DB8

void func_80052B48(s32 arg0) {
    s32 var_a0;
    u32 temp_a1;

    var_a0 = (arg0 & 0xFF) * D_800AD558;
    if (var_a0 < 0) {
        var_a0 += 0xF;
    }
    temp_a1 = (u32) var_a0 >> 4;
    func_80083CD4(0, temp_a1, temp_a1);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char p0[0x2670]; s32 x2670; char p1[0x4]; s32 x2678; char p2[0x20]; s32 x269C; char p3[0x8]; s32 x26A8; char p4[0x4]; s32 x26B0; s32 x26B4; char p5[0xC]; u16 x26C4; s16 x26C6; s16 x26C8; u16 x26CA; u16 x26CC; u16 x26CE; } (*))&D_800F4DB8)

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

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(S (*))&D_800F4DB8)

void func_80052C70()
{
    switch (D_800F4DB8.f269C) {
    case 0:
    case 1:
        if (D_800F4DB8.f2670 != 2) {
            D_800F4DB8.f2670 = 2;
            D_800F4DB8.f2678 = 0;
            D_800F4DB8.h26CE = D_800F4DB8.h26C4;
        }
        if (D_800F4DB8.f269C == 1) {
            D_800F4DB8.f26B8 = D_800F4DB8.f269C;
        } else {
            D_800F4DB8.f26B8 = 0;
            D_800F4DB8.f2694 = D_800F4DB8.f2664;
        }
        break;
    case 2:
        if (D_800F4DB8.f2670 != 3) {
            D_800F4DB8.f2670 = 3;
            D_800F4DB8.f26A4 = 0;
            D_800F4DB8.f2678 = 0;
        }
        break;
    }
    if (D_800F4DB8.f2670 == 1) {
        if (D_800F4DB8.f26B0 < 0x20) {
            D_800F4DB8.f26B4 = D_800F4DB8.f2670;
            return;
        }
        D_800F4DB8.f2670 = 2;
        D_800F4DB8.f2678 = 0;
        D_800F4DB8.h26CE = D_800F4DB8.h26C4;
    }
}

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x266C]; s32 a, b, c, d; } (*))&D_800F4DB8)

void func_80052D50(int a0) {
    D_800F4DB8.a = a0;
    D_800F4DB8.b = 6;
    D_800F4DB8.d = 0;
}

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x2674]; s32 a, b; char p[4]; s32 c; } (*))&D_800F4DB8)

int func_80052D6C(void) {
    if (D_800F4DB8.a != 0 || D_800F4DB8.b != 0) return 1;
    return D_800F4DB8.c;
}

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x266C]; int a; int b; int pp; int c; int d; int e; char pad2[0x14]; int g; } (*))&D_800F4DB8)

void func_80052DA8(void) {
    char sp[8];
    switch (D_800F4DB8.c) {
    case 0:
        sp[0] = 1;
        sp[1] = D_800F4DB8.a;
        if (func_8008C140(0xD, sp, 0) != 0) {
            D_800F4DB8.c++;
            return;
        }
        if (func_8008F274(1) == 0x10) {
            D_800F4DB8.d = 1;
            D_800F4DB8.e = -3;
            D_800F4DB8.b = -1;
            D_800F4DB8.c = 0;
            return;
        }
        return;
    case 1:
        D_800F4DB8.b = -1;
        D_800F4DB8.c = 0;
        D_800F4DB8.e = 0;
        D_800F4DB8.g = D_800F4DB8.a;
        break;
    }
}

#undef D_800F4DB8
