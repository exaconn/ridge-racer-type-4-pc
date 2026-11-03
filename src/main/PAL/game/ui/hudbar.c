struct P { unsigned w0; char b4, b5, b6, b7; short x, y, w, h; };
struct O { char pad[0xB70]; unsigned t; };
extern struct O *D_800ACC14;

void func_80022390(int a0)
{
    struct P *p;
    struct O *o;
    p = *(struct P **)0x1F800000;
    ((char *)p)[3] = 3;
    p->b7 = 0x60;
    if (a0 == 0) {
        p->y = 0x7E;
        p->w = 0x140;
        p->x = 0;
        p->h = 4;
    } else {
        p->x = 0x9E;
        p->w = 4;
        p->y = 0;
        p->h = 0x100;
    }
    p->b4 = 8;
    p->b5 = 8;
    p->b6 = 8;
    o = D_800ACC14;
    p->w0 = (p->w0 & 0xFF000000) | (o->t & 0xFFFFFF);
    *(volatile unsigned *)&o->t = (o->t & 0xFF000000) | ((unsigned)p++ & 0xFFFFFF);
    *(struct P **)0x1F800000 = p;
}
