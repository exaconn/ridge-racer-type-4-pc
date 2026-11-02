struct P { unsigned addr:24; unsigned len:8; unsigned char r, g, b, code; short x; unsigned short y; unsigned char u, v; unsigned short clut; short w, h; };
extern int func_8001FCC8();
extern int func_80095B24(), func_80095AF4();

void func_80020F20(a0, a1, a2, a3)
struct P *a0; int a1; int a2; unsigned char *a3;
{
    struct P *p = *(struct P **)0x1F800000;
    int c, u, kh;
    if (*a3 != 0) {
        kh = 0x10;
        do {
            c = *a3;
            a3++;
            p->len = 4;
            p->code = 0x64;
            p->x = (short)a1;
            p->y = (unsigned short)a2;
            c -= 0x30;
            u = c * 8;
            if (c < 10) {
                a1 += 5;
                p->w = 8;
                p->h = kh;
            } else {
                a1 += 3;
                p->w = 4;
                p->h = kh;
            }
            func_80095B24(p, 1);
            func_80095AF4(p, 1);
            { int v58 = 0x58; p->u = u; p->v = v58; p->clut = 0x7FA4; }
            p->addr = a0->addr;
            a0->addr = (unsigned)p;
            p++;
        } while (*a3 != 0);
    }
    *(int *)0x1F800000 = func_8001FCC8(a0, p, 0x59);
}
