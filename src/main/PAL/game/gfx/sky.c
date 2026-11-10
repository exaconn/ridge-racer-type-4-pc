/* RODATA 0x800100e4 0x20 */
#include "scratchpad.h"
#include "rr_asm.h"
#include "psyq/libgpu/gpudisp.h"
#include "game/data/ldscn5.h"
#include "game/gfx/sky.h"
#include "rr/audio_cd_controller.h"
#include "rr/menu_overlay_runtime.h"

#define GS ((SPD *)SCRATCHPAD)
#define LNK(n, t) \
    GS->cur[0] = GS->ot[(t) / 4].word; \
    ((char *)GS->cur)[3] = (n); \
    GS->ot[(t) / 4].word = GS->cur; \
    GS->ot[(t) / 4].b.len = 0

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

typedef struct {
    int v01;
    int v23;
    int c0;
    int c1;
    int c2;
    int c3;
    char b18;
    signed char b19;
    short pad1A;
} SkyQuad17528;

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
} Scratch17528;

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
    int *pkt;
    int ot;
} Scratch177E4;

typedef struct {
    int v01;
    short v2;
    short pad06;
    int c0;
    int c1;
    int c2;
    int uv0;
    int uv1;
    union {
        int uv2;
        struct {
            char unk0[3];
            signed char b1F;
        } b;
    } u1C;
} SkyTri17A90;

typedef struct {
    int *pkt;
    int ot;
} Scratch17A90;

typedef struct {
    int *cur;
    union {
        int word;
        struct { char b0, b1, b2, len; } b;
    } *ot;
} SPD;

typedef struct {
    unsigned short v0, v1, v2;
    unsigned short n0, n1, n2;
    int unk0C;
    unsigned char b10;
    signed char b11;
    short unk12;
} SkyLitTri14;

typedef struct {
    unsigned short v0, v1, v2, v3;
    unsigned short n0, n1, n2, n3;
    int rgb10;
    unsigned char b14;
    signed char b15;
    short unk16;
} SkyLitQuad18;

typedef struct {
    unsigned short v0, v1, v2, v3;
    unsigned short n0, n1, n2, n3;
    int rgb10;
    int uv0;
    int uv1;
    union {
        int word;
        struct { short lo, hi; } h;
    } uv2;
    unsigned char b20;
    signed char b21;
    short unk22;
} SkyLitTexQuad24;

typedef struct {
    int unk00[28];
    char *verts70;
    int unk74[28];
    int countE4;
    int otzE8;
    int mac0EC;
    int mac0F0;
    int rgbF4;
    int pageF8;
    int clutFC;
    struct { int x, y, z; } ir100;
    struct { int x, y, z; } ir10C;
    struct { int x, y, z; } ir118;
    struct { int x, y, z; } ir124;
    int unk130[4];
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
    } sxy140;
} SkyLitCtx;

typedef struct {
    int unk00;
    int verts04;
    int unk08;
    int *cmds0C;
} SkyCmdHeader;

extern char D_800100D0[];
extern short *CourseSkyKeyframePtrs[];
extern short *CourseSceneryRangePtrs[];
extern char D_8009E248[];
extern int SetGeomOffset();
extern int SetDrawArea();

SkyTri *SkyEmitTriFT3(SkyTri *a0, SkyCtx *a1, int a2);
SkyQuad17528 *SkyEmitQuadG4(SkyQuad17528 *a0, SkyCtxQuad *a1, int a2);
SkyQuad177E4 *SkyEmitQuadFT4(SkyQuad177E4 *a0, SkyCtxQuad *a1, int a2);
SkyTri17A90 *EmitSkyDepthCuedGT3(SkyTri17A90 *a0, SkyCtx *a1, int a2);
void DrawTrackScenery();
SkyLitTri14 *SkyEmitLitTriG3(SkyLitTri14 *a0, SkyLitCtx *a1);
SkyLitQuad18 *SkyEmitLitTexQuadGT4(SkyLitQuad18 *a0, SkyLitCtx *a1);
SkyLitQuad18 *SkyEmitLitQuadG4(SkyLitQuad18 *a0, SkyLitCtx *a1);
SkyLitTexQuad24 *EmitSkyLitGT4(SkyLitTexQuad24 *a0, SkyLitCtx *a1);
SkyLitTexQuad24 *SkyEmitSpecTexQuadGT4(SkyLitTexQuad24 *a0, SkyLitCtx *a1);
void RunSkyCommandList(char *a0, int a1);
void RenderSkyToRect(int a0, int a1, short *a2, int a3);

SkyTri *SkyEmitTriFT3(SkyTri *a0, SkyCtx *a1, int a2) {
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
    pkt = ((Scratch17310 *)SCRATCHPAD)->pkt;
    do {
        RR_GTE_LOAD_SKY_TRI_VERTS(a1);
        v0 = a0[1].v01;
        RR_GTE_CMD_1NOP(0x4A280030);
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
                    RR_GTE_DPCT_SKY_TRI(a1, v0, v1, a3, pkt);
                    g = (int *)SCRATCHPAD;
                    pkt[0] = ((int *)((a0->b7 << 2) + g[1]))[0xAF8 / 4];
                    ((char *)pkt)[3] = 6;
                    ((int *)((a0->b7 << 2) + g[1]))[0xAF8 / 4] = pkt;
                    pkt += 7;
                    ((char *)((a0->b7 << 2) + g[1]))[0xAFB] = 0;
                }
            }
        }
        a0++;
        a2--;
    } while (a2 > 0);
    *(int **)SCRATCHPAD = pkt;
    return a0;
}

SkyQuad17528 *SkyEmitQuadG4(SkyQuad17528 *a0, SkyCtxQuad *a1, int a2) {
    int v1 = a0->v01;
    int a3 = a0->v23;
    char *verts = a1->verts;
    int t0 = v1 >> 16;
    int t1 = a3 >> 16;
    int *pkt;
    int *g;
    int *p;

    v1 &= 0xFFFF;
    a3 &= 0xFFFF;
    a1->v0 = verts + (v1 << 3);
    a1->v1 = verts + (t0 << 3);
    a1->v2 = verts + (a3 << 3);
    a1->v3 = (int *)(verts + (t1 << 3));
    pkt = ((Scratch17528 *)SCRATCHPAD)->pkt;
    do {
        RR_GTE_RTPT_SKY_QUAD_VERTS(a1);
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
        RR_GTE14_SWC2_OTZ(&a1->otz);
        if (a1->otz > 0) {
            RR_GTE_SWC2_SXYZ(&a1->u.w.sxy0);
            RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, &a1->v3_0);
            v1 = a1->u.w.sxy0;
            t0 = a1->u.w.sxy1;
            RR_GTE14_RTPS();
            a3 = a1->u.w.sxy2;
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
                    v1 = a0->c0;
                    t0 = a0->c1;
                    a3 = a0->c2;
                    t1 = a0->c3;
                    RR_GTE_DPCT_DPCS_SKY_QUAD(a1, v1, t0, a3, t1, pkt);
                    RR_GTE14_SWC2_RGB2(pkt + 7);
                    g = (int *)SCRATCHPAD;
                    pkt[0] = ((int *)((a0->b19 << 2) + g[1]))[0xAF8 / 4];
                    ((char *)pkt)[3] = 8;
                    ((int *)((a0->b19 << 2) + g[1]))[0xAF8 / 4] = pkt;
                    pkt += 9;
                    ((char *)((a0->b19 << 2) + g[1]))[0xAFB] = 0;
                }
            }
        }
        a0++;
        a2--;
    } while (a2 > 0);
    *(int **)SCRATCHPAD = pkt;
    return a0;
}

SkyQuad177E4 *SkyEmitQuadFT4(SkyQuad177E4 *a0, SkyCtxQuad *a1, int a2) {
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
    pkt = ((Scratch177E4 *)SCRATCHPAD)->pkt;
    do {
        RR_GTE_RTPT_SKY_QUAD_VERTS(a1);
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
            RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, &a1->v3_0);
            a3 = a0->uv23;
            RR_GTE_CMD_1NOP(0x4A180001);
            v0 = a3 >> 16;
            /* ASM_BARRIER: keep sra $v0, $a3, 16 right after rtps and before pkt stores */
            __asm__ volatile("" : "+r"(v0));
            pkt[3] = v1;
            pkt[5] = t0;
            pkt[9] = v0;
            pkt[7] = a3;
            v1 = a1->u.w.sxy0;
            t0 = a1->u.w.sxy1;
            a3 = a1->u.w.sxy2;
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
                    RR_GTE_DPCS_SKY_FT4(a1, t1);
                    RR_GTE14_SWC2_RGB2(pkt + 1);
                    g = (int *)SCRATCHPAD;
                    pkt[0] = ((int *)((a0->b19 << 2) + g[1]))[0xAF8 / 4];
                    ((char *)pkt)[3] = 9;
                    ((int *)((a0->b19 << 2) + g[1]))[0xAF8 / 4] = pkt;
                    pkt += 10;
                    ((char *)((a0->b19 << 2) + g[1]))[0xAFB] = 0;
                }
            }
        }
        a0++;
        a2--;
    } while (a2 > 0);
    *(int **)SCRATCHPAD = pkt;
    return a0;
}

SkyTri17A90 *EmitSkyDepthCuedGT3(SkyTri17A90 *a0, SkyCtx *a1, int a2) {
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
    pkt = ((Scratch17A90 *)SCRATCHPAD)->pkt;
    do {
        RR_GTE_LOAD_SKY_TRI_VERTS(a1);
        v0 = a0[1].v01;
        RR_GTE_CMD_1NOP(0x4A280030);
        a3 = a0[1].v2;
        v1 = v0 >> 16;
        v0 &= 0xFFFF;
        a1->v0 = verts + (v0 << 3);
        a1->v1 = verts + (v1 << 3);
        a1->v2 = verts + (a3 << 3);
        v0 = a0->uv0;
        v1 = a0->uv1;
        a3 = a0->u1C.uv2;
        pkt[3] = v0;
        pkt[6] = v1;
        pkt[9] = a3;
        RR_GTE_OP(0x4B58002D);
        RR_GTE14_SWC2_OTZ(&a1->otz);
        if (a1->otz > 0) {
            RR_GTE_SWC2_SXYZ(&a1->u.w.sxy0);
            v0 = a1->u.w.sxy0;
            v1 = a1->u.w.sxy1;
            a3 = a1->u.w.sxy2;
            pkt[2] = v0;
            pkt[5] = v1;
            pkt[8] = a3;
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
                    RR_GTE_DPCT_SKY_GT3(a1, v0, v1, a3, pkt);
                    g = (int *)SCRATCHPAD;
                    pkt[0] = ((int *)((a0->u1C.b.b1F << 2) + g[1]))[0xAF8 / 4];
                    ((char *)pkt)[3] = 9;
                    ((int *)((a0->u1C.b.b1F << 2) + g[1]))[0xAF8 / 4] = pkt;
                    pkt += 10;
                    ((char *)((a0->u1C.b.b1F << 2) + g[1]))[0xAFB] = 0;
                }
            }
        }
        a0++;
        a2--;
    } while (a2 > 0);
    *(int **)SCRATCHPAD = pkt;
    return a0;
}

void DrawTrackScenery(int a0) {
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
    s0 = (int *)(SCRATCHPAD + 0x7C);
    s4 = TrackSkyTable;
    RR_GTE_SETROT((int *)SCRATCH_ROT_MATRIX);
    RR_GTE_CLRTR();
    a1 = CourseSkyKeyframePtrs[CourseIndex];
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
    a1 = CourseSceneryRangePtrs[CourseIndex];
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
                case 7: r = SkyEmitTriFT3(r, s0, w); break;
                case 8: r = SkyEmitQuadG4(r, s0, w); break;
                case 1: r = SkyEmitQuadFT4(r, s0, w); break;
                case 11: r = EmitSkyDepthCuedGT3(r, s0, w); break;
                default: DebugPrintStub(D_800100D0, k, w); return;
                }
            }
            s1++;
            s2 += 12;
        } while (s1 < s3);
    }
}

SkyLitTri14 *SkyEmitLitTriG3(SkyLitTri14 *a0, SkyLitCtx *a1) {
    int *hw;
    int n;
    int *pkt;
    int shift;
    char *verts;

    a1->rgbF4 = 0x34808080;
    RR_GTE14_LWC2_RGB(&a1->rgbF4);

    hw = (int *)SCRATCHPAD;
    n = a1->countE4;
    pkt = (int *)hw[0];
    shift = hw[0x60 / 4];
    verts = a1->verts70;

    while (n-- > 0) {
        RR_GTE_RTPT_LDV(
            verts + (a0->v0 << 3),
            verts + (a0->v1 << 3),
            verts + (a0->v2 << 3)
        );
        RR_GTE14_NOP2();
        RR_GTE14_NCLIP();
        RR_GTE14_SWC2_MAC0(&a1->mac0EC);

        if (a1->mac0EC > 0) {
            RR_GTE14_SWC2_SXYZ(&a1->sxy140.w.sxy0);
            pkt[2] = a1->sxy140.w.sxy0;
            pkt[5] = a1->sxy140.w.sxy1;
            pkt[8] = a1->sxy140.w.sxy2;
            RR_GTE14_NOP2();
            RR_GTE_OP(0x4B58002D);

            if (a1->sxy140.s.sx0 >= 0 || a1->sxy140.s.sx1 >= 0 || a1->sxy140.s.sx2 >= 0) {
                if (a1->sxy140.s.sx0 < 0x140 || a1->sxy140.s.sx1 < 0x140 || a1->sxy140.s.sx2 < 0x140) {
                    if (a1->sxy140.s.sy0 >= 0 || a1->sxy140.s.sy1 >= 0 || a1->sxy140.s.sy2 >= 0) {
                        if (a1->sxy140.s.sy0 < 0x200 || a1->sxy140.s.sy1 < 0x200 || a1->sxy140.s.sy2 < 0x200) {
                            int u, v, z;

                            RR_GTE14_SWC2_OTZ(&a1->otzE8);
                            RR_GTE14_LWC2_V0_V1_V2(
                                a1->verts70 + (a0->n0 << 3),
                                a1->verts70 + (a0->n1 << 3),
                                a1->verts70 + (a0->n2 << 3)
                            );
                            RR_GTE14_NOP2();
                            RR_GTE14_LIGHT0();
                            RR_GTE14_LIGHT1();
                            RR_GTE14_LIGHT2();
                            RR_GTE_SWC2_RGB012_ENV(pkt);
                            RR_GTE14_NOP2();
                            RR_GTE_OP(0x4A486012);
                            RR_GTE_STORE_IR_MEM(&a1->ir100);
                            RR_GTE14_NOP2();
                            RR_GTE_OP(0x4A48E012);

                            u = (a1->ir100.x >> 6) + 0x40;
                            v = (a1->ir100.y >> 7) + 0x40;
                            if (u < 0) {
                                u = 0;
                            } else if (u >= 0x80) {
                                u = 0x7F;
                            }
                            if (v < 0) {
                                v = 0;
                            } else if (v >= 0x80) {
                                v = 0x7F;
                            }
                            pkt[3] = (v << 8) | u | a1->clutFC;

                            RR_GTE_STORE_IR_MEM(&a1->ir10C);
                            RR_GTE14_NOP2();
                            RR_GTE_OP(0x4A496012);

                            u = (a1->ir10C.x >> 6) + 0x40;
                            v = (a1->ir10C.y >> 7) + 0x40;
                            if (u < 0) {
                                u = 0;
                            } else if (u >= 0x80) {
                                u = 0x7F;
                            }
                            if (v < 0) {
                                v = 0;
                            } else if (v >= 0x80) {
                                v = 0x7F;
                            }
                            pkt[6] = (v << 8) | u | a1->pageF8;

                            RR_GTE_STORE_IR_MEM(&a1->ir118);
                            u = (a1->ir118.x >> 6) + 0x40;
                            v = (a1->ir118.y >> 7) + 0x40;
                            if (u < 0) {
                                u = 0;
                            } else if (u >= 0x80) {
                                u = 0x7F;
                            }
                            if (v < 0) {
                                v = 0;
                            } else if (v >= 0x80) {
                                v = 0x7F;
                            }
                            pkt[9] = (v << 8) | u;

                            z = a1->otzE8 >> shift;
                            if (z > 0) {
                                if (z < 0x1C0) {
                                    int *g = (int *)SCRATCHPAD;
                                    pkt[0] = *(int *)((a0->b11 << 2) + ((z << 2) + g[1]) + 0x200);
                                    ((char *)pkt)[3] = 9;
                                    *(int *)((a0->b11 << 2) + ((z << 2) + g[1]) + 0x200) = pkt;
                                    pkt += 10;
                                    *(char *)((a0->b11 << 2) + ((z << 2) + g[1]) + 0x203) = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        a0++;
    }

    /* ASM_BARRIER: extend live ranges of n and a0 past loop for register allocation priority */
    __asm__("" : : "r"(n), "r"(n), "r"(n), "r"(a0), "r"(a0), "r"(a0));
    *(int **)SCRATCHPAD = pkt;
    return a0;
}

SkyLitQuad18 *SkyEmitLitTexQuadGT4(SkyLitQuad18 *a0, SkyLitCtx *a1) {
    int *hw;
    int n;
    int *pkt;
    int shift;
    char *verts;

    a1->rgbF4 = 0x3C808080;
    RR_GTE14_LWC2_RGB(&a1->rgbF4);

    hw = (int *)SCRATCHPAD;
    n = a1->countE4;
    pkt = (int *)hw[0];
    shift = hw[0x60 / 4];
    verts = a1->verts70;

    while (n-- > 0) {
        RR_GTE_RTPT_LDV(
            verts + (a0->v0 << 3),
            verts + (a0->v1 << 3),
            verts + (a0->v2 << 3)
        );
        RR_GTE14_NOP2();
        RR_GTE14_NCLIP();
        RR_GTE14_SWC2_MAC0(&a1->mac0EC);
        RR_GTE14_SWC2_SXYZ(&a1->sxy140.w.sxy0);
        RR_GTE_RTPS_LDV(verts + (a0->v3 << 3));
        pkt[2] = a1->sxy140.w.sxy0;
        pkt[5] = a1->sxy140.w.sxy1;
        pkt[8] = a1->sxy140.w.sxy2;
        RR_GTE14_NOP2();
        RR_GTE14_NCLIP();
        RR_GTE14_SWC2_SXY2(&a1->sxy140.w.sxy3);
        RR_GTE14_SWC2_MAC0(&a1->mac0F0);

        if (a1->mac0EC > 0 || a1->mac0F0 < 0) {
            RR_GTE14_NOP2();
            RR_GTE14_AVSZ4();
            pkt[11] = a1->sxy140.w.sxy3;

            if (a1->sxy140.s.sx0 >= 0 || a1->sxy140.s.sx1 >= 0 || a1->sxy140.s.sx2 >= 0 || a1->sxy140.s.sx3 >= 0) {
                if (a1->sxy140.s.sx0 < 0x140 || a1->sxy140.s.sx1 < 0x140 || a1->sxy140.s.sx2 < 0x140 || a1->sxy140.s.sx3 < 0x140) {
                    if (a1->sxy140.s.sy0 >= 0 || a1->sxy140.s.sy1 >= 0 || a1->sxy140.s.sy2 >= 0 || a1->sxy140.s.sy3 >= 0) {
                        if (a1->sxy140.s.sy0 < 0x200 || a1->sxy140.s.sy1 < 0x200 || a1->sxy140.s.sy2 < 0x200 || a1->sxy140.s.sy3 < 0x200) {
                            int u, v, z;

                            RR_GTE14_SWC2_OTZ(&a1->otzE8);
                            RR_GTE14_LWC2_V0_V1_V2(
                                a1->verts70 + (a0->n0 << 3),
                                a1->verts70 + (a0->n1 << 3),
                                a1->verts70 + (a0->n2 << 3)
                            );
                            RR_GTE14_NOP2();
                            RR_GTE14_LIGHT0();
                            RR_GTE14_LIGHT1();
                            RR_GTE14_LIGHT2();
                            RR_GTE_SWC2_RGB012_ENV(pkt);
                            RR_GTE14_NOP2();
                            RR_GTE_OP(0x4A486012);
                            RR_GTE_STORE_IR_MEM(&a1->ir100);
                            RR_GTE14_NOP2();
                            RR_GTE_OP(0x4A48E012);

                            u = (a1->ir100.x >> 6) + 0x40;
                            v = (a1->ir100.y >> 7) + 0x40;
                            if (u < 0) {
                                u = 0;
                            } else if (u >= 0x80) {
                                u = 0x7F;
                            }
                            if (v < 0) {
                                v = 0;
                            } else if (v >= 0x80) {
                                v = 0x7F;
                            }
                            pkt[3] = (v << 8) | u | a1->clutFC;

                            RR_GTE_STORE_IR_MEM(&a1->ir10C);
                            RR_GTE14_NOP2();
                            RR_GTE_OP(0x4A496012);

                            u = (a1->ir10C.x >> 6) + 0x40;
                            v = (a1->ir10C.y >> 7) + 0x40;
                            if (u < 0) {
                                u = 0;
                            } else if (u >= 0x80) {
                                u = 0x7F;
                            }
                            if (v < 0) {
                                v = 0;
                            } else if (v >= 0x80) {
                                v = 0x7F;
                            }
                            pkt[6] = (v << 8) | u | a1->pageF8;

                            RR_GTE_STORE_IR_MEM(&a1->ir118);
                            RR_GTE14_LWC2_V0(a1->verts70 + (a0->n3 << 3));
                            RR_GTE14_NOP2();
                            RR_GTE14_LIGHT0();
                            RR_GTE14_SWC2_RGB2(pkt + 10);
                            RR_GTE14_NOP2();
                            RR_GTE_OP(0x4A486012);

                            u = (a1->ir118.x >> 6) + 0x40;
                            v = (a1->ir118.y >> 7) + 0x40;
                            if (u < 0) {
                                u = 0;
                            } else if (u >= 0x80) {
                                u = 0x7F;
                            }
                            if (v < 0) {
                                v = 0;
                            } else if (v >= 0x80) {
                                v = 0x7F;
                            }
                            pkt[9] = (v << 8) | u;

                            RR_GTE_STORE_IR_MEM(&a1->ir124);
                            u = (a1->ir124.x >> 6) + 0x40;
                            v = (a1->ir124.y >> 7) + 0x40;
                            if (u < 0) {
                                u = 0;
                            } else if (u >= 0x80) {
                                u = 0x7F;
                            }
                            if (v < 0) {
                                v = 0;
                            } else if (v >= 0x80) {
                                v = 0x7F;
                            }
                            pkt[12] = (v << 8) | u;

                            z = a1->otzE8 >> shift;
                            if (z > 0) {
                                if (z < 0x1C0) {
                                    int *g = (int *)SCRATCHPAD;
                                    pkt[0] = *(int *)((a0->b15 << 2) + ((z << 2) + g[1]) + 0x200);
                                    ((char *)pkt)[3] = 0xC;
                                    *(int *)((a0->b15 << 2) + ((z << 2) + g[1]) + 0x200) = pkt;
                                    pkt += 13;
                                    *(char *)((a0->b15 << 2) + ((z << 2) + g[1]) + 0x203) = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        a0++;
    }

    /* ASM_BARRIER: extend live range of n past loop for register allocation priority */
    __asm__("" : : "r"(n), "r"(n), "r"(n));
    *(int **)SCRATCHPAD = pkt;
    return a0;
}

SkyLitQuad18 *SkyEmitLitQuadG4(SkyLitQuad18 *a0, SkyLitCtx *a1) {
    int *hw = (int *)SCRATCHPAD;
    int n = a1->countE4;
    int *pkt = (int *)hw[0];
    int shift = hw[0x60 / 4];
    char *verts = a1->verts70;

    while (n-- > 0) {
        RR_GTE_RTPT_LDV(
            verts + (a0->v0 << 3),
            verts + (a0->v1 << 3),
            verts + (a0->v2 << 3)
        );
        RR_GTE14_NOP2();
        RR_GTE14_NCLIP();
        RR_GTE14_SWC2_MAC0(&a1->mac0EC);
        RR_GTE14_NOP2();
        RR_GTE_OP(0x4B58002D);
        RR_GTE14_SWC2_OTZ(&a1->otzE8);
        if (a1->otzE8 > 0) {
            RR_GTE14_SWC2_SXYZ(&a1->sxy140.w.sxy0);
            RR_GTE_RTPS_LDV(verts + (a0->v3 << 3));
            RR_GTE14_NOP2();
            RR_GTE14_NCLIP();
            RR_GTE14_SWC2_MAC0(&a1->mac0F0);
            if (a1->mac0EC > 0 || a1->mac0F0 < 0) {
                RR_GTE14_SWC2_SXY2(&a1->sxy140.w.sxy3);
                pkt[2] = a1->sxy140.w.sxy0;
                pkt[4] = a1->sxy140.w.sxy1;
                pkt[6] = a1->sxy140.w.sxy2;
                pkt[8] = a1->sxy140.w.sxy3;
                if ((a1->sxy140.s.sx0 >= 0 || a1->sxy140.s.sx1 >= 0 || a1->sxy140.s.sx2 >= 0 || a1->sxy140.s.sx3 >= 0) &&
                    (a1->sxy140.s.sx0 < 0x140 || a1->sxy140.s.sx1 < 0x140 || a1->sxy140.s.sx2 < 0x140 || a1->sxy140.s.sx3 < 0x140)) {
                    if ((a1->sxy140.s.sy0 >= 0 || a1->sxy140.s.sy1 >= 0 || a1->sxy140.s.sy2 >= 0 || a1->sxy140.s.sy3 >= 0) &&
                        (a1->sxy140.s.sy0 < 0x200 || a1->sxy140.s.sy1 < 0x200 || a1->sxy140.s.sy2 < 0x200 || a1->sxy140.s.sy3 < 0x200)) {
                        int z = a1->otzE8 >> shift;
                        if (z > 0) {
                            if (z < 0x1C0) {
                                int *g = (int *)SCRATCHPAD;
                                RR_GTE14_LWC2_RGB(&a0->rgb10);
                                RR_GTE14_LWC2_V0_V1_V2(
                                    a1->verts70 + (a0->n0 << 3),
                                    a1->verts70 + (a0->n1 << 3),
                                    a1->verts70 + (a0->n2 << 3)
                                );
                                RR_GTE14_NOP2();
                                RR_GTE_OP(0x4B18043F);
                                RR_GTE14_SWC2_RGB012(pkt);
                                RR_GTE14_LWC2_V0(a1->verts70 + (a0->n3 << 3));
                                RR_GTE14_NOP2();
                                RR_GTE_OP(0x4B08041B);
                                RR_GTE14_SWC2_RGB2(pkt + 7);
                                pkt[0] = *(int *)((a0->b15 << 2) + ((z << 2) + g[1]) + 0x200);
                                ((char *)pkt)[3] = 8;
                                *(int *)((a0->b15 << 2) + ((z << 2) + g[1]) + 0x200) = pkt;
                                pkt += 9;
                                *(char *)((a0->b15 << 2) + ((z << 2) + g[1]) + 0x203) = 0;
                            }
                        }
                    }
                }
            }
        }
        a0++;
        /* ASM_BARRIER: boost n ref count in loop so n gets $t3 */
        __asm__("" : "+r"(n));
    }
    /* ASM_BARRIER: boost a0 ref count and extend verts live range past loop so a0 gets $t4 and verts gets $t5 */
    __asm__("" : : "r"(a0), "r"(a0), "r"(a0), "r"(verts));
    *(int **)SCRATCHPAD = pkt;
    return a0;
}

SkyLitTexQuad24 *EmitSkyLitGT4(SkyLitTexQuad24 *a0, SkyLitCtx *a1) {
    int *hw = (int *)SCRATCHPAD;
    int n = a1->countE4;
    int *pkt = (int *)hw[0];
    int shift = hw[0x60 / 4];
    char *verts = a1->verts70;
    int z;

    while (n-- > 0) {
        RR_GTE_RTPT_LDV(
            verts + (a0->v0 << 3),
            verts + (a0->v1 << 3),
            verts + (a0->v2 << 3)
        );
        RR_GTE14_NOP2();
        RR_GTE14_NCLIP();
        RR_GTE14_SWC2_MAC0(&a1->mac0EC);
        RR_GTE14_NOP2();
        RR_GTE_OP(0x4B58002D);
        RR_GTE14_SWC2_OTZ(&a1->otzE8);
        if (a1->otzE8 > 0) {
            RR_GTE14_SWC2_SXYZ(&a1->sxy140.w.sxy0);
            RR_GTE_RTPS_LDV(verts + (a0->v3 << 3));
            RR_GTE14_NOP2();
            RR_GTE14_NCLIP();
            RR_GTE14_SWC2_MAC0(&a1->mac0F0);
            if (a1->mac0EC > 0 || a1->mac0F0 < 0) {
                RR_GTE14_SWC2_SXY2(&a1->sxy140.w.sxy3);
                pkt[2] = a1->sxy140.w.sxy0;
                pkt[5] = a1->sxy140.w.sxy1;
                pkt[8] = a1->sxy140.w.sxy2;
                pkt[11] = a1->sxy140.w.sxy3;
                if ((a1->sxy140.s.sx0 >= 0 || a1->sxy140.s.sx1 >= 0 || a1->sxy140.s.sx2 >= 0 || a1->sxy140.s.sx3 >= 0) &&
                    (a1->sxy140.s.sx0 < 0x140 || a1->sxy140.s.sx1 < 0x140 || a1->sxy140.s.sx2 < 0x140 || a1->sxy140.s.sx3 < 0x140)) {
                    if ((a1->sxy140.s.sy0 >= 0 || a1->sxy140.s.sy1 >= 0 || a1->sxy140.s.sy2 >= 0 || a1->sxy140.s.sy3 >= 0) &&
                        (a1->sxy140.s.sy0 < 0x200 || a1->sxy140.s.sy1 < 0x200 || a1->sxy140.s.sy2 < 0x200 || a1->sxy140.s.sy3 < 0x200)) {
                        pkt[3] = a0->uv0;
                        pkt[6] = a0->uv1;
                        pkt[9] = a0->uv2.word;
                        pkt[12] = a0->uv2.h.hi;
                        z = a1->otzE8 >> shift;
                        if (z > 0) {
                            if (z < 0x1C0) {
                                int *g = (int *)SCRATCHPAD;
                                RR_GTE14_LWC2_RGB(&a0->rgb10);
                                RR_GTE14_LWC2_V0_V1_V2(
                                    a1->verts70 + (a0->n0 << 3),
                                    a1->verts70 + (a0->n1 << 3),
                                    a1->verts70 + (a0->n2 << 3)
                                );
                                RR_GTE14_NOP2();
                                RR_GTE14_LIGHT0();
                                RR_GTE14_LIGHT1();
                                RR_GTE14_LIGHT2();
                                RR_GTE_SWC2_RGB012_ENV(pkt);
                                RR_GTE14_LWC2_V0(a1->verts70 + (a0->n3 << 3));
                                RR_GTE14_NOP2();
                                RR_GTE14_LIGHT0();
                                RR_GTE14_SWC2_RGB2(pkt + 10);
                                pkt[0] = *(int *)((a0->b21 << 2) + ((z << 2) + g[1]) + 0x200);
                                ((char *)pkt)[3] = 0xC;
                                *(int *)((a0->b21 << 2) + ((z << 2) + g[1]) + 0x200) = pkt;
                                pkt += 13;
                                *(char *)((a0->b21 << 2) + ((z << 2) + g[1]) + 0x203) = 0;
                                /* ASM_BARRIER: extend z live range to end of block so pkt+3 gets $t0 and z gets $t1 */
                                __asm__("" : : "r"(z));
                            }
                        }
                    }
                }
            }
        }
        a0++;
        /* ASM_BARRIER: boost n ref count in loop so n gets $t3 */
        __asm__("" : "+r"(n));
    }
    /* ASM_BARRIER: boost a0 ref count and extend verts live range past loop */
    __asm__("" : : "r"(a0), "r"(a0), "r"(a0), "r"(verts));
    *(int **)SCRATCHPAD = pkt;
    return a0;
}

SkyLitTexQuad24 *SkyEmitSpecTexQuadGT4(SkyLitTexQuad24 *a0, SkyLitCtx *a1) {
    int *hw = (int *)SCRATCHPAD;
    int n = a1->countE4;
    int *pkt = (int *)hw[0];
    int shift = hw[0x60 / 4];
    char *verts = a1->verts70;

    while (n-- > 0) {
        RR_GTE_RTPT_LDV(
            verts + (a0->v0 << 3),
            verts + (a0->v1 << 3),
            verts + (a0->v2 << 3)
        );
        RR_GTE14_NOP2();
        RR_GTE14_NCLIP();
        RR_GTE14_SWC2_MAC0(&a1->mac0EC);
        RR_GTE14_NOP2();
        RR_GTE_OP(0x4B58002D);
        RR_GTE14_SWC2_OTZ(&a1->otzE8);
        if (a1->otzE8 > 0) {
            RR_GTE14_SWC2_SXYZ(&a1->sxy140.w.sxy0);
            RR_GTE_RTPS_LDV(verts + (a0->v3 << 3));
            RR_GTE14_NOP2();
            RR_GTE14_NCLIP();
            RR_GTE14_SWC2_MAC0(&a1->mac0F0);
            if (a1->mac0EC > 0 || a1->mac0F0 < 0) {
                RR_GTE14_SWC2_SXY2(&a1->sxy140.w.sxy3);
                pkt[2] = a1->sxy140.w.sxy0;
                pkt[4] = a1->sxy140.w.sxy1;
                pkt[6] = a1->sxy140.w.sxy2;
                pkt[8] = a1->sxy140.w.sxy3;
                if ((a1->sxy140.s.sx0 >= 0 || a1->sxy140.s.sx1 >= 0 || a1->sxy140.s.sx2 >= 0 || a1->sxy140.s.sx3 >= 0) &&
                    (a1->sxy140.s.sx0 < 0x140 || a1->sxy140.s.sx1 < 0x140 || a1->sxy140.s.sx2 < 0x140 || a1->sxy140.s.sx3 < 0x140)) {
                    if ((a1->sxy140.s.sy0 >= 0 || a1->sxy140.s.sy1 >= 0 || a1->sxy140.s.sy2 >= 0 || a1->sxy140.s.sy3 >= 0) &&
                        (a1->sxy140.s.sy0 < 0x200 || a1->sxy140.s.sy1 < 0x200 || a1->sxy140.s.sy2 < 0x200 || a1->sxy140.s.sy3 < 0x200)) {
                        int z;
                        pkt[3] = a0->uv0;
                        pkt[5] = a0->uv1;
                        pkt[7] = a0->uv2.word;
                        pkt[9] = a0->uv2.h.hi;
                        z = a1->otzE8 >> shift;
                        if (z > 0) {
                            if (z < 0x1C0) {
                                int *g = (int *)SCRATCHPAD;
                                RR_GTE14_LWC2_RGB(&a0->rgb10);
                                RR_GTE14_LWC2_V0(a1->verts70 + (a0->n0 << 3));
                                RR_GTE14_NOP2();
                                RR_GTE14_LIGHT0();
                                RR_GTE14_SWC2_RGB2(pkt + 1);
                                pkt[0] = *(int *)((a0->b21 << 2) + ((z << 2) + g[1]) + 0x200);
                                ((char *)pkt)[3] = 9;
                                *(int *)((a0->b21 << 2) + ((z << 2) + g[1]) + 0x200) = pkt;
                                pkt += 10;
                                *(char *)((a0->b21 << 2) + ((z << 2) + g[1]) + 0x203) = 0;
                            }
                        }
                    }
                }
            }
        }
        a0++;
        /* ASM_BARRIER: weight pkt, n, a0 loop references for register allocation order */
        __asm__("" : "+r"(n) : "r"(pkt), "r"(a0));
    }
    *(int **)SCRATCHPAD = pkt;
    return a0;
}

void RunSkyCommandList(char *a0, int a1)
{
    int *s = (int *)(SCRATCHPAD + 0x7C);
    int w, v;
    int *p;
    SkyCmdHeader *h = (SkyCmdHeader *)(a0 + a1 * 12);
    v = h->verts04;
    s[0x3F] = 0x20180000;
    s[0x3E] = 0x60000;
    s[0x1C] = v;
    p = h->cmds0C;
    w = *p++;
    while (w) {
        s[0x39] = w >> 16;
        switch (w & 0xFFFF) {
        case 9:
            p = (int *)SkyEmitLitTriG3((SkyLitTri14 *)p, (SkyLitCtx *)s);
            break;
        case 10:
            p = (int *)SkyEmitLitTexQuadGT4((SkyLitQuad18 *)p, (SkyLitCtx *)s);
            break;
        case 13:
            p = (int *)SkyEmitLitQuadG4((SkyLitQuad18 *)p, (SkyLitCtx *)s);
            break;
        case 15:
            p = (int *)EmitSkyLitGT4((SkyLitTexQuad24 *)p, (SkyLitCtx *)s);
            break;
        case 16:
            p = (int *)SkyEmitSpecTexQuadGT4((SkyLitTexQuad24 *)p, (SkyLitCtx *)s);
            break;
        }
        w = *p++;
    }
}

void SetupSkyLightMatrices(char *a0, unsigned short *a1_in)
{
    volatile unsigned short *p = (unsigned short *)(SCRATCHPAD + 0x7C), *a1 = a1_in;
    RR_GTE_SETROT(D_8009E248);
    RR_GTE_MV3_COL(a1);
    RR_GTE_ST3_COL(p);
    RR_GTE_MV3_COL(a1 + 1);
    RR_GTE_ST3_COL((unsigned short *)(SCRATCHPAD + 0x7E));
    RR_GTE_MV3_COL(a1 + 2);
    RR_GTE_ST3_COL((unsigned short *)SCRATCH_CAR_WORK);
    RR_GTE_SETLLM(p);
    p[0] = a1[0];
    p[1] = a1[1];
    p[2] = a1[2];
    p[3] = a1[3] << 1;
    p[4] = a1[4] << 1;
    p[5] = a1[5] << 1;
    p[6] = a1[6];
    p[7] = a1[7];
    p[8] = a1[8];
    RR_GTE_SETROT(p);
    RR_GTE_SETTR(a0);
    RunSkyCommandList(SkyCommandListPtr, 0);
}

void RenderSkyToRect(int a0, int a1, short *a2, int a3) {
    short sp10[4];
    int c;
    register int *p __asm__("$4"); /* ASM_BARRIER: pin temp reg to match original allocation */
    int k;
    int m;
    int *q, *p1;
    int n;

    GS->cur[2] = 0x65000000;
    GS->cur[3] = (a2[1] << 16) | a2[0];
    GS->cur[5] = a2[2] | 0x01000000;
    GS->cur[4] = 0;
    GS->cur[1] = 0xE1000307;
    LNK(5, 0x10);
    p1 = GS->cur + 6;
    GS->cur = p1;
    if (a2[3] >= 0x101) {
        p1[2] = 0x65000000;
        GS->cur[3] = ((a2[1] + 0x100) << 16) | a2[0];
        GS->cur[5] = ((a2[3] - 0x100) << 16) | a2[2];
        GS->cur[4] = 0;
        GS->cur[1] = 0xE1000317;
        LNK(5, 0x10);
        GS->cur = GS->cur + 6;
    }
    sp10[2] = 0x140;
    sp10[0] = 0;
    sp10[1] = 0;
    sp10[3] = 0x1E0;
    SetDrawArea(GS->cur, sp10);
    LNK(2, 0x10);
    GS->cur = GS->cur + 3;
    SetDrawOffset(GS->cur, sp10);
    LNK(2, 0x10);
    GS->cur = GS->cur + 3;
    SetGeomOffset((short)(unsigned short)a2[2] / 2, (short)(unsigned short)a2[3] / 2);
    SetupSkyLightMatrices(a0, a1);
    c = a3 << 16;
    n = a3 << 8;
    m = 0x66000000;
    n = n | m;
    c = c | n;
    c = c | a3;
    p = GS->cur;
    p[2] = c;
    p[3] = 0xFFFF0000;
    p[5] = a2[2] | 0x01000000;
    p[4] = 0;
    p[1] = 0xE1000307;
    p[0] = GS->ot[4].word;
    ((char *)p)[3] = 5;
    GS->ot[4].word = p;
    GS->ot[4].b.len = 0;
    p = p + 6;
    q = p;
    q[2] = c;
    q[3] = 0xFF0000;
    q[5] = ((a2[3] - 0x100) << 16) | a2[2];
    q[4] = 0;
    k = 0xE1000317;
    q[1] = k;
    q[0] = GS->ot[4].word;
    ((char *)q)[3] = 5;
    GS->ot[4].word = q;
    GS->ot[4].b.len = 0;
    p = p + 6;
    GS->cur = p;
    p[1] = 0x60000000;
    GS->cur[2] = 0;
    GS->cur[3] = (a2[3] << 16) | a2[2];
    GS->cur[0] = GS->ot[700].word;
    ((char *)GS->cur)[3] = 3;
    GS->ot[700].word = GS->cur;
    GS->ot[700].b.len = 0;
    sp10[0] = 0x1C0;
    sp10[1] = 0;
    p = GS->cur + 4;
    GS->cur = p;
    sp10[2] = a2[2];
    sp10[3] = a2[3];
    SetDrawArea(p, sp10);
    LNK(2, 0xAF0);
    GS->cur = GS->cur + 3;
    SetDrawOffset(GS->cur, sp10);
    LNK(2, 0xAF0);
    GS->cur = GS->cur + 3;
}
