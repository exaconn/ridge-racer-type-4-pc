#include "rr_asm.h"
extern int *func_80017310(), *func_80017528(), *func_800177E4(), *func_80017A90();
extern int func_8001E84C();
extern char D_800100D0[];
extern short *D_8009E1D4[];
extern short *D_8009E224[];
extern short D_800F3914;
extern int *D_8010BF68;

void func_80017CC0(a0) int a0; {
    int *s4;
    short *a1;
    int s1;
    int s2;
    int s3;
    int *p;
    int w;
    int r;
    int k;
    int *s0;
    s0 = (int *)0x1F80007C;
    s4 = D_8010BF68;
    RR_GTE_SETROT((int *)0x1F800028);
    RR_GTE_CLRTR();
    a1 = D_8009E1D4[D_800F3914];
    if (a1 != 0) {
        while (a1[0] < a0) a1 += 2;
        {
            int pv = a1[-1];
            int a3 = a1[1];
            int sum = pv + a3;
            if (sum == 0) {
                *s0 = 0;
            } else if (sum == 0x2000) {
                *s0 = 0x1000;
            } else {
                int a2 = a1[0];
                if (a0 == a2) {
                    *s0 = a3;
                } else {
                    int d = ((a2 - a0) << 12) / (a2 - a1[-2]);
                    RR_GTE_MTC2_IR0(d);
                    RR_GTE_MTC2_IR1((unsigned short)a1[1]);
                    RR_GTE_CTC2_FCR((unsigned short)a1[-1]);
                    RR_GTE_INTPL();
                    RR_GTE_SWC2_IR1(s0);
                }
            }
        }
    } else {
        *s0 = 0;
    }
    k = 0;
    RR_GTE_SETFC(k, k, k);
    a1 = D_8009E224[D_800F3914];
    if (a1 != 0) {
        while (a1[0] < a0) a1 += 2;
        s3 = a1[1];
        s1 = 0;
    } else {
        s1 = 0;
        s3 = *s4;
    }
    s2 = 4;
    if (s3 > 0) {
        do {
            p = (int *)((char *)s4 + s2);
            s0[3] = p[0];
            r = p[2];
            while (w = *(int *)r, r += 4, w != 0) {
                k = w & 0xFFFF;
                w = w >> 16;
                switch (k) {
                case 7: r = (int)func_80017310(r, s0, w); break;
                case 8: r = (int)func_80017528(r, s0, w); break;
                case 1: r = (int)func_800177E4(r, s0, w); break;
                case 11: r = (int)func_80017A90(r, s0, w); break;
                default: func_8001E84C(D_800100D0, k, w); return;
                }
            }
            s1++;
            s2 += 12;
        } while (s1 < s3);
    }
}
