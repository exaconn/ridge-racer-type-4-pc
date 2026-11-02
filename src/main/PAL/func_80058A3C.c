#include "types.h"

typedef struct {
    s16 vx, vy, vz, pad;
} SVECTOR;

typedef struct {
    u32 addr: 24;
    u32 len: 8;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 w, h;
} TILE;

typedef struct {
    u32 addr: 24;
    u32 len: 8;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    s16 clut;
    s16 w, h;
} SPRT;

typedef struct {
    u32 addr: 24;
    u32 len: 8;
    u32 code[1];
} DR_MODE;

typedef struct {
    s32 a, b, c, d;
    s32 e[11];
} S3C;

typedef struct {
    u8 u, v, w, h;
} UvRect;

typedef struct {
    void *p;
} Scratch;

#define SCRATCH ((Scratch *)0x1F800000)

extern int func_8004D290();
extern s32 D_800A101C;
extern UvRect D_800A1020[];
extern UvRect D_800A1028;
extern UvRect D_800A102C;
extern s16 D_800A1030[];
extern s16 D_800A1040[];
extern struct { u16 x, y; } D_800A1050[];
extern s32 D_800A1070[];
extern S3C *D_800ABEA4;
extern s32 D_800ABECC;
extern s16 D_800AC648;
extern u8 *D_800ACC14;
extern s16 D_800F3914;

#define ADDPRIM(ot, p) \
    (p)->addr = ((SPRT *)(ot))->addr; \
    ((SPRT *)(ot))->addr = (u32)(p)

void func_80058A3C(s32 *arg0) {
    SVECTOR sp10;
    SVECTOR sp18;
    s16 sp20[4];
    u8 sp28[4];
    TILE *tile;
    SPRT *sprt;
    DR_MODE *drmode;
    s32 i;
    s16 *s1;
    UvRect *s3;
    s32 s2;
    S3C *base;
    S3C *sec;

    tile = SCRATCH->p;
    sp10.vx = D_800A1050[D_800F3914].x;
    sp10.vy = D_800A1050[D_800F3914].y;

    for (i = 0; i < D_800ABECC; i++) {
        sp18.vx = D_800ABEA4[i].c / 384;
        sp18.vy = -D_800ABEA4[i].d / 192;
        sp18.vx += sp10.vx;
        sp18.vy += sp10.vy;
        tile->len = 3;
        tile->code = 0x62;
        tile->x0 = sp18.vx - 1;
        tile->y0 = sp18.vy - 3;
        tile->w = 3;
        tile->h = 6;
        tile->r0 = 0xFF;
        tile->g0 = 0xDE;
        tile->b0 = 0x7F;
        ADDPRIM(D_800ACC14 + 0x84, tile);
        tile++;
    }
    SCRATCH->p = tile;

    if (D_800AC648 < 8) {
        s1 = D_800A1030;
        s3 = &D_800A1028;
    } else {
        s1 = D_800A1040;
        s3 = &D_800A102C;
    }

    base = D_800ABEA4;
    sec = (S3C *)(D_800A1070[D_800F3914] * 0x3C + (s32)base);
    sp18.vx = sec->c / 384;
    sp18.vy = -sec->d / 192;
    sp18.vx += sp10.vx;
    sp18.vy += sp10.vy;

    s2 = sp20[0] = s1[0] + sp18.vx;
    sp20[1] = s1[1] + sp18.vy;
    sp20[2] = s1[2] + sp18.vx;
    sp20[3] = s1[3] + sp18.vy;
    sp28[0] = 0xFF;
    sp28[1] = 0xDE;
    sp28[2] = 0x7F;

    for (i = 0; i < 4; i++) {
        sp20[2] = sp20[0] = s2 + (i & 1);
        func_8004D290(sp20, sp28, 1, 5);
    }

    sprt = SCRATCH->p;
    sprt->len = 4;
    sprt->code = 0x64;
    sprt->u0 = D_800A1020[1].u;
    sprt->v0 = D_800A1020[1].v;
    sprt->w = D_800A1020[1].w;
    sprt->h = D_800A1020[1].h;
    sprt->x0 = s1[4] + sp18.vx;
    sprt->y0 = s1[5] + sp18.vy;
    sprt->code |= 3;
    sprt->clut = 0x7FE5;
    ADDPRIM(D_800ACC14 + 0x84, sprt);
    sprt++;

    sprt->len = 4;
    sprt->code = 0x64;
    sprt->u0 = s3->u;
    sprt->v0 = s3->v;
    sprt->w = s3->w;
    sprt->h = s3->h;
    sprt->x0 = s1[6] + sp18.vx;
    sprt->y0 = s1[7] + sp18.vy;
    sprt->code |= 3;
    sprt->clut = 0x7FE5;
    ADDPRIM(D_800ACC14 + 0x84, sprt);
    sprt++;

    SCRATCH->p = sprt;
    drmode = (DR_MODE *)sprt;
    drmode->len = 1;
    drmode->code[0] = 0xE1000219;
    ADDPRIM(D_800ACC14 + 0x84, drmode);
    drmode++;
    SCRATCH->p = drmode;

    sp18.vx = arg0[0] / 384;
    sp18.vy = -arg0[2] / 192;
    sp18.vx += sp10.vx;
    sp18.vy += sp10.vy;

    D_800A101C &= 0xFF;
    if (D_800A101C & 0x80) {
        D_800A101C &= 0x7F;
        if (D_800A101C < 0x7A) {
            D_800A101C = (D_800A101C + 6) | 0x80;
        } else {
            D_800A101C = 0x7F;
        }
    } else {
        D_800A101C &= 0x7F;
        if (D_800A101C >= 6) {
            D_800A101C -= 6;
        } else {
            D_800A101C = 0x80;
        }
    }

    sprt = SCRATCH->p;
    sprt->len = 4;
    sprt->code = 0x64;
    sprt->u0 = D_800A1020[0].u;
    sprt->v0 = D_800A1020[0].v;
    sprt->w = D_800A1020[0].w;
    sprt->h = D_800A1020[0].h;
    sprt->r0 = D_800A101C & 0x7F;
    sprt->g0 = D_800A101C & 0x7F;
    sprt->b0 = D_800A101C & 0x7F;
    sprt->x0 = sp18.vx - 4;
    sprt->y0 = sp18.vy - 8;
    sprt->code = (sprt->code | 2) & ~1;
    sprt->clut = 0x7FE5;
    ADDPRIM(D_800ACC14 + 0x80, sprt);
    sprt++;

    SCRATCH->p = sprt;
    drmode = (DR_MODE *)sprt;
    drmode->len = 1;
    drmode->code[0] = 0xE1000239;
    ADDPRIM(D_800ACC14 + 0x80, drmode);
    drmode++;
    SCRATCH->p = drmode;
}
