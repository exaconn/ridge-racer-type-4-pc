#include "rr_asm.h"

typedef struct {
    unsigned short v0;
    unsigned short v1;
    unsigned short v2;
    unsigned short v3;
    int pad08;
    int uv0;
    int uv1;
    int uv23;
    signed char b18;
    signed char b19;
    short pad1A;
} Poly1A8DC;

typedef struct {
    int pad00[7];
    int otz;
    int mac0;
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
} Ctx1A8DC;

extern int D_8009E36C[];
extern int D_800ABEB0;

void func_8001A8DC(int a0) {
    Ctx1A8DC *a3 = (Ctx1A8DC *)0x1F80007C;
    /* ASM_BARRIER: pin scratchpad base pointer to $a1 ($5) to match original pre-loop CSE */
    register int *g asm("$5");
    int *v1;
    int *pkt;
    Poly1A8DC *t1;
    char *verts;
    /* ASM_BARRIER: pin count to $t3 ($11) to match original caller-saved loop allocation */
    register int count asm("$11");
    int t5;
    int t6;
    int t7;
    int v0, v1_tmp, a1, a2, off, g64;

    RR_GTE_SETROT((int *)0x1F800028);
    RR_GTE_SETTR((int *)0x1F80003C);

    off = a0 * 12;
    g = (int *)0x1F800000;
    /* ASM_BARRIER: keep 0x1F800000 base pointer in $a1 and finish a0*12 before v1 load so v1 gets $a0 */
    __asm__("" : "+r"(g) : "r"(off));
    v1 = (int *)((char *)g[0x48 / 4] - 4);
    pkt = (int *)g[0];
    t7 = g[0x60 / 4] + 2;
    v1 = (int *)((char *)v1 + off);
    t1 = (Poly1A8DC *)v1[3];
    t5 = g[1] + 0x200;
    verts = (char *)v1[1];
    count = ((unsigned short *)t1)[1];
    t1 = (Poly1A8DC *)((char *)t1 + 4);
    g64 = g[0x64 / 4];
    /* ASM_BARRIER: materialize count in $t3 after g[0x64/4] load and order global-alloc priorities t1 ($t1) > pkt ($t2) > verts ($t4) > t5 ($t5) */
    __asm__("" : "+r"(count) : "r"(t1), "r"(t1), "r"(t1), "r"(t5), "r"(g64));
    t6 = (g64 != 0) << 31;
    while (count-- > 0) {
        RR_GTE_RTPT_LDV(
            verts + (t1->v0 << 3),
            verts + (t1->v1 << 3),
            verts + (t1->v2 << 3)
        );
        a2 = t1->uv0;
        a1 = t1->uv1;
        pkt[3] = a2;
        pkt[5] = a1;
        a2 = t1->uv23;
        a1 = a2 >> 16;
        pkt[7] = a2;
        pkt[9] = a1;
        RR_GTE14_NCLIP();
        RR_GTE_SWC2_SXYZ(&a3->u.w.sxy0);
        RR_GTE14_SWC2_MAC0(&a3->mac0);
        if ((a3->mac0 ^ t6) >= 0) {
            RR_GTE_RTPS_LDV(verts + (t1->v3 << 3));
            a2 = D_8009E36C[(D_800ABEB0 + t1->b18) & 0x3F];
            a1 = a3->u.w.sxy0;
            v0 = a3->u.w.sxy1;
            v1_tmp = a3->u.w.sxy2;
            pkt[2] = a1;
            pkt[4] = v0;
            pkt[6] = v1_tmp;
            pkt[1] = a2;
            RR_GTE14_NOP2();
            RR_GTE14_AVSZ4();
            RR_GTE_SWC2_SXY2(&a3->u.w.sxy3);
            RR_GTE14_SWC2_OTZ(&a3->otz);
            pkt[8] = a3->u.w.sxy3;
            {
                /* ASM_BARRIER: keep shifted otz in $v0 ($2) so sw $v0, 0x1C($a3) stays before slti $v0, $v0, 0x1C0 */
                register int otz asm("$2") = a3->otz;
                if (otz > 0) {
                    otz >>= t7;
                    a3->otz = otz;
                    if (otz < 0x1C0) {
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
                                /* ASM_BARRIER: give pkt+3 GIV higher priority than t1+0x19 GIV ($a2 vs $t0) and t6 higher priority than t7 ($t6 vs $t7) */
                                __asm__("" : : "m"(((char *)pkt)[1]), "m"(((char *)pkt)[2]), "r"(t1), "r"(pkt), "r"(pkt), "r"(t6));
                                pkt[0] = *(int *)((t1->b19 << 2) + ((a3->otz << 2) + t5));
                                ((char *)pkt)[3] = 9;
                                *(int *)((t1->b19 << 2) + ((a3->otz << 2) + t5)) = (int)pkt;
                                pkt += 10;
                                *(char *)((t1->b19 << 2) + ((a3->otz << 2) + t5) + 3) = 0;
                            }
                        }
                    }
                }
            }
        }
        t1++;
    }
    *(int **)0x1F800000 = pkt;
}
