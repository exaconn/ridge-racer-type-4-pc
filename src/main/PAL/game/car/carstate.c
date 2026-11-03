/* RODATA 0x80010468 0x20 */
#include "rr/race.h"
#include "game/sound/sndvab.h"
#include "game/system/pad.h"
#include "game/car/carstate.h"
#include "rr/menu_overlay_runtime.h"

#define B(o) (*(unsigned char *)(p + (o)))
#define S(o) (*(short *)(p + (o)))
#define U(o) (*(unsigned short *)(p + (o)))
#define I(o) (*(int *)(p + (o)))

extern int func_80025F84();
extern int D_800AD504;

void func_8002C328(char *p);

void func_8002C1D4(a0, a1) int a0; RrRaceVehicle *a1; {
    int a2 = -1;
    int v1;
    register int av asm("$3"); /* ASM_BARRIER: abs copy */
    int tt;
    int t;
    a1->state1A4 = a0;
    switch (a0) {
    case 1:
        a2 = 0;
        a1->stateTimer1A5 = 0x1E;
        a1->steerAngle1A2 = a1->steerInput1BA * 8;
        break;
    case 2:
        v1 = func_80025F84((short)a1->trackAngle1EC, a1->rot54);
        goto L;
    case 8:
        v1 = 0x400;
    L:
        if (v1 >= 0x401) v1 = 0x800 - v1;
        t = a1->input;
        if (t < 0x140) {
            a1->steerAngle1A2 = 0;
        } else {
            a1->steerAngle1A2 = ((t - 0x140) * v1) / 4096;
        }
        a1->stateTimer1A5 = 0x1E;
        tt = -(unsigned short)a1->steerAngle1A2; a1->steerAngle1A2 = tt; tt = (short)tt; av = tt; if (tt < 0) av = -av; v1 = av;
        a2 = 0;
        if (v1 >= 0x64) {
            a2 = 2;
            if (v1 < 0x12C) a2 = 1;
        }
        break;
    }
    if (a1 == &D_800ABEF8[0] || a1 == &D_800ABEF8[1]) {
        if (a2 != -1) {
            func_8004B7E0(a1->playerFlags218 & 1, 2, a2);
        }
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEF8 (*(char (*)[])&D_800ABEF8)

void func_8002C328(char *p) {
    int s1, s2, a1, v;

    if (B(0x1A4)) {
        B(0x1A5) = B(0x1A5) - 1;
        if (B(0x1A5) == 0) {
            B(0x1A4) = 0;
            S(0x1B4) = 0;
            S(0x1B6) = 0;
            S(0x1B0) = 0;
            S(0x1B2) = 0;
        }
        a1 = B(0x1A5);
        s1 = (a1 * S(0x1A2)) / 128;
        v = func_80090424((a1 * 0x3000) / 30);
        s2 = (v * s1) / 2048;
        switch (B(0x1A4)) {
        case 1:
            if (p == D_800ABEF8 || p == D_800ABEF8 + 0x320) {
                if (D_800AD504 != 2 && B(0x1A5) == 0x1D) {
                    a1 = S(0x1BA) * 4;
                    if (a1 > 0x64) a1 = 0x64;
                    func_8004F218(0x28, (short)a1, 0x40, -1);
                }
            }
        case 5:
            S(0x1B0) = s2 + s1;
            S(0x1B2) = (s2 + s1 / 2) / 2;
            a1 = S(0x1B0);
            if (a1 < 0) a1 += 3;
            a1 >>= 2;
            U(0x1AC) = U(0x1AC) + a1;
            U(0x1AE) = U(0x1AE) + U(0x1B2);
            break;
        case 2:
        case 8:
            if (S(0x1B8) == 0) S(0x1B6) = s2;
            U(0x1AE) = U(0x1AE) + U(0x1B6);
            break;
        case 3:
            v = s2 + s1;
            S(0x1B4) = v;
            U(0x1AC) = U(0x1AC) + v;
            break;
        }
    } else if (B(0x1A7)) {
        if (p == D_800ABEF8 || p == D_800ABEF8 + 0x320) {
            func_8004B7E0(I(0x218) & 1, 1, 1);
        }
        s1 = (short)U(0x1D8) >> 2;
        a1 = B(0x1A7);
        s1 = (a1 * s1) / 128;
        v = func_80090424((a1 * 0x3000) / 30);
        s2 = (v * s1) / 2048;
        if (S(0x1B8) == 0) S(0x1B6) = s2;
        U(0x1AE) = U(0x1AE) + U(0x1B6);
        B(0x1A7) = B(0x1A7) - 1;
    } else {
        S(0x1B6) = 0;
        if (p == D_800ABEF8 || p == D_800ABEF8 + 0x320) {
            func_8004B7E0(I(0x218) & 1, 0, 1);
        }
    }
}

#undef D_800ABEF8
