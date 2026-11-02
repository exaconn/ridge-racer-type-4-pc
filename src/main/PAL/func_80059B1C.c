extern char *D_800ACC14;

void func_80059B1C(unsigned char *a0x, int a1, int a2, int a3) {
    register unsigned char *p asm("$10");
    int *g;
    unsigned hi;
    unsigned cmd;
    register unsigned char *q asm("$2");
    int t;
    unsigned t3;
    register unsigned lo asm("$5");
    register int v1 asm("$3");
    register char *d asm("$4");
    p = *(unsigned char **)0x1F800000;
    p[3] = 4;
    p[7] = 0x64;
    if (a3 < 0xFF) {
        p[7] = 0x66;
    } else {
        p[7] = 0x65;
    }
    v1 = a1;
    lo = 0xFFFFFF;
    cmd = 0xE100023E;
    p[4] = a3;
    p[5] = a3;
    p[6] = a3;
    *(short *)(p + 8) = v1;
    *(short *)(p + 0xA) = a2;
    *(short *)(p + 0x10) = a0x[2];
    *(short *)(p + 0x12) = a0x[3];
    p[0xC] = a0x[0];
    t = a0x[1];
    *(short *)(p + 0xE) = 0x7F78;
    p[0xD] = t;
    hi = 0xFF000000;
    g = (int *)0x1F800000;
    d = D_800ACC14;
    *(unsigned int *)p = (*(unsigned int *)p & hi) | (*(unsigned int *)(d + 0x70) & lo);
    *(unsigned int *)(d + 0x70) = (*(unsigned int *)(d + 0x70) & hi) | ((unsigned int)p & lo);
    p += 0x14;
    *g = (int)p;
    p[3] = 1;
    d = D_800ACC14;
    t3 = *(unsigned int *)p;
    *(unsigned int *)(p + 4) = cmd;
    *(unsigned int *)p = (t3 & hi) | (*(unsigned int *)(d + 0x70) & lo);
    *(unsigned int *)(d + 0x70) = (*(unsigned int *)(d + 0x70) & hi) | ((unsigned int)p & lo);
    q = p + 8;
    *g = (int)q;
}
