#include "rr_asm.h"

typedef struct {
    int v01;
    unsigned short v2;
    unsigned short pad;
    int c0;
    int c1;
    int c2;
} Tri20;

typedef struct {
    char pad[0x2C];
    char *p2C;
    Tri20 *p30;
} S_8001AC24;

int *func_8001AC24(S_8001AC24 *a0, int *a1, int *a2) {
    Tri20 *p = a0->p30;
    char *verts = a0->p2C;
    /* ASM_BARRIER: allocate count to $t5 ($13) so verts gets $t6 ($14) */
    register int count __asm__("$13") = ((unsigned short *)p)[1];
    int x0, x1, x2, y0, y1, y2;
    int *ret;

    p = (Tri20 *)((char *)p + 4);
    do {
        x0 = p->v01;
        x2 = p->v2;
        x1 = x0 >> 16;
        x0 &= 0xFFFF;
        RR_GTE_RTPT_LDV(
            verts + (x0 << 3),
            verts + (x1 << 3),
            verts + (x2 << 3)
        );
        x0 = p->c0;
        x1 = p->c1;
        x2 = p->c2;
        a2[1] = x0;
        a2[3] = x1;
        a2[5] = x2;
        /* ASM_BARRIER: GTE SXY0/SXY1/SXY2 stores and reads */
        __asm__ volatile(
            "swc2 $12, 8(%3)\n\t"
            "swc2 $13, 16(%3)\n\t"
            "swc2 $14, 24(%3)\n\t"
            "mfc2 %0, $12\n\t"
            "mfc2 %1, $13\n\t"
            "mfc2 %2, $14"
            : "=r"(x0), "=r"(x1), "=r"(x2)
            : "r"(a2)
            : "memory"
        );
        y0 = x0 >> 16;
        y1 = x1 >> 16;
        y2 = x2 >> 16;
        if ((y0 >= 0 || y1 >= 0 || y2 >= 0) &&
            (y0 < 0x100 || y1 < 0x100 || y2 < 0x100)) {
            x0 <<= 16;
            x1 <<= 16;
            x2 <<= 16;
            x0 >>= 16;
            x1 >>= 16;
            x2 >>= 16;
            if ((x0 >= 0 || x1 >= 0 || x2 >= 0) &&
                (x0 < 0x140 || x1 < 0x140 || x2 < 0x140)) {
                a2[0] = *a1;
                ((char *)a2)[3] = 6;
                *a1 = (int)a2;
                a2 += 7;
                ((char *)a1)[3] = 0;
            }
        }
        p++;
        count--;
    } while (count > 0);
    a2[1] = 0xE1000220;
    a2[0] = *a1;
    ((char *)a2)[3] = 1;
    ret = a2 + 2;
    *a1 = (int)a2;
    ((char *)a1)[3] = 0;
    return ret;
}
