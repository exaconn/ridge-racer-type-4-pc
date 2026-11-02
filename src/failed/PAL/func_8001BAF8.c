typedef struct { int a, b, c; } V;
extern V D_8009E5E0[];
void func_8001BAF8(a0, a1) int a0; int a1; {
    int *g;
    int v;
    int c = a0;
    int idx = a1;
    int *p;
    V *t;
    if (c == 0) return;
    if (c >= 0) {
        v = 0xFF;
        if (c < 0x100) goto L;
    } else {
L:
        v = c;
        if (c < 0) v = 0;
    }
    c = v;
    g = (int *)0x1F800000;
    p = (int *)g[0];
    p[1] = (c << 16) | ((c << 8) | 0x66000000) | c;
    t = D_8009E5E0 + idx;
    p[2] = t->a;
    p[4] = t->b;
    p[3] = t->c;
    p[0] = ((int *)g[1])[1];
    ((char *)p)[3] = 4;
    ((int *)g[1])[1] = (int)p;
    ((char *)g[1])[7] = 0;
    g[0] = (int)(p + 5);
}
