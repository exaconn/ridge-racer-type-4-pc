extern unsigned char func_80043080();
extern int func_800430BC();
extern int func_800430D8();
extern unsigned char func_80043104();
extern unsigned char D_8009FE2C[];
int func_8004326C(unsigned char *a0, unsigned char *a1) {
    unsigned char *p = a1;
    unsigned char *q;
    unsigned char *r;
    int i;
    int k;
    int c;
    unsigned v;
    unsigned char *tab;
    c = func_800430BC(p[0], p[1]);
    i = 0;
    q = a0;
L1:
    {
        unsigned char t0 = p[2];
        unsigned char t1 = p[3];
        p += 2;
        i++;
        *q = func_800430BC(t0, t1);
        q++;
    }
    if (i < 6) goto L1;
    q = a0;
    i = 0;
    k = ((unsigned char)c % 11) & 0xFF;
    tab = D_8009FE2C;
    do {
        unsigned char *t;
        r = (unsigned char *)(k + (int)tab);
        t = q + i;
        k++;
        *t = func_80043080(*t, *r, 1);
        if (k >= 11) k = 0;
        i++;
    } while (i < 6);
    a0[5] = a0[5] & 0xF;
    if ((unsigned char)c != func_80043104(a0, 6)) return -1;
    func_800430D8(a0, 6);
    v = (((a0[4] & 0xF) << 16) + (a0[3] << 8) + a0[2]);
    v >>= 2;
    if (v > 0x927BF) return -1;
    v = (a0[1] << 4) + (a0[0] >> 4);
    v &= 0x3FF;
    if (v >= 0x280) return -1;
    v = a0[5] & 0xF;
    if (v >= 6) return -1;
    return 0;
}
