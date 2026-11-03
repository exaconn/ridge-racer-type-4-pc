/* RODATA 0x80011160 0x14 */
#include "types.h"

extern struct {
    char pad0[0x2670];
    int f2670;
    char pad1[0x2678 - 0x2674];
    int f2678;
    char pad2[0x2688 - 0x267C];
    int f2688;
    char pad3[0x2694 - 0x268C];
    int f2694;
    char pad4[0x269C - 0x2698];
    int f269C;
    char pad5[0x26B8 - 0x26A0];
    int f26B8;
} D_800F4DB8;
extern unsigned int D_800F7430;
extern int func_80052ADC();
extern int func_8008C0D8();
extern int func_8008C140();
extern int func_80052A30();
extern s32 D_800F745C;

void func_80053120();
void func_80053254(void);
void func_80053300(void);

void func_80053120()
{
    int r;
    switch (D_800F7430) {
    case 0:
        D_800F4DB8.f269C = 2;
        func_80052ADC(0);
        D_800F4DB8.f2678 = D_800F4DB8.f2678 + 1;
    case 1:
        if (func_8008C0D8(1, 0) == 0) break;
        D_800F4DB8.f2678 = D_800F4DB8.f2678 + 1;
    case 2:
        if (func_8008C140(9, 0, 0) == 0) break;
        goto inc;
    case 3:
        r = func_8008C0D8(1, 0);
        if (r == 2) {
    inc:
            D_800F4DB8.f2678 = D_800F4DB8.f2678 + 1;
        } else if (r == 5) {
            D_800F7430 = 1;
        }
        break;
    case 4:
        D_800F4DB8.f2670 = -1;
        D_800F4DB8.f2678 = 0;
        if (D_800F4DB8.f26B8 == 1) D_800F4DB8.f2694 = D_800F4DB8.f2688;
        break;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char p0[0x2664]; s32 x2664; char p1[0x8]; s32 x2670; s32 x2674; char p2[0x1C]; s32 x2694; char p3[0x8]; s32 x26A0; s32 x26A4; char p4[0x14]; s32 x26BC; char p5[0xE]; u16 x26CE; } (*))&D_800F4DB8)

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

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char p0[0x2664]; s32 x2664; char p1[0x4]; s32 x266C; char p2[0x4]; s32 x2674; char p3[0xC]; s32 x2684; char p4[0x10]; s32 x2698; char p5[0x8]; s32 x26A4; } (*))&D_800F4DB8)

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

#undef D_800F4DB8
