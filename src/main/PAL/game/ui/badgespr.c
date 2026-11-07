#include "rr/badge_rows.h"
#include "psyq/libgpu/primattr.h"
#include "psyq/libgpu/settrans.h"
#include "rr/animated_glyph.h"
#include "rr/menu_label_render.h"
struct P { int link; unsigned char r, g, b, code; short x, y; unsigned char u, v; unsigned short clut; short w, h; };

void *AddSpriteWithDrawMode(u32 *a0, void *a1, int a2, int a3, int a4, int a5, int a6, int a7, unsigned short a8, unsigned short a9, unsigned short a10, unsigned short a11)
{
    struct P *p = AddDrawModePrim(a0, a1, 5);
    SetSprt(p);
    p->x = a2;
    p->y = a3;
    p->w = a4;
    p->h = a5;
    p->u = a6;
    p->v = a7;
    SetSemiTrans(p, a10);
    if (a11 != 0x80) {
        SetShadeTex(p, 0);
        p->r = a11;
        p->g = a11;
        p->b = a11;
    } else {
        SetShadeTex(p, 1);
    }
    p->clut = a9;
    AddPrim(a0, p++);
    return AddDrawModePrim(a0, p, a8);
}
