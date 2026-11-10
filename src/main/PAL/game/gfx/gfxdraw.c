#include "psyq/libgte/gtemat4.h"
#include "rr/frame_storage_types.h"
#include "psyq/libgte/gtemat3.h"
#include "psyq/libgte/gtemat.h"
/* RODATA 0x80010f00 0x1c */
#include "types.h"
#include "scratchpad.h"
#include "rr_asm.h"
#include "game/course/trkintp2.h"
#include "game/gfx/dispenv.h"
#include "psyq/libgte/gtemat7.h"
#include "game/gfx/gfxdraw.h"
#include "rr/audio_loop.h"
#include "rr/overlay662_distance.h"
#include "rr/fade_sprite.h"
#include "rr/trig.h"

typedef struct { int a; int b; } P;

typedef struct {
    s32 unk00[7];
    int f1C;
    int f20, f24, f28, f2C;
    short f30, f32, f34, f36;
    s32 unk38[4];
    int f48, f4C;
    P e[2];
    s32 unk60[0x13C];
    CamNode44 *f550;
} Context650;

typedef struct { int x, y, z, w; } V4;

typedef struct { unsigned char a, b, c, d; } C4;

typedef struct { unsigned short a, b, c, d; } S4;

typedef struct {
    s32 unk00[4];
    s16 rotX10, rotY12, rotZ14, unk16;
    C4 c[3];
    S4 v[3];
    unsigned char b3C, b3D, b3E;
} A0;

typedef struct { s32 unk00[0x57C / 4]; } B;

typedef struct { s16 x, y, z, pad; } ShortVec4;

typedef struct {
    s32 unk00[2];
    u16 sxy[6];
    s32 unk14[4];
    ShortVec4 v[3];
    s32 unk3C[2];
    u8 c0[3];
    u8 pad3;
    u8 c1[3];
} GfxGteScratch;

typedef struct {
    u8 link[3], len;
    u8 r0, g0, b0, code;
    u16 x0, y0;
    u8 r1, g1, b1, pad0F;
    u16 x1, y1;
    u8 r2, g2, b2, pad17;
    u16 x2, y2;
} PolyG3Packet;

extern Context650 RaceContext;
extern int D_800AC698;
extern int ViewUsesCarTrackNode;
extern int ResetCarSlotBuffers();
extern void UpdateRaceChaseCamera(void *);
extern void BuildCameraMatrices(void);
extern int Atan2Table(int, int);
extern void SetGeomScreen(int);
extern int *TrackNodes;
extern int D_800AC548[];
extern int BuildRotMatrixZ();
extern int BuildRotMatrixY();
extern int BuildRotMatrixX();
extern int ScaleMatrix();
extern V4 D_80010F1C;
extern int RotTransPers();
extern B D_800D0DD8[];
extern B D_800AE660[];

void UpdateRaceCamera(int arg0, void *arg1);
void InitTrackStartPos(void);

void UpdateRaceCamera(int arg0, void *arg1) {
    int sp10[4];
    int sp20[4];
    unsigned short sp30[4];
    unsigned short sp38[4];
    CamNode44 *s2;
    int a0;
    int a1;
    int a2;
    int t1;
    int *scratch;

    s2 = RaceContext.f550;
    a0 = FindNearestTrackEntry(arg1);
    scratch = (int *)SCRATCHPAD;
    if (arg0 >= 2) {
        s2 = RaceContext.f550;
        s2 += a0;
        ViewUsesCarTrackNode = 0;
        arg0 = s2->f30 & 0xF;
    }
    RaceContext.f1C = 0;
    a1 = RaceContext.f48 + 1;
    RaceContext.f48 = a1;
    if (a0 != RaceContext.f4C) {
        if (GameModeType == 5 || a1 >= 0x1A) {
            RaceContext.f1C = 1;
            RaceContext.f4C = a0;
            RaceContext.f48 = 0;
        }
        ResetCarSlotBuffers(a0, a1);
    }
    switch (arg0) {
    case 0:
        UpdateRaceChaseCamera(arg1);
        break;
    case 1:
        scratch[2] = s2->x * 4;
        scratch[3] = s2->y * 4;
        scratch[4] = s2->z * 4;
        RaceContext.f30 = 0;
        RaceContext.f32 = 0;
        RaceContext.f34 = -0x100;
        CarLocalToWorld(arg1, &RaceContext.f30, &RaceContext.f20);
        sp10[0] = RaceContext.f20 - scratch[2];
        sp10[2] = RaceContext.f28 - scratch[4];
        sp10[1] = RaceContext.f24 - scratch[3];
        scratch[7] = 0x400 - Atan2Table(sp10[0], sp10[2]);
        scratch[6] = 0x400 - Atan2Table(sp10[1], CordicSqrt(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = LerpAngleWrap4096(s2, RaceContext.f48);
        SetGeomScreen(s2->f28);
        break;
    case 2:
        RaceContext.f30 = 0;
        RaceContext.f32 = 0;
        RaceContext.f34 = -0x100;
        CarLocalToWorld(arg1, &RaceContext.f30, &RaceContext.f20);
        a2 = ComputeCameraNodeDist(arg1, s2) >> 4;
        t1 = a2 / 4;
        if (a2 - t1 > (0x400000 / s2->f28) * 8) {
            t1 = a2 - (0x400000 / s2->f28) * 8;
        }
        sp10[0] = (RaceContext.f20 / 4) - s2->x;
        sp10[1] = (RaceContext.f24 / 4) - s2->y;
        sp10[2] = (RaceContext.f28 / 4) - s2->z;
        scratch[2] = RaceContext.f20 - ((sp10[0] * t1) / a2) * 4;
        scratch[3] = RaceContext.f24 - ((sp10[1] * t1) / a2) * 4;
        scratch[4] = RaceContext.f28 - ((sp10[2] * t1) / a2) * 4;
        a0 = (t1 * (s2->f28 << 4)) / 4096;
        if (a0 >= 0x401) {
            a0 = 0x400;
        } else if (a0 < 0x118) {
            a0 = 0x118;
        }
        SetGeomScreen(a0);
        sp10[0] = RaceContext.f20 - scratch[2];
        sp10[2] = RaceContext.f28 - scratch[4];
        sp10[1] = RaceContext.f24 - scratch[3];
        scratch[7] = 0x400 - Atan2Table(sp10[0], sp10[2]);
        scratch[6] = 0x400 - Atan2Table(sp10[1], CordicSqrt(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = LerpAngleWrap4096(s2, RaceContext.f48);
        break;
    case 3:
        scratch[2] = s2->x * 4;
        scratch[3] = s2->y * 4;
        scratch[4] = s2->z * 4;
        scratch[6] = s2->f10;
        scratch[7] = s2->f14;
        scratch[8] = LerpAngleWrap4096(s2, D_800AC698);
        SetGeomScreen(s2->f28);
        break;
    case 4:
        scratch[2] = s2->x * 4;
        scratch[3] = s2->y * 4;
        scratch[4] = s2->z * 4;
        RaceContext.f30 = 0;
        RaceContext.f32 = 0;
        RaceContext.f34 = -0x100;
        CarLocalToWorld(arg1, &RaceContext.f30, &RaceContext.f20);
        sp30[0] = s2->f10;
        sp30[1] = s2->f14;
        sp30[2] = 0;
        RotateNegZByEulerZYX(sp30, s2->f18, sp20);
        sp20[0] += s2->x;
        sp20[1] += s2->y;
        sp20[2] += s2->z;
        RaceContext.f20 = ((((RaceContext.f20 / 4) - sp20[0]) * s2->f2C) / 1024 + sp20[0]) * 4;
        RaceContext.f24 = ((((RaceContext.f24 / 4) - sp20[1]) * s2->f2C) / 1024 + sp20[1]) * 4;
        RaceContext.f28 = ((((RaceContext.f28 / 4) - sp20[2]) * s2->f2C) / 1024 + sp20[2]) * 4;
        sp10[0] = RaceContext.f20 - scratch[2];
        sp10[2] = RaceContext.f28 - scratch[4];
        sp10[1] = RaceContext.f24 - scratch[3];
        scratch[7] = 0x400 - Atan2Table(sp10[0], sp10[2]);
        scratch[6] = 0x400 - Atan2Table(sp10[1], CordicSqrt(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = LerpAngleWrap4096(s2, RaceContext.f48);
        SetGeomScreen(s2->f28);
        break;
    case 5:
        GetTrackLaneWorldPos(arg1, &RaceContext.f20, s2);
        sp38[0] = s2->f10;
        sp38[1] = s2->f14;
        sp38[2] = s2->f18;
        TransformCameraOffset(sp38, s2->f2C, &scratch[2]);
        scratch[8] = LerpAngleWrap4096(s2, RaceContext.f48);
        scratch[2] += RaceContext.f20;
        scratch[3] += RaceContext.f24;
        scratch[4] += RaceContext.f28;
        SetGeomScreen(s2->f28);
        break;
    case 6:
        RaceContext.f30 = 0;
        RaceContext.f32 = 0;
        RaceContext.f34 = -0x100;
        CarLocalToWorld(arg1, &RaceContext.f30, &RaceContext.f20);
        GetTrackLaneWorldPos(arg1, &scratch[2], s2);
        sp10[0] = RaceContext.f20 - scratch[2];
        sp10[2] = RaceContext.f28 - scratch[4];
        sp10[1] = RaceContext.f24 - scratch[3];
        scratch[7] = 0x400 - Atan2Table(sp10[0], sp10[2]);
        scratch[6] = 0x400 - Atan2Table(sp10[1], CordicSqrt(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = LerpAngleWrap4096(s2, RaceContext.f48);
        SetGeomScreen(s2->f28);
        break;
    }
    BuildCameraMatrices();
}

void InitTrackStartPos(void) {
    int *s = TrackNodes;
    D_800AC548[0] = s[2];
    D_800AC548[1] = 0;
    D_800AC548[2] = s[3];
    D_800AC548[4] = 0;
    D_800AC548[5] = 0;
    D_800AC548[6] = 0;
}

void BuildObjectLightAndRotMatrices(A0 *a0, MatrixWords32 *a1) {
    struct { MatrixWords32 m10; MatrixWords32 m30; MatrixWords32 m50; } s;
    MatrixWords32 t70;
    MatrixWords32 t90;
    V4 b0;
    int a3;
    b0 = D_80010F1C;
    BuildRotMatrixY(&s.m50, -a0->rotY12);
    MulMatrix2((const MatrixWords32 *)((int *)SCRATCH_ROT_MATRIX), &s.m50);
    BuildRotMatrixX(&s.m10, -a0->rotX10);
    BuildRotMatrixZ(&s.m30, -a0->rotZ14);
    MulMatrix(&s.m10, &s.m30);
    MulMatrix2(&s.m50, &s.m10);
    BuildRotMatrixY(&s.m50, a0->rotY12 - 0x400);
    BuildRotMatrixX(&s.m30, a0->rotX10);
    MulMatrix(&s.m50, &s.m30);
    BuildRotMatrixZ(&s.m30, a0->rotZ14);
    MulMatrix2(&s.m50, &s.m30);
    a3 = 0;
    do {
        t70.fields.rotation00[a3][0] = a0->v[a3].a;
        t70.fields.rotation00[a3][1] = -a0->v[a3].b;
        t70.fields.rotation00[a3][2] = a0->v[a3].c;
        t90.fields.rotation00[0][a3] = a0->c[a3].a * 16;
        t90.fields.rotation00[1][a3] = a0->c[a3].b * 16;
        t90.fields.rotation00[2][a3] = a0->c[a3].c * 16;
        a3++;
    } while (a3 < 3);
    MulMatrix0(&t70, &s.m30, &s.m50);
    SetLightMatrix(&s.m50);
    SetColorMatrix(&t90);
    SetBackColor(a0->b3C, a0->b3D, a0->b3E);
    ScaleMatrix(&s.m10, &b0);
    *a1 = s.m10;
}

int RotTransPersOrigin(void) {
    short a[3];
    int b, c, d;
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    return RotTransPers(a, &b, &c, &d);
}

void InitRaceContextBuffers(void) {
    int i;
    for (i = 0; i < 2; i++) {
        RaceContext.e[i].a = &D_800AE660[i];
        RaceContext.e[i].b = &D_800D0DD8[i];
    }
}

u8 *EmitSpotlightConePolys(s32 a0) {
    GfxGteScratch *g = (GfxGteScratch *)(SCRATCHPAD + 0x7C);
    PolyG3Packet *p = *(PolyG3Packet **)SCRATCHPAD;
    s32 i;
    s32 t;
    g->c0[0] = 0x50;
    g->c0[1] = 0x50;
    g->c0[2] = 0x50;
    g->c1[0] = 0;
    g->c1[1] = 0;
    g->c1[2] = 0;
    g->v[0].x = 0;
    g->v[0].y = 0;
    g->v[0].z = 0;
    i = 0;
    do {
        s32 *ot;
        s32 w, o;
        s32 a = i << 8;
        g->v[1].x = (rsin(a) * 800) / 4096;
        g->v[1].y = 0;
        g->v[1].z = (rcos(a) * 800) / 4096;
        i++;
        a = i << 8;
        g->v[2].x = (rsin(a) * 800) / 4096;
        g->v[2].y = 0;
        g->v[2].z = (rcos(a) * 800) / 4096;
        RR_GTE_LWC2_V012_3P(&g->v[0], &g->v[1], &g->v[2]);
        RR_GTE_RTPT_NOP2();
        RR_GTE_SWC2_SXY_3P(&g->sxy[0], &g->sxy[2], &g->sxy[4]);
        p->len = 6;
        p->code = 0x33;
        p->x0 = g->sxy[0];
        p->y0 = g->sxy[1];
        p->x1 = g->sxy[2];
        p->y1 = g->sxy[3];
        p->x2 = g->sxy[4];
        p->y2 = g->sxy[5];
        p->r0 = g->c0[0];
        p->g0 = g->c0[1];
        p->b0 = g->c0[2];
        p->r1 = g->c1[0];
        p->g1 = g->c1[1];
        p->b1 = g->c1[2];
        p->r2 = g->c1[0];
        p->g2 = g->c1[1];
        p->b2 = g->c1[2];
        w = *(s32 *)p;
        ot = (s32 *)(a0 * 4 + (s32)CurrentDrawBuf);
        o = ot[0x70 / 4];
        o = o & 0xFFFFFF;
        w = w & 0xFF000000;
        *(s32 *)p = w | o;
        ot[0x70 / 4] = (ot[0x70 / 4] & 0xFF000000) | ((s32)p & 0xFFFFFF);
        p++;
    } while (i < 16);
    {
        s32 *ot;
        p->len = 1;
        ot = (s32 *)(a0 * 4 + (s32)CurrentDrawBuf);
        ((s32 *)p)[1] = 0xE1000245;
        *(s32 *)p = (*(s32 *)p & 0xFF000000) | (ot[0x70 / 4] & 0xFFFFFF);
        ot[0x70 / 4] = (ot[0x70 / 4] & 0xFF000000) | ((s32)p & 0xFFFFFF);
    }
    {
        u8 *r;
        /* ASM_BARRIER: forces p+8 computed after final stores (return value copy) */
        __asm__ volatile ("" : "=r"(r) : "0"((u8 *)p + 8));
        *(u8 **)SCRATCHPAD = r;
        return r;
    }
}
