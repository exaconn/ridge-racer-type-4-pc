#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define UiRows (*(int (*))&UiRows)

#undef UiRows

/* legacy typing of shared globals in this part of the unit */
#define UiRows ((UiElem24 *)UiRows)

#define AddTilePrim ((s32 (*)())AddTilePrim)

#define AddSpriteWithDrawMode ((s32 (*)())AddSpriteWithDrawMode)

#undef UiRows

#undef AddTilePrim

#undef AddSpriteWithDrawMode

typedef struct {
    short x, y;
    unsigned short page, clut;
    unsigned char u, v, w, h;
} SpriteDesc;

typedef struct {
    signed char f00;
    signed char f01;
    signed char f02;
    unsigned char f03;
    char f04;
    unsigned char f05;
    unsigned char f06;
    unsigned char f07;
    short x;
    short y;
    short w;
    short h;
    unsigned short page;
    unsigned short clut;
    unsigned char u;
    unsigned char v;
    short pad16;
    int f18;
} MenuRow1C;

extern int AddDrawModePrim();

extern void *AddTilePrim();

extern void *AddSpriteWithDrawMode();

extern void *func_8002018C();

extern void *func_80020508();

extern void InitRect16Row();

extern int func_8003DCEC();

extern void func_80043608();

extern int D_800ACC14;

extern MenuRow1C *GlyphRows;

int func_8003E13C(int arg0) {
    SpriteDesc sp30;
    SpriteDesc sp40;
    MenuRow1C *base;
    MenuRow1C *p;
    int s7;
    unsigned int s5;
    int v0;
    void *a1;

    base = GlyphRows;
    p = (MenuRow1C *)((arg0 * 0x1C) + (int)base);
    sp40.page = sp30.page = p->page;
    sp40.clut = sp30.clut = p->clut;
    sp40.x = sp30.x = p->x;
    sp40.y = sp30.y = p->y;
    sp30.y += p->h / 2 - 10;
    sp40.u = sp30.u = p->u;
    sp40.w = sp30.w = p->w;
    sp30.v = p->v + p->h;
    sp30.h = 0x10;
    sp40.v = p->v;
    sp40.h = p->h;

    s7 = p->f02;
    s5 = p->f03;

    switch (s7) {
    case -2:
        GlyphRows[arg0].f03 = 0;
        GlyphRows[arg0].f02 = -4;
        GlyphRows[arg0].f00 = 0;
        break;
    case -3:
        break;
    case -4:
        InitRect16Row(GlyphRows[arg0].f07, GlyphRows[arg0].x, GlyphRows[arg0].y, GlyphRows[arg0].w, GlyphRows[arg0].h, 0, 0);
        GlyphRows[arg0].f02 = -5;
        break;
    case -5:
        if (func_8003DCEC(GlyphRows[arg0].f07) != 0) {
            GlyphRows[arg0].f02 = -1;
        }
        break;
    case -6:
        GlyphRows[arg0].f03 = 0;
        GlyphRows[arg0].f02 = -7;
    case -7:
        func_8003DCEC(GlyphRows[arg0].f07);
        if (GlyphRows[arg0].f03 >= 0x14) {
            GlyphRows[arg0].f02 = -1;
            GlyphRows[arg0].f03 = 0;
        } else {
            GlyphRows[arg0].f03++;
        }
        break;
    case -1:
        a1 = *(void **)0x1F800000;
        v0 = ((unsigned short)s5 * 0x7C) / 10;
        *(void **)0x1F800000 = (void *)AddDrawModePrim(
            D_800ACC14 + 0x74,
            func_80020508(D_800ACC14 + 0x74, a1, GlyphRows[arg0].x, GlyphRows[arg0].y, GlyphRows[arg0].w, GlyphRows[arg0].h, v0, v0, v0),
            0x45
        );
        if ((unsigned short)s5 >= 10) {
            GlyphRows[arg0].f02 = 0;
            GlyphRows[arg0].f03 = 0;
        } else {
            GlyphRows[arg0].f03++;
        }
        func_8003DCEC(GlyphRows[arg0].f07);
        break;
    case 0:
        a1 = *(void **)0x1F800000;
        if ((unsigned short)s5 >= 3) {
            a1 = func_8002018C(D_800ACC14 + 0x70, a1, &sp30, 1, 0xFF, 3);
        }
        *(void **)0x1F800000 = AddTilePrim(
            D_800ACC14 + 0x74,
            a1,
            GlyphRows[arg0].x,
            GlyphRows[arg0].y,
            GlyphRows[arg0].w,
            GlyphRows[arg0].h,
            8, 8, 8
        );
        if ((unsigned short)s5 >= 0x15) {
            GlyphRows[arg0].f02 = 1;
            GlyphRows[arg0].f03 = 0;
        } else {
            GlyphRows[arg0].f03++;
        }
        if (s5 == 1) {
            func_80043608(GlyphRows[arg0].f18 + ((GlyphRows[arg0].f06 * GlyphRows[arg0].f00) << 11));
        }
        break;
    case 1:
        a1 = func_8002018C(D_800ACC14 + 0x70, *(void **)0x1F800000, &sp30, 1, (0x80 - s5) * 2, 2);
        a1 = func_8002018C(D_800ACC14 + 0x70, a1, &sp40, 1, s5, 1);
        *(void **)0x1F800000 = AddTilePrim(
            D_800ACC14 + 0x7C,
            a1,
            GlyphRows[arg0].x,
            GlyphRows[arg0].y,
            GlyphRows[arg0].w,
            GlyphRows[arg0].h,
            8, 8, 8
        );
        if (s5 >= 0x80) {
            GlyphRows[arg0].f03 = 0;
            GlyphRows[arg0].f02 = 2;
        } else {
            GlyphRows[arg0].f03 += 8;
        }
        break;
    case 2:
    default:
        a1 = *(void **)0x1F800000;
        if (GlyphRows[arg0].f00 >= 0) {
            a1 = AddSpriteWithDrawMode(
                D_800ACC14 + 0x70,
                a1,
                GlyphRows[arg0].x,
                GlyphRows[arg0].y,
                GlyphRows[arg0].w,
                GlyphRows[arg0].h,
                GlyphRows[arg0].u,
                GlyphRows[arg0].v,
                GlyphRows[arg0].page,
                GlyphRows[arg0].clut,
                0,
                0x80
            );
        }
        *(void **)0x1F800000 = AddTilePrim(
            D_800ACC14 + 0x74,
            a1,
            GlyphRows[arg0].x,
            GlyphRows[arg0].y,
            GlyphRows[arg0].w,
            GlyphRows[arg0].h,
            8, 8, 8
        );
        if (s5 >= GlyphRows[arg0].f05 && GlyphRows[arg0].f01 >= 2) {
            GlyphRows[arg0].f03 = 0;
            GlyphRows[arg0].f02 = 0;
            GlyphRows[arg0].f00 = ++GlyphRows[arg0].f00 % GlyphRows[arg0].f01;
        } else {
            GlyphRows[arg0].f03++;
        }
        break;
    }

    return s7 >= 0;
}
