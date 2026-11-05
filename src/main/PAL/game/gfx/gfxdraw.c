/* RODATA 0x80010f00 0x1c */
#include "types.h"
#include "scratchpad.h"
#include "game/course/trkintp2.h"
#include "game/gfx/dispenv.h"
#include "game/course/trkintp2.h"
#include "game/gfx/dispenv.h"
#include "psyq/libgte/gtemat7.h"
#include "rr/audio_loop.h"
#include "rr/overlay662_distance.h"
#include "rr/fade_sprite.h"

typedef struct {
    int x, y, z, f0C;
    int f10, f14, f18, f1C;
    int f20, f24, f28, f2C;
    unsigned short f30;
    char pad32[0x12];
} CamNode;

typedef struct {
    char pad0[0x1C];
    int f1C;
    int f20, f24, f28, f2C;
    short f30, f32, f34, f36;
    char pad38[0x10];
    int f48, f4C;
    char pad50[0x500];
    CamNode *f550;
} Context650;

typedef struct { int x, y, z, w; } V4;

typedef struct { V4 a, b; } V8;

typedef struct { unsigned char a, b, c, d; } C4;

typedef struct { unsigned short a, b, c, d; } S4;

typedef struct { char p0[0x18]; C4 c[3]; S4 v[3]; unsigned char b3C, b3D, b3E; } A0;

typedef struct { char pad[0x57C]; } B;

typedef struct { int a; int b; } P;

typedef struct { int pad[20]; P e[2]; } S;

typedef struct { s16 x, y, z, pad; } w3t1_SV;

typedef struct {
    char pad0[8];
    u16 sxy[6];
    char pad1[0x24 - 0x14];
    w3t1_SV v[3];
    char pad2[0x44 - 0x3C];
    u8 c0[3];
    u8 pad3;
    u8 c1[3];
} w3t1_G;

extern Context650 D_800AC650;
extern int D_800AC698;
extern int D_8010718C;
extern void func_800172DC(int, int);
extern void func_80033BE4(void *);
extern void func_8006E1AC(void);
extern int func_8006E368(int, int);
extern void func_800912D4(int);
extern int *D_800ABEA4;
extern int D_800AC548[];
extern int BuildRotMatrixZ();
extern int BuildRotMatrixY();
extern int BuildRotMatrixX();
extern int MulMatrix0();
extern int MulMatrix();
extern int MulMatrix2();
extern int func_80090F74();
extern int SetLightMatrix();
extern V4 D_80010F1C;
extern int func_800916D4();
extern B D_800D0DD8[];
extern B D_800AE660[];
extern s32 rsin();
extern s32 rcos();

void func_8004417C(int arg0, void *arg1);
void func_800448EC(void);
void func_80044920();
void func_80044B24(void);
void func_80044B5C(void);
u8 *func_80044BA0(s32 a0);

void func_8004417C(int arg0, void *arg1) {
    int sp10[4];
    int sp20[4];
    unsigned short sp30[4];
    unsigned short sp38[4];
    CamNode *s2;
    int a0;
    int a1;
    int a2;
    int t1;
    int *scratch;

    s2 = D_800AC650.f550;
    a0 = func_80043730(arg1);
    scratch = (int *)SCRATCHPAD;
    if (arg0 >= 2) {
        s2 = D_800AC650.f550;
        s2 += a0;
        D_8010718C = 0;
        arg0 = s2->f30 & 0xF;
    }
    D_800AC650.f1C = 0;
    a1 = D_800AC650.f48 + 1;
    D_800AC650.f48 = a1;
    if (a0 != D_800AC650.f4C) {
        if (D_800F4D44 == 5 || a1 >= 0x1A) {
            D_800AC650.f1C = 1;
            D_800AC650.f4C = a0;
            D_800AC650.f48 = 0;
        }
        func_800172DC(a0, a1);
    }
    switch (arg0) {
    case 0:
        func_80033BE4(arg1);
        break;
    case 1:
        scratch[2] = s2->x * 4;
        scratch[3] = s2->y * 4;
        scratch[4] = s2->z * 4;
        D_800AC650.f30 = 0;
        D_800AC650.f32 = 0;
        D_800AC650.f34 = -0x100;
        func_800439BC(arg1, &D_800AC650.f30, &D_800AC650.f20);
        sp10[0] = D_800AC650.f20 - scratch[2];
        sp10[2] = D_800AC650.f28 - scratch[4];
        sp10[1] = D_800AC650.f24 - scratch[3];
        scratch[7] = 0x400 - func_8006E368(sp10[0], sp10[2]);
        scratch[6] = 0x400 - func_8006E368(sp10[1], func_80090758(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = func_80043FC0(s2, D_800AC650.f48);
        func_800912D4(s2->f28);
        break;
    case 2:
        D_800AC650.f30 = 0;
        D_800AC650.f32 = 0;
        D_800AC650.f34 = -0x100;
        func_800439BC(arg1, &D_800AC650.f30, &D_800AC650.f20);
        a2 = func_80043C14(arg1, s2) >> 4;
        t1 = a2 / 4;
        if (a2 - t1 > (0x400000 / s2->f28) * 8) {
            t1 = a2 - (0x400000 / s2->f28) * 8;
        }
        sp10[0] = (D_800AC650.f20 / 4) - s2->x;
        sp10[1] = (D_800AC650.f24 / 4) - s2->y;
        sp10[2] = (D_800AC650.f28 / 4) - s2->z;
        scratch[2] = D_800AC650.f20 - ((sp10[0] * t1) / a2) * 4;
        scratch[3] = D_800AC650.f24 - ((sp10[1] * t1) / a2) * 4;
        scratch[4] = D_800AC650.f28 - ((sp10[2] * t1) / a2) * 4;
        a0 = (t1 * (s2->f28 << 4)) / 4096;
        if (a0 >= 0x401) {
            a0 = 0x400;
        } else if (a0 < 0x118) {
            a0 = 0x118;
        }
        func_800912D4(a0);
        sp10[0] = D_800AC650.f20 - scratch[2];
        sp10[2] = D_800AC650.f28 - scratch[4];
        sp10[1] = D_800AC650.f24 - scratch[3];
        scratch[7] = 0x400 - func_8006E368(sp10[0], sp10[2]);
        scratch[6] = 0x400 - func_8006E368(sp10[1], func_80090758(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = func_80043FC0(s2, D_800AC650.f48);
        break;
    case 3:
        scratch[2] = s2->x * 4;
        scratch[3] = s2->y * 4;
        scratch[4] = s2->z * 4;
        scratch[6] = s2->f10;
        scratch[7] = s2->f14;
        scratch[8] = func_80043FC0(s2, D_800AC698);
        func_800912D4(s2->f28);
        break;
    case 4:
        scratch[2] = s2->x * 4;
        scratch[3] = s2->y * 4;
        scratch[4] = s2->z * 4;
        D_800AC650.f30 = 0;
        D_800AC650.f32 = 0;
        D_800AC650.f34 = -0x100;
        func_800439BC(arg1, &D_800AC650.f30, &D_800AC650.f20);
        sp30[0] = s2->f10;
        sp30[1] = s2->f14;
        sp30[2] = 0;
        func_80043F5C(sp30, s2->f18, sp20);
        sp20[0] += s2->x;
        sp20[1] += s2->y;
        sp20[2] += s2->z;
        D_800AC650.f20 = ((((D_800AC650.f20 / 4) - sp20[0]) * s2->f2C) / 1024 + sp20[0]) * 4;
        D_800AC650.f24 = ((((D_800AC650.f24 / 4) - sp20[1]) * s2->f2C) / 1024 + sp20[1]) * 4;
        D_800AC650.f28 = ((((D_800AC650.f28 / 4) - sp20[2]) * s2->f2C) / 1024 + sp20[2]) * 4;
        sp10[0] = D_800AC650.f20 - scratch[2];
        sp10[2] = D_800AC650.f28 - scratch[4];
        sp10[1] = D_800AC650.f24 - scratch[3];
        scratch[7] = 0x400 - func_8006E368(sp10[0], sp10[2]);
        scratch[6] = 0x400 - func_8006E368(sp10[1], func_80090758(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = func_80043FC0(s2, D_800AC650.f48);
        func_800912D4(s2->f28);
        break;
    case 5:
        func_80043DE4(arg1, &D_800AC650.f20, s2);
        sp38[0] = s2->f10;
        sp38[1] = s2->f14;
        sp38[2] = s2->f18;
        func_80043ED4(sp38, s2->f2C, &scratch[2]);
        scratch[8] = func_80043FC0(s2, D_800AC650.f48);
        scratch[2] += D_800AC650.f20;
        scratch[3] += D_800AC650.f24;
        scratch[4] += D_800AC650.f28;
        func_800912D4(s2->f28);
        break;
    case 6:
        D_800AC650.f30 = 0;
        D_800AC650.f32 = 0;
        D_800AC650.f34 = -0x100;
        func_800439BC(arg1, &D_800AC650.f30, &D_800AC650.f20);
        func_80043DE4(arg1, &scratch[2], s2);
        sp10[0] = D_800AC650.f20 - scratch[2];
        sp10[2] = D_800AC650.f28 - scratch[4];
        sp10[1] = D_800AC650.f24 - scratch[3];
        scratch[7] = 0x400 - func_8006E368(sp10[0], sp10[2]);
        scratch[6] = 0x400 - func_8006E368(sp10[1], func_80090758(sp10[0] * sp10[0] + sp10[2] * sp10[2]) >> 6);
        scratch[8] = func_80043FC0(s2, D_800AC650.f48);
        func_800912D4(s2->f28);
        break;
    }
    func_8006E1AC();
}

void func_800448EC(void) {
    int *s = D_800ABEA4;
    D_800AC548[0] = s[2];
    D_800AC548[1] = 0;
    D_800AC548[2] = s[3];
    D_800AC548[4] = 0;
    D_800AC548[5] = 0;
    D_800AC548[6] = 0;
}

void func_80044920(a0, a1) A0 *a0; V8 *a1;
{
    struct { V8 m10; V8 m30; V8 m50; } s;
    struct { short a[3][3]; short pad[7]; } t70;
    struct { short a[3][3]; short pad[7]; } t90;
    V4 b0;
    int a3;
    b0 = D_80010F1C;
    BuildRotMatrixY(&s.m50, -*(short *)((char *)a0 + 0x12));
    MulMatrix2((int *)SCRATCH_ROT_MATRIX, &s.m50);
    BuildRotMatrixX(&s.m10, -*(short *)((char *)a0 + 0x10));
    BuildRotMatrixZ(&s.m30, -*(short *)((char *)a0 + 0x14));
    MulMatrix(&s.m10, &s.m30);
    MulMatrix2(&s.m50, &s.m10);
    BuildRotMatrixY(&s.m50, *(short *)((char *)a0 + 0x12) - 0x400);
    BuildRotMatrixX(&s.m30, *(short *)((char *)a0 + 0x10));
    MulMatrix(&s.m50, &s.m30);
    BuildRotMatrixZ(&s.m30, *(short *)((char *)a0 + 0x14));
    MulMatrix2(&s.m50, &s.m30);
    a3 = 0;
    do {
        t70.a[a3][0] = a0->v[a3].a;
        t70.a[a3][1] = -a0->v[a3].b;
        t70.a[a3][2] = a0->v[a3].c;
        t90.a[0][a3] = a0->c[a3].a * 16;
        t90.a[1][a3] = a0->c[a3].b * 16;
        t90.a[2][a3] = a0->c[a3].c * 16;
        a3++;
    } while (a3 < 3);
    MulMatrix0(t70.a, &s.m30, &s.m50, a3);
    SetLightMatrix(&s.m50);
    SetColorMatrix(t90.a);
    SetBackColor(a0->b3C, a0->b3D, a0->b3E);
    func_80090F74(&s.m10, &b0);
    *a1 = s.m10;
}

void func_80044B24(void) {
    short a[3];
    int b, c, d;
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    func_800916D4(a, &b, &c, &d);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(S (*))&D_800AC650)

void func_80044B5C(void) {
    int i;
    for (i = 0; i < 2; i++) {
        D_800AC650.e[i].a = (int)&D_800AE660[i];
        D_800AC650.e[i].b = (int)&D_800D0DD8[i];
    }
}

#undef D_800AC650

u8 *func_80044BA0(s32 a0) {
    w3t1_G *g = (w3t1_G *)(SCRATCHPAD + 0x7C);
    u8 *p = *(u8 **)SCRATCHPAD;
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
        __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)\n\tlwc2 $2, 0(%1)\n\tlwc2 $3, 4(%1)\n\tlwc2 $4, 0(%2)\n\tlwc2 $5, 4(%2)" : : "r"(&g->v[0]), "r"(&g->v[1]), "r"(&g->v[2]));
        __asm__ volatile ("nop\n\tnop\n\t.word 0x4A280030");
        __asm__ volatile ("swc2 $12, 0(%0)\n\tswc2 $13, 0(%1)\n\tswc2 $14, 0(%2)" : : "r"(&g->sxy[0]), "r"(&g->sxy[2]), "r"(&g->sxy[4]));
        p[3] = 6;
        p[7] = 0x33;
        *(u16 *)(p + 8) = g->sxy[0];
        *(u16 *)(p + 0xA) = g->sxy[1];
        *(u16 *)(p + 0x10) = g->sxy[2];
        *(u16 *)(p + 0x12) = g->sxy[3];
        *(u16 *)(p + 0x18) = g->sxy[4];
        *(u16 *)(p + 0x1A) = g->sxy[5];
        p[4] = g->c0[0];
        p[5] = g->c0[1];
        p[6] = g->c0[2];
        p[0xC] = g->c1[0];
        p[0xD] = g->c1[1];
        p[0xE] = g->c1[2];
        p[0x14] = g->c1[0];
        p[0x15] = g->c1[1];
        p[0x16] = g->c1[2];
        w = *(s32 *)p;
        ot = (s32 *)(a0 * 4 + (s32)D_800ACC14);
        o = ot[0x70 / 4];
        o = o & 0xFFFFFF;
        w = w & 0xFF000000;
        *(s32 *)p = w | o;
        ot[0x70 / 4] = (ot[0x70 / 4] & 0xFF000000) | ((s32)p & 0xFFFFFF);
        p += 0x1C;
    } while (i < 16);
    {
        s32 *ot;
        p[3] = 1;
        ot = (s32 *)(a0 * 4 + (s32)D_800ACC14);
        *(s32 *)(p + 4) = 0xE1000245;
        *(s32 *)p = (*(s32 *)p & 0xFF000000) | (ot[0x70 / 4] & 0xFFFFFF);
        ot[0x70 / 4] = (ot[0x70 / 4] & 0xFF000000) | ((s32)p & 0xFFFFFF);
    }
    {
        u8 *r;
        /* ASM_BARRIER: forces p+8 computed after final stores (return value copy) */
        __asm__ volatile ("" : "=r"(r) : "0"(p + 8));
        *(u8 **)SCRATCHPAD = r;
        return r;
    }
}
