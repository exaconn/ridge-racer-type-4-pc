#include "types.h"
struct R { unsigned char u, v, w, h; };
struct P { int link; unsigned char r, g, b, code; short x, y; unsigned char u, v; unsigned short clut; short w, h; };
void SetSprt(void*);void SetShadeTex(void*,s32);void SetSemiTrans(void*,s32);
void AddPrim(u32 *,u32 *);

struct P *func_8001FF84(u32 *a0, struct P *a1, int a2, int a3, struct R *a4, unsigned short a5, int a6)
{
    int s1 = a6 >> 1;
    SetSprt(a1);
    if (s1 == 0) {
        SetShadeTex(a1, 1);
    } else {
        a1->r = s1;
        a1->g = s1;
        a1->b = s1;
    }
    SetSemiTrans(a1, a6 & 1);
    a1->x = a2;
    a1->y = a3;
    a1->w = a4->w;
    a1->h = a4->h;
    a1->u = a4->u;
    a1->v = a4->v;
    a1->clut = a5;
    AddPrim(a0, (u32 *)a1++);
    return a1;
}
