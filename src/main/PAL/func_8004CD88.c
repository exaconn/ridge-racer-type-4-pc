#include "rr/gpu.h"

extern int D_800ACC14;

void func_8004CD88(unsigned short *a0, int a1, unsigned char *a2, unsigned char *a3, int a4) {
    RrPolyG4Packet *p = *(RrPolyG4Packet **)0x1F800000;
    int *a;
    p->len = 8;
    p->code = 0x3A;
    if (a1 == 0) {
        p->x0 = a0[0];
        p->y0 = a0[1];
        p->x1 = a0[0] + a0[2];
        p->y1 = a0[1] + (short)a0[3] / 2;
        p->x2 = a0[0];
        p->y2 = a0[1] + a0[3];
        p->x3 = a0[0] + a0[2];
        p->y3 = a0[1] + a0[3];
    } else {
        p->x0 = a0[0];
        p->y0 = a0[1];
        p->x1 = a0[0] + a0[2];
        p->y1 = a0[1];
        p->x2 = a0[0];
        p->y2 = a0[1] + a0[3];
        p->x3 = a0[0] + a0[2];
        p->y3 = a0[1] + (short)a0[3] / 2;
    }
    p->r0 = a2[0];
    p->g0 = a2[1];
    p->b0 = a2[2];
    p->r1 = a2[0];
    p->g1 = a2[1];
    p->b1 = a2[2];
    p->r2 = a3[0];
    p->g2 = a3[1];
    p->b2 = a3[2];
    p->r3 = a3[0];
    p->g3 = a3[1];
    p->b3 = a3[2];
    a = (int *)(a4 * 4 + D_800ACC14);
    *(int *)p = (*(int *)p & 0xFF000000) | (a[0x70 / 4] & 0xFFFFFF);
    *(volatile int *)&a[0x70 / 4] = (a[0x70 / 4] & 0xFF000000) | ((int)p & 0xFFFFFF);
    p++;
    *(RrPolyG4Packet **)0x1F800000 = p;
}
