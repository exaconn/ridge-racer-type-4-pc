#include "rr/race.h"

extern int func_80025F84();
extern int func_8004B7E0();

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
