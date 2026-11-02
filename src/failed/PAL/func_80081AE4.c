/* GCC_VERSION: 2.7.2 */
typedef struct { unsigned char *c0; int c4, c8, cC; char p10[4]; unsigned char b14, b15, b16; unsigned char b17, b18, b19, b1a, b1b, b1c, b1d, b1e, b1f, b20, b21; signed char b22[2]; char p24[2]; signed char b26; unsigned char b27[0x10]; unsigned char b37[0x10]; char p47[1]; short h48[8]; unsigned short h58[4]; unsigned short h60[0x10]; short h80; char p82[6]; int w88; int w8c; int w90; char p94[4]; int w98; int w9c; int wa0, wa4, wa8, wac; char pb0[0]; } Ch;
extern Ch *D_8010F068[];
extern unsigned int D_8010F064;
extern int func_80081E04(short, short, unsigned, unsigned);
void func_80081AE4(short a0, short a1) {
    int t = a0 << 16;
    Ch **b = D_8010F068;
    Ch **q = (Ch **)((int)b + (t >> 14));
    Ch *c = (Ch *)((char *)*q + a1 * 0xB0);
    unsigned int a2, a3, v0, lo, m;
    int t0;
    register int tt asm("$3");
    unsigned char *p;
    int b0, b1, b2;
    p = c->c0;
    b0 = *p++;
    c->c0 = p;
    b1 = *p;
    c->c0 = p + 1;
    b2 = p[1];
    lo = 0x03938700 / (b2 | ((b0 << 16) | (b1 << 8)));
    m = c->h48[4] * lo;
    a2 = D_8010F064;
    c->c0 = p + 2;
    *(volatile int *)((char *)c + 0x94) = lo;
    tt = a2 << 4;
    t0 = tt - a2;
    a3 = a2 * 0x3C;
    if (m * 0xA < a3) {
        v0 = (a2 * 0x258) / m;
        c->h48[5] = v0;
        c->h48[6] = v0;
    } else {
        a2 = (unsigned)(c->h48[4] * *(int *)((char *)c + 0x94) * 0xA) / a3;
        c->h48[5] = -1;
        c->h48[6] = a2;
        if ((unsigned)(t0 * 2) < (unsigned)(c->h48[4] * *(int *)((char *)c + 0x94) * 0xA) % a3)
            c->h48[6] = a2 + 1;
    }
    c->w90 = func_80081E04(a0, a1, a2, a3);
}
