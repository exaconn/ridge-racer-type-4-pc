#include "types.h"
#include "m2c_macros.h"
#include "rr/race.h"

extern char *D_800ACC14;
extern unsigned short D_8009F01C[];
extern int func_8002BB88();
extern int func_80020E2C();
extern char D_8009F020[];
extern int D_800AD504;
extern int func_80025F84();
extern int func_80090424();
extern int func_800904F4();
extern int func_8004F218();

void func_8002BD18(int a, int b);
void func_8002BDB4(int t);
void func_8002BEF8(void *arg0);
void func_8002BF34();
void func_8002C11C(void *arg0, s16 arg1, s16 arg2);

void func_8002BD18(int a, int b)
{
    char *p = D_800ACC14 + (b * 120 + 0x226EC);
    char *q;
    int r;
    int i;
    int t;
    if (b == 0) {
        q = D_800ACC14 + 0x70;
    } else {
        q = D_800ACC14 + 0xB70;
    }
    r = func_8002BB88(a);
    i = a < 0x5DC;
    t = D_8009F01C[i];
    func_80020E2C(q, 0x111, *(short *)(p + 0xA), r, t);
}

void func_8002BDB4(int t)
{
    int a, b, c;
    /* ASM_BARRIER: pins the D_8009F020 base to $v0 (original allocates it before the digit temporaries) */
    register char *p asm("$2");

    a = t / 1000;
    b = a / 60;
    c = a % 60;
    a = t % 1000;
    p = D_8009F020;
    p[0] = b / 10 + '0';
    p[1] = b % 10 + '0';
    p[3] = c / 10 + '0';
    p[4] = c % 10 + '0';
    p[6] = a / 100 + '0';
    p[7] = a / 10 % 10 + '0';
    p[8] = a % 10 + '0';
}

void func_8002BEF8(void *arg0) {
    M2C_FIELD(arg0, s16 *, 0x1A0) = 0;
    M2C_FIELD(arg0, s8 *, 0x1A4) = 0;
    M2C_FIELD(arg0, s8 *, 0x1A5) = 0;
    M2C_FIELD(arg0, s16 *, 0x1A2) = 0;
    M2C_FIELD(arg0, s16 *, 0x19C) = 0;
    M2C_FIELD(arg0, s16 *, 0x19E) = 0;
    M2C_FIELD(arg0, s16 *, 0x198) = 0;
    M2C_FIELD(arg0, s16 *, 0x19A) = 0;
    M2C_FIELD(arg0, s16 *, 0x1B4) = 0;
    M2C_FIELD(arg0, s16 *, 0x1B6) = 0;
    M2C_FIELD(arg0, s16 *, 0x1B0) = 0;
    M2C_FIELD(arg0, s16 *, 0x1B2) = 0;
    M2C_FIELD(arg0, s16 *, 0x1B8) = 0;
    M2C_FIELD(arg0, s16 *, 0x1BA) = 0;
}

void func_8002BF34(a0, a1) RrRaceVehicle *a0; int a1; {
    short tv, s1, v1, s0;
    int w, w2, z, sx, sy, sc;
    unsigned short su;
    a0->shakeActive19C = 1;
    tv = 0x800 - a0->trackAngle1EC;
    s1 = (tv + (a0->lateralVelF4 < 0 ? -0x400 : 0x400)) & 0xFFF;
    if (a1 < 3) {
        v1 = a0->input;
        if (v1 > 0x320) {
            su = 0x708;
            if (v1 < 0x709) su = (unsigned short)a0->input;
            w = (short)su * func_80090424((short)func_80025F84(tv, a0->heading1D4));
            s0 = w / 65536 + 4;
        } else {
            s0 = 0xC;
            if (v1 < 0xF0) s0 = 0x1E;
        }
        sx = (func_80090424(s1) * s0) >> 4;
        sy = (func_800904F4(s1) * s0) >> 4;
        sc = 0x64;
        a0->shakeTimer19E = 0x1E;
    } else {
        sx = (func_80090424(s1) * 8) >> 4;
        sy = (func_800904F4(s1) * 8) >> 4;
        a0->shakeTimer19E = 0xF;
        sc = 0x32;
    }
    a0->shakeX198 = sx;
    a0->shakeY19A = sy;
    if (a0->flags1E4 & 0x10) {
        w2 = func_800904F4(func_80025F84(tv, a0->heading1D4)) >> 6;
        if (a0->lateralVelF4 < 0) w2 = -w2;
        z = w2 + 0x40;
        if (a0 == &D_800ABEF8[0] || a0 == &D_800ABEF8[1]) {
            if (D_800AD504 != 2) func_8004F218(0x21, sc, (short)z, -1);
        }
    }
}

void func_8002C11C(void *arg0, s16 arg1, s16 arg2) {
    M2C_FIELD(arg0, s16 *, 0x19C) = 2;
    M2C_FIELD(arg0, s16 *, 0x198) = arg1;
    M2C_FIELD(arg0, s16 *, 0x19A) = arg2;
    M2C_FIELD(arg0, s16 *, 0x19E) = 0xF;
}
