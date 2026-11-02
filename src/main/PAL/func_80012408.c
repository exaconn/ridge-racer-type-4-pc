extern int func_80090424();
extern int func_800904F4();
extern int D_8009DFA0, D_8009DFA4;
extern struct P { int w0, w4; short x0, y0; int c1; short x1, y1; int c2; short x2, y2; } D_80109400[];
extern struct P *D_80109438;

void func_80012408(void)
{
    int a, v, w;
    unsigned *t;
    unsigned **g;
    register unsigned m asm("$5");
    unsigned h;
    a = D_8009DFA0 + 0x10;
    v = D_8009DFA4 ^ 1;
    D_8009DFA4 = v;
    D_8009DFA0 = a;
    D_80109438 = &D_80109400[v];
    D_80109438->x0 = (func_80090424(a) >> 7) + 0x50;
    D_80109438->y0 = (func_800904F4(D_8009DFA0) >> 7) + 0x80;
    D_80109438->x1 = (func_80090424(D_8009DFA0 + 0x555) >> 7) + 0x50;
    D_80109438->y1 = (func_800904F4(D_8009DFA0 + 0x555) >> 7) + 0x80;
    D_80109438->x2 = (func_80090424(D_8009DFA0 - 0x555) >> 7) + 0x50;
    D_80109438->y2 = (func_800904F4(D_8009DFA0 - 0x555) >> 7) + 0x80;
    w = 0x300000FF;
    m = 0xFFFFFF;
    g = (unsigned **)0x1F800000;
    h = 0xFF000000;
    D_80109438->c1 = 0xFF00;
    D_80109438->c2 = 0xFF0000;
    D_80109438->w4 = w;
    *((unsigned char *)D_80109438 + 3) = 6;
    D_80109438->w0 = (D_80109438->w0 & h) | (**(g + 1) & m);
    t = *(g + 1);
    *t = (*t & h) | ((unsigned)D_80109438 & m);
}
