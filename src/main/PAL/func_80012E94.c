extern int func_80095C14();
extern unsigned char D_8009DFD8[];
extern unsigned char D_8009DFEC[];

int func_80012E94(int a0, int a1, int a2, int a3, int a4)
{
    int b1 = a1;
    int xx;
    /* ASM_BARRIER: pins a<<4 temp to a3 ($7) to match reg choice */
    register int b16 asm("$7");
    int t;
    int c;
    int a, b, x;
    char *s0, *p0, *p1;
    char * volatile *g;
    /* ASM_BARRIER: keeps 0x1F80007C pointer in a1 ($5) */
    register short *sp asm("$5");

    sp = (short *)0x1F80007C;
    if (a0 == 2 && b1 == 0 && a2 == 0x13)
        return 0x13;
    t = D_8009DFEC[b1 * 16 + a3];
    /* ASM_BARRIER: forces copy t->a (original addu a0,v1,zero) */
    __asm__("" : "=r"(a) : "0"(t));
    xx = (t & 7) * 0x10 + 0x340; x = xx;
    if (t < 0) a = t + 7;
    a >>= 3;
    c = D_8009DFD8[a2];
    b16 = a << 4; b = b16 + (a << 2);
    if (c >= 4) {
        if (c >= 7)
            b += 0x78;
        else
            b += 0x3C;
    }
    g = (char * *)0x1F800000;
    s0 = g[0];
    sp[0] = xx;
    sp[1] = b;
    *(int *)(sp + 2) = 0x140010;
    func_80095C14(s0, sp, (a4 << 6) + 0x1B0, 0xD8);
    *(int *)s0 = *(int *)g[1];
    s0[3] = 5;
    *(int *)g[1] = (int)s0;
    g[1][3] = 0;
    s0 += 0x18;
    g[0] = s0;
    *(int *)(s0 + 4) = 0xE6000000;
    p1 = g[1]; p0 = g[0]; *(int *)p0 = *(int *)p1;
    g[0][3] = 1;
    *(int *)g[1] = (int)g[0];
    g[1][3] = 0;
    g[0] += 8;
    /* ASM_BARRIER: keeps li v0,0 after final sw (stops sched hoisting it) */
    { int r; __asm__ volatile("" : "=r"(r) : "0"(0)); return r; }
}
