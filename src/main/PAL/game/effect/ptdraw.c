#include "rr/ui_primitives.h"

typedef struct {
    s32 active00, state04, timer08;
    s16 x0C, y0E;
    RrGpuColorCommand c10;
    s16 s14;
    u16 u16;
    s16 s18, s1A, s1C, s1E;
} RrParticle20;

typedef struct {
    s32 pad00[6];
    s32 val18;
    s32 pad1C[4];
    RrParticle20 e[24];
} RrParticleSys;

typedef struct {
    u32 addr:24;
    u32 len:8;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad1;
    s16 x3, y3;
    u8 u3, v3;
    u16 pad2;
} PolyFT4;

extern RrParticleSys D_800F2AB8;
extern struct {
    char pad[0x3E8];
    s32 f3E8;
} D_800AC650;
extern u8 *D_800ACC14;
extern RrGpuColorCommand D_800A0CF0;

#define ADDPRIM(ot, p) \
    (p)->addr = ((PolyFT4 *)(ot))->addr; \
    ((PolyFT4 *)(ot))->addr = (u32)(p)

extern void func_8004CD88();

void func_80054264(int a0) {
    RrGpuColorCommand c[4];
    RrGpuRectangleFields rect;
    RrParticle20 *e;
    PolyFT4 *p;
    int i, v;
    int s3, sp30, s7, s1;

    e = &D_800F2AB8.e[a0];
    c[0] = c[1] = c[2] = c[3] = e->c10;
    for (i = 0; i < 4; i++) {
        v = (c[i].r * D_800F2AB8.val18) / 256;
        v = (v * D_800AC650.f3E8) / 256;
        c[i].r = v;
        v = (c[i].g * D_800F2AB8.val18) / 256;
        v = (v * D_800AC650.f3E8) / 256;
        c[i].g = v;
        v = (c[i].b * D_800F2AB8.val18) / 256;
        v = (v * D_800AC650.f3E8) / 256;
        c[i].b = v;
    }
    c[1].r /= 2;
    c[1].g /= 2;
    c[1].b /= 2;
    c[2].r /= 4;
    c[2].g /= 4;
    c[2].b /= 4;
    c[3].r = c[2].r + c[2].r / 2;
    c[3].g = c[2].g + c[2].g / 2;
    c[3].b = c[2].b + c[2].b / 2;
    for (i = 0; i < 4; i++) {
        c[i].r = (c[i].r * e->s1A) / 256;
        c[i].g = (c[i].g * e->s1A) / 256;
        c[i].b = (c[i].b * e->s1A) / 256;
    }

    p = *(PolyFT4 **)0x1F800000;
    if (e->s1E >= 4) {
        p->len = 9;
        p->code = 0x2E;
        p->r0 = c[3].r;
        p->g0 = c[3].g;
        p->b0 = c[3].b;
        p->x0 = e->x0C - 9;
        p->y0 = e->y0E - 0x18;
        p->x1 = e->x0C + 3;
        p->y1 = e->y0E - 0x18;
        p->x2 = e->x0C - 9;
        p->y2 = e->y0E + 0x18;
        p->x3 = e->x0C + 3;
        p->y3 = e->y0E + 0x18;
        p->u0 = ((s16)e->u16 / 2) * 16 + 0x30;
        p->v0 = 0x70;
        p->u1 = ((s16)e->u16 / 2) * 16 + 0x3C;
        p->v1 = 0x70;
        p->u2 = ((s16)e->u16 / 2) * 16 + 0x30;
        p->v2 = 0xA0;
        p->u3 = ((s16)e->u16 / 2) * 16 + 0x3C;
        p->v3 = 0xA0;
        p->tpage = 0xA5;
        p->clut = 0x7EB8;
        ADDPRIM(D_800ACC14 + 0x188, p);
        p++;
    }

    p->len = 9;
    p->code = 0x2E;
    p->r0 = c[2].r;
    p->g0 = c[2].g;
    p->b0 = c[2].b;
    p->x0 = e->x0C - 0x10;
    p->y0 = e->y0E - 0x18;
    p->x1 = e->x0C;
    p->y1 = e->y0E - 0x18;
    p->x2 = e->x0C - 0x10;
    p->y2 = e->y0E + 0x18;
    p->x3 = e->x0C;
    p->y3 = e->y0E + 0x18;
    p->u0 = ((s16)e->u16 / 2) * 16 + 0x30;
    p->v0 = 0x70;
    p->u1 = ((s16)e->u16 / 2) * 16 + 0x40;
    p->v1 = 0x70;
    p->u2 = ((s16)e->u16 / 2) * 16 + 0x30;
    p->v2 = 0xA0;
    p->u3 = ((s16)e->u16 / 2) * 16 + 0x40;
    p->v3 = 0xA0;
    p->tpage = 0xA5;
    p->clut = 0x7EB8;
    ADDPRIM(D_800ACC14 + 0x188, p);
    p++;

    if (e->s1E >= 5) {
        p->len = 9;
        p->code = 0x2E;
        p->r0 = c[0].r;
        p->g0 = c[0].g;
        p->b0 = c[0].b;
        p->x0 = e->x0C + e->s18 - 0xC;
        p->y0 = e->y0E - 0x18;
        p->x1 = e->x0C + e->s18 + 4;
        p->y1 = e->y0E - 0x18;
        p->x2 = e->x0C + e->s18 - 0xC;
        p->y2 = e->y0E + 0x18;
        p->x3 = e->x0C + e->s18 + 4;
        p->y3 = e->y0E + 0x18;
        p->u0 = ((s16)e->u16 / 2) * 16 + 0x30;
        p->v0 = 0x70;
        p->u1 = ((s16)e->u16 / 2) * 16 + 0x40;
        p->v1 = 0x70;
        p->u2 = ((s16)e->u16 / 2) * 16 + 0x30;
        p->v2 = 0xA0;
        p->u3 = ((s16)e->u16 / 2) * 16 + 0x40;
        p->v3 = 0xA0;
        p->tpage = 0xA5;
        p->clut = 0x7EB8;
        ADDPRIM(D_800ACC14 + 0x188, p);
        p++;
    }
    *(PolyFT4 **)0x1F800000 = p;
    /* ASM_BARRIER: keep scratchpad packet pointer store before s18 clamp */
    __asm__ volatile("");

    s3 = 0x18;
    if (e->s18 < 0x18) {
        s3 = e->s18;
    }
    sp30 = e->s18 - s3;
    s7 = e->x0C + 3;
    if (e->s1E >= 5) {
        s1 = s7 + sp30;
        rect.x = s1;
        rect.y = e->y0E - (s16)e->u16 / 2 - 0x10;
        rect.w = s3;
        rect.h = 0x10;
        func_8004CD88(&rect, 0, &D_800A0CF0, &c[2], 0x41);

        rect.x = s1;
        rect.y = e->y0E - (s16)e->u16 / 2;
        rect.w = s3;
        rect.h = (s16)e->u16 / 2;
        func_8004D11C(&rect, &c[1], &c[2], &c[0], &c[1], 0x41);

        rect.x = s1;
        rect.y = e->y0E;
        rect.w = s3;
        rect.h = (s16)e->u16 / 2;
        func_8004D11C(&rect, &c[0], &c[1], &c[1], &c[2], 0x41);

        rect.x = s1;
        rect.y = e->y0E + (s16)e->u16 / 2;
        rect.w = s3;
        rect.h = 0x10;
        func_8004CD88(&rect, 1, &c[2], &D_800A0CF0, 0x41);

        if (sp30 > 0) {
            rect.x = s7;
            rect.y = e->y0E - (s16)e->u16 / 2 - 0x10;
            rect.w = e->s18 - s3;
            rect.h = 0x10;
            func_8004CFB0(&rect, &D_800A0CF0, &c[2], 0x41);

            rect.x = s7;
            rect.y = e->y0E;
            rect.w = e->s18 - s3;
            rect.h = (s16)e->u16 / 2;
            func_8004CFB0(&rect, &c[0], &c[1], 0x41);

            rect.x = s7;
            rect.y = e->y0E - (s16)e->u16 / 2;
            rect.w = e->s18 - s3;
            rect.h = (s16)e->u16 / 2;
            func_8004CFB0(&rect, &c[1], &c[0], 0x41);

            rect.x = s7;
            rect.y = e->y0E + (s16)e->u16 / 2;
            rect.w = e->s18 - s3;
            rect.h = 0x10;
            func_8004CFB0(&rect, &c[2], &D_800A0CF0, 0x41);
        }
    }
}
