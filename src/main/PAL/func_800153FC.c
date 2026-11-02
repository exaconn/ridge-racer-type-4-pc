#include "rr_asm.h"
extern int func_80014C3C();
extern int func_80014F34();
extern int func_80015110();
extern int func_80096464();
extern signed char D_800F4C68;
extern int D_800ABEB0;
extern int D_8009E0B0[];
void func_800153FC(a0, a1) char *a0; char *a1;
{
    int s0;
    int *p;
    char *q;
    int *o;
    int n;
    int a2;
    int t;
    int v;
    RR_GTE_SETTR(a1 + 0x14);
    if (D_800F4C68 != 0) {
        s0 = *(int *)(a0 + 0x64);
    } else {
        s0 = (*(short *)(*(int *)(a1 + 0x90) + 0x1D8) - 0x3E0) * 2;
        /* clamp s0 to [0, 0x1000]; out-of-line arm for the upper clamp reproduces block layout */
        if (s0 < 0) goto L1;
        if (s0 > 0x1000) goto E;
    L1:
        v = s0;
        if (s0 < 0) v = 0;
        goto L2;
    E:
        v = 0x1000;
    L2:
        s0 = v;
        s0 += func_80096464() & 0xF;
        *(int *)(a0 + 0x64) = s0;
    }
    RR_GTE_MTC2_IR0(s0);
    p = D_8009E0B0;
    q = (char *)D_8009E0B0 + 0x18;
    o = (int *)(a1 + 0x15C);
    n = 6;
    do {
        RR_GTE_LWC2_RGB(p);
        RR_GTE_DPCS_FC4(q);
        p++;
        q += 4;
        o++;
        RR_GTE_SWC2_RGB2(o);
        n--;
    } while (n > 0);
    a2 = *(short *)(*(int *)(a1 + 0x90) + 0x1D8);
    func_80014C3C(*(int *)(a0 + 0xC4), a1, a2 / 4);
    t = (D_800ABEB0 & 3) * 12 + 0x64;
    func_80015110(*(int *)a0 + t, a1);
    {
        int u = (func_80096464() & 3) - 1;
        *(int *)(a1 + 0xAC) = *(int *)(a1 + 0x14) + u;
    }
    {
        int w = (func_80096464() & 3) - 1;
        *(int *)(a1 + 0xB0) = *(int *)(a1 + 0x18) + w;
    }
    *(int *)(a1 + 0xB4) = *(int *)(a1 + 0x1C);
    RR_GTE_SETTR(a1 + 0xAC);
    func_80014F34(*(int *)a0 + 0x58, a1);
}
