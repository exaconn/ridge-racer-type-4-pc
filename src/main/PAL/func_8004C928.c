extern int D_800ACC14;

void func_8004C928(int a0) {
    register int *g asm("$8") = (int *)0x1F800000;
    int lo;
    int *p = (int *)g[0];
    int *q;
    int t;
    ((char *)p)[3] = 1;
    p[1] = 0xE100023E;
    q = (int *)(a0 * 4 + D_800ACC14);
    lo = 0xFFFFFF;
    t = q[0x70/4];
    p[0] = (p[0] & 0xFF000000) | (t & lo);
    q[0x70/4] = (q[0x70/4] & 0xFF000000) | ((int)p & lo);
    g[0] = (int)(p + 2);
}
