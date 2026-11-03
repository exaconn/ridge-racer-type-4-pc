#include "rr_asm.h"

typedef struct {
    int v01;
    short v2;
    char b6;
    signed char b7;
    int c0;
    int c1;
    int c2;
} SkyTri;

typedef struct {
    int ir0;
    int otz;
    int pad08;
    char *verts;
    int pad10;
    int pad14;
    char *v0;
    char *v1;
    char *v2;
    int pad24;
    union {
        struct {
            int sxy0;
            int sxy1;
            int sxy2;
        } w;
        struct {
            short sx0, sy0;
            short sx1, sy1;
            short sx2, sy2;
        } s;
    } u;
} SkyCtx;

typedef struct {
    int *pkt;
    int ot;
} Scratch17310;

SkyTri *func_80017310(SkyTri *a0, SkyCtx *a1, int a2) {
    int v0 = a0->v01;
    int a3 = a0->v2;
    char *verts = a1->verts;
    int v1 = v0 >> 16;
    int *pkt;
    int *g;

    v0 &= 0xFFFF;
    a1->v0 = verts + (v0 << 3);
    a1->v1 = verts + (v1 << 3);
    a1->v2 = verts + (a3 << 3);
    pkt = ((Scratch17310 *)0x1F800000)->pkt;
    do {
        /* ASM_BARRIER: load V0..V2 from a1->v0..v2 into GTE $0..$5 */
        __asm__ volatile(
            "lw $13, 0x18(%0)\n\t"
            "lw $14, 0x1C(%0)\n\t"
            "lw $15, 0x20(%0)\n\t"
            "lwc2 $0, 0($13)\n\t"
            "lwc2 $1, 4($13)\n\t"
            "lwc2 $2, 0($14)\n\t"
            "lwc2 $3, 4($14)\n\t"
            "lwc2 $4, 0($15)\n\t"
            "lwc2 $5, 4($15)"
            :
            : "r"(a1)
            : "$13", "$14", "$15"
        );
        v0 = a0[1].v01;
        /* ASM_BARRIER: GTE RTPT after 1-cycle load delay */
        __asm__ volatile("nop\n\t.word 0x4A280030");
        a3 = a0[1].v2;
        v1 = v0 >> 16;
        v0 &= 0xFFFF;
        a1->v0 = verts + (v0 << 3);
        a1->v1 = verts + (v1 << 3);
        a1->v2 = verts + (a3 << 3);
        RR_GTE_OP(0x4B58002D);
        RR_GTE14_SWC2_OTZ(&a1->otz);
        if (a1->otz > 0) {
            RR_GTE_SWC2_SXYZ(&a1->u.w.sxy0);
            v0 = a1->u.w.sxy0;
            v1 = a1->u.w.sxy1;
            a3 = a1->u.w.sxy2;
            pkt[2] = v0;
            pkt[4] = v1;
            pkt[6] = a3;
            v0 = a1->u.s.sx0;
            v1 = a1->u.s.sx1;
            a3 = a1->u.s.sx2;
            if ((v0 >= 0 || v1 >= 0 || a3 >= 0) &&
                (v0 < 0x140 || v1 < 0x140 || a3 < 0x140)) {
                v0 = a1->u.s.sy0;
                v1 = a1->u.s.sy1;
                a3 = a1->u.s.sy2;
                if ((v0 >= 0 || v1 >= 0 || a3 >= 0) &&
                    (v0 < 0x100 || v1 < 0x100 || a3 < 0x100)) {
                    v0 = a0->c0;
                    v1 = a0->c1;
                    a3 = a0->c2;
                    /* ASM_BARRIER: load IR0, RGB0..RGB2, run DPCT, store RGB0..RGB2 */
                    __asm__ volatile(
                        "lwc2 $8, 0(%0)\n\t"
                        "mtc2 %1, $6\n\t"
                        "mtc2 %1, $20\n\t"
                        "mtc2 %2, $21\n\t"
                        "mtc2 %3, $22\n\t"
                        "nop\n\t"
                        "nop\n\t"
                        ".word 0x4AF8002A\n\t"
                        "swc2 $20, 4(%4)\n\t"
                        "swc2 $21, 12(%4)\n\t"
                        "swc2 $22, 20(%4)"
                        :
                        : "r"(a1), "r"(v0), "r"(v1), "r"(a3), "r"(pkt)
                        : "memory"
                    );
                    g = (int *)0x1F800000;
                    pkt[0] = *(int *)((a0->b7 << 2) + g[1] + 0xAF8);
                    ((char *)pkt)[3] = 6;
                    *(int *)((a0->b7 << 2) + g[1] + 0xAF8) = (int)pkt;
                    pkt += 7;
                    *(char *)((a0->b7 << 2) + g[1] + 0xAFB) = 0;
                }
            }
        }
        a0++;
        a2--;
    } while (a2 > 0);
    *(int **)0x1F800000 = pkt;
    return a0;
}
