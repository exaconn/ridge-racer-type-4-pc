typedef struct {
    int f0;
    short h4;
    short pad6;
    int f8;
    int fC;
    int pad10;
    int f14;
    char pad18[0x28 - 0x18];
    short h28;
    short h2A;
    char pad2C[0x5DC - 0x2C];
    short h5DC[(0x620 - 0x5DC) / 2];
} W;
extern W D_800F4DF0[];
extern int func_80096454();
extern int func_800857A4();

void func_800500A8(a0)
int a0;
{
    int s[4];
    W *w = &D_800F4DF0[a0];
    unsigned short *q;
    int i;
    int k;
    int t;
    func_80096454(s, 0, 0x10);
    s[3] = 2;
    k = w->f8;
    i = 0;
    if (w->fC > 0) {
        q = (unsigned short *)((int)s + a0 * 4);
        do {
            func_800857A4((short)k, w->h4, w->h5DC[w->f14 * 5 + i], 0, (short)(*q + 0x3C), 0, 0, 0);
            k++;
            i++;
        } while (i < w->fC);
    }
    t = *(unsigned short *)&s[a0];
    w->h2A = 0;
    w->h28 = t + 0x3C;
}
