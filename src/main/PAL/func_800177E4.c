#include "rr_asm.h"

typedef struct {
    int v01;
    int v23;
    int c0;
    int uv0;
    int uv1;
    int uv23;
    char b18;
    signed char b19;
    short pad1A;
} SkyQuad177E4;

typedef struct {
    int ir0;
    int otz;
    int pad08;
    char *verts;
    int v3_0;
    int v3_1;
    char *v0;
    char *v1;
    char *v2;
    int *v3;
    union {
        struct {
            int sxy0;
            int sxy1;
            int sxy2;
            int sxy3;
        } w;
        struct {
            short sx0, sy0;
            short sx1, sy1;
            short sx2, sy2;
            short sx3, sy3;
        } s;
    } u;
} SkyCtxQuad;

typedef struct {
    int *pkt;
    int ot;
} Scratch177E4;

SkyQuad177E4 *func_800177E4(SkyQuad177E4 *a0, SkyCtxQuad *a1, int a2) {
    int v1 = a0->v01;
    int a3 = a0->v23;
    char *verts = a1->verts;
    int t0 = v1 >> 16;
    int t1 = a3 >> 16;
    int v0;
    int *pkt;
    int *g;
    int *p;

    v1 &= 0xFFFF;
    a3 &= 0xFFFF;
    a1->v0 = verts + (v1 << 3);
    a1->v1 = verts + (t0 << 3);
    a1->v2 = verts + (a3 << 3);
    a1->v3 = (int *)(verts + (t1 << 3));
    pkt = ((Scratch177E4 *)0x1F800000)->pkt;
    do {
        /* ASM_BARRIER: load V0..V2 from a1->v0..v2 into GTE $0..$5 and run RTPT */
        __asm__ volatile(
            "lw $15, 0x18(%0)\n\t"
            "lw $24, 0x1C(%0)\n\t"
            "lw $25, 0x20(%0)\n\t"
            "lwc2 $0, 0($15)\n\t"
            "lwc2 $1, 4($15)\n\t"
            "lwc2 $2, 0($24)\n\t"
            "lwc2 $3, 4($24)\n\t"
            "lwc2 $4, 0($25)\n\t"
            "lwc2 $5, 4($25)\n\t"
            "nop\n\t"
            "nop\n\t"
            ".word 0x4A280030"
            :
            : "r"(a1)
            : "$15", "$24", "$25"
        );
        p = a1->v3;
        v1 = p[0];
        t0 = p[1];
        a1->v3_0 = v1;
        a1->v3_1 = t0;
        v1 = a0[1].v01;
        a3 = a0[1].v23;
        t0 = v1 >> 16;
        t1 = a3 >> 16;
        v1 &= 0xFFFF;
        a3 &= 0xFFFF;
        a1->v0 = verts + (v1 << 3);
        a1->v1 = verts + (t0 << 3);
        a1->v2 = verts + (a3 << 3);
        a1->v3 = (int *)(verts + (t1 << 3));
        RR_GTE_OP(0x4B58002D);
        v1 = a0->uv0;
        t0 = a0->uv1;
        RR_GTE14_SWC2_OTZ(&a1->otz);
        if (a1->otz > 0) {
            RR_GTE_SWC2_SXYZ(&a1->u.w.sxy0);
            /* ASM_BARRIER: load V0 from a1->v3_0..v3_1 */
            __asm__ volatile("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(&a1->v3_0));
            a3 = a0->uv23;
            /* ASM_BARRIER: GTE RTPS after 1-cycle load delay */
            __asm__ volatile("nop\n\t.word 0x4A180001");
            v0 = a3 >> 16;
            /* ASM_BARRIER: keep sra $v0, $a3, 16 right after rtps and before pkt stores */
            __asm__ volatile("" : "+r"(v0));
            pkt[3] = v1;
            pkt[5] = t0;
            pkt[9] = v0;
            pkt[7] = a3;
            v1 = *(int *)((char *)a1 + 0x28);
            t0 = *(int *)((char *)a1 + 0x2C);
            a3 = *(int *)((char *)a1 + 0x30);
            pkt[2] = v1;
            pkt[4] = t0;
            pkt[6] = a3;
            RR_GTE_SWC2_SXY2(&a1->u.w.sxy3);
            RR_GTE_SWC2_SXY2(pkt + 8);
            v1 = a1->u.s.sx0;
            t0 = a1->u.s.sx1;
            a3 = a1->u.s.sx2;
            t1 = a1->u.s.sx3;
            if ((v1 >= 0 || t0 >= 0 || a3 >= 0 || t1 >= 0) &&
                (v1 < 0x140 || t0 < 0x140 || a3 < 0x140 || t1 < 0x140)) {
                v1 = a1->u.s.sy0;
                t0 = a1->u.s.sy1;
                a3 = a1->u.s.sy2;
                t1 = a1->u.s.sy3;
                if ((v1 >= 0 || t0 >= 0 || a3 >= 0 || t1 >= 0) &&
                    (v1 < 0x100 || t0 < 0x100 || a3 < 0x100 || t1 < 0x100)) {
                    t1 = a0->c0;
                    /* ASM_BARRIER: load IR0, RGB0, run DPCS */
                    __asm__ volatile(
                        "lwc2 $8, 0(%0)\n\t"
                        "mtc2 %1, $6\n\t"
                        "nop\n\t"
                        "nop\n\t"
                        ".word 0x4A780010"
                        :
                        : "r"(a1), "r"(t1)
                        : "memory"
                    );
                    RR_GTE14_SWC2_RGB2(pkt + 1);
                    g = (int *)0x1F800000;
                    pkt[0] = *(int *)((a0->b19 << 2) + g[1] + 0xAF8);
                    ((char *)pkt)[3] = 9;
                    *(int *)((a0->b19 << 2) + g[1] + 0xAF8) = (int)pkt;
                    pkt += 10;
                    *(char *)((a0->b19 << 2) + g[1] + 0xAFB) = 0;
                }
            }
        }
        a0++;
        a2--;
    } while (a2 > 0);
    *(int **)0x1F800000 = pkt;
    return a0;
}
