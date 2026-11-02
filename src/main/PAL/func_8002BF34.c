#include "rr/race.h"

extern int D_800AD504;
extern int func_80025F84();
extern int func_80090424();
extern int func_800904F4();
extern int func_8004F218();

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
