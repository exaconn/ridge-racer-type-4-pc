#include "rr_asm.h"

typedef struct {
    int v01;
    int v23;
    int pad08;
    int uv0;
    int uv1;
    int uv23;
    signed char b18;
    signed char b19;
    unsigned short vz;
    int vxy;
} Poly1A52C;

typedef struct {
    int rot[5];
    int pad14[5];
    int ir3;
    int vxy;
    short vz;
    short pad32;
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
} Ctx1A52C;

extern int D_800FF6A4[];
extern int D_8009E26C[];
extern int D_800ABEB0;

void func_8001A52C(int a0) {
    Ctx1A52C *a3 = (Ctx1A52C *)0x1F80007C;
    /* ASM_BARRIER: pin scratchpad base pointer to $a1 ($5) to match original pre-loop CSE */
    register int *g asm("$5");
    int *v1;
    int *pkt;
    Poly1A52C *t3;
    char *verts;
    /* ASM_BARRIER: pin count to $t8 ($24) to match original caller-saved loop allocation */
    register int count asm("$24");
    int s2;
    int s1;
    int s0;
    int v0, v1_tmp, a1, a2, t2;

    a3->rot[4] = 0x4000;
    a3->rot[2] = 0x4000;
    a3->rot[0] = 0x4000;
    a3->rot[3] = 0;
    a3->rot[1] = 0;
    RR_GTE_SETROT(a3);
    RR_GTE_SETLLM((int *)0x1F800028);
    RR_GTE_SETBK_P(D_800FF6A4);

    g = (int *)0x1F800000;
    /* ASM_BARRIER: keep 0x1F800000 base pointer in $a1 and mark t2 global across basic blocks */
    __asm__("" : "+r"(g), "=r"(t2));
    v1 = (int *)((char *)g[0x48 / 4] - 4);
    pkt = (int *)g[0];
    v1 = (int *)((char *)v1 + a0 * 12);
    t3 = (Poly1A52C *)v1[3];
    verts = (char *)v1[1];
    count = ((unsigned short *)t3)[1];
    t3 = (Poly1A52C *)((char *)t3 + 4);
    v0 = t3->vxy;
    a2 = t3->vz;
    /* ASM_BARRIER: schedule count post-decrement and order global-alloc priorities t3 ($t3) > pkt ($t7) > verts ($t9) */
    __asm__("" : "+r"(count) : "r"(v0), "r"(a2), "r"(t3), "r"(t3), "r"(t3), "r"(t3), "r"(pkt), "r"(pkt));
    a3->vxy = v0;
    a3->vz = a2;
    s2 = g[1] + 0x200;
    s1 = g[0x60 / 4] + 2;
    while (count-- > 0) {
        RR_GTE_MVMVA_LLM_BK_LDV0(&a3->vxy);
        v0 = t3[1].vxy;
        a2 = t3[1].vz;
        a3->vxy = v0;
        a3->vz = a2;
        RR_GTE_SWC2_IR3(&a3->ir3);
        v0 = a3->ir3;
        if (v0 >= 0) {
            s0 = v0 >> s1;
            if (s0 < 0x1C0) {
                RR_GTE_TR_FROM_IR();
                v0 = t3->v01;
                a1 = t3->v23;
                a2 = v0 >> 16;
                t2 = a1 >> 16;
                v0 &= 0xFFFF;
                a1 &= 0xFFFF;
                RR_GTE_RTPT_LDV(
                    verts + (v0 << 3),
                    verts + (a2 << 3),
                    verts + (a1 << 3)
                );
                v0 = t3->uv0;
                a2 = t3->uv1;
                pkt[3] = v0;
                pkt[5] = a2;
                v0 = t3->uv23;
                a2 = v0 >> 16;
                pkt[7] = v0;
                pkt[9] = a2;
                RR_GTE_SWC2_SXYZ(&a3->u.w.sxy0);
                RR_GTE_RTPS_LDV(verts + (t2 << 3));
                a2 = a3->u.w.sxy0;
                a1 = a3->u.w.sxy1;
                t2 = a3->u.w.sxy2;
                v0 = D_8009E26C[(D_800ABEB0 - (v1_tmp = t3->b18 - 0x40)) & 0x3F];
                pkt[2] = a2;
                pkt[4] = a1;
                pkt[6] = t2;
                pkt[1] = v0;
                RR_GTE_SWC2_SXY2(&a3->u.w.sxy3);
                pkt[8] = a3->u.w.sxy3;
                {
                    int x0 = a3->u.s.sx0;
                    int x1 = a3->u.s.sx1;
                    int x2 = a3->u.s.sx2;
                    int x3 = a3->u.s.sx3;
                    if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                        (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                        int y0 = a3->u.s.sy0;
                        int y1 = a3->u.s.sy1;
                        int y2 = a3->u.s.sy2;
                        int y3 = a3->u.s.sy3;
                        if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                            (y0 < 0x100 || y1 < 0x100 || y2 < 0x100 || y3 < 0x100)) {
                            pkt[0] = *(int *)((t3->b19 << 2) + ((s0 << 2) + s2));
                            ((char *)pkt)[3] = 9;
                            *(int *)((t3->b19 << 2) + ((s0 << 2) + s2)) = (int)pkt;
                            pkt += 10;
                            *(char *)((t3->b19 << 2) + ((s0 << 2) + s2) + 3) = 0;
                        }
                    }
                }
            }
        }
        t3++;
    }
    *(int **)0x1F800000 = pkt;
}
