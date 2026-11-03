#include "rr/badge_rows.h"
struct P { int link; unsigned char r, g, b, code; short x, y; unsigned char u, v; unsigned short clut; short w, h; };
extern struct P *func_8001FCC8();
extern int func_80095B94(), func_80095B24(), func_80095AF4(), func_80095A44();

void *func_80020074(u32 *a0, void *a1, int a2, int a3, int a4, int a5, int a6, int a7, unsigned short a8, unsigned short a9, unsigned short a10, unsigned short a11)
{
    struct P *p = func_8001FCC8(a0, a1, 5);
    func_80095B94(p);
    p->x = a2;
    p->y = a3;
    p->w = a4;
    p->h = a5;
    p->u = a6;
    p->v = a7;
    func_80095AF4(p, a10);
    if (a11 != 0x80) {
        func_80095B24(p, 0);
        p->r = a11;
        p->g = a11;
        p->b = a11;
    } else {
        func_80095B24(p, 1);
    }
    p->clut = a9;
    func_80095A44(a0, p++);
    return func_8001FCC8(a0, p, a8);
}
