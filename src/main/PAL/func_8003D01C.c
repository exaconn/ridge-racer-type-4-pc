extern unsigned short D_800ABED0[];
extern char *D_800F4D90;
extern int func_80091274();
extern int func_80091114();

void func_8003D01C(a0) int a0;
{
    short sp[14];
    short *s0;
    int i = 0;
    char *base;
    unsigned short *a3 = D_800ABED0;
    unsigned char *a1;
    unsigned char *a2;
    base = D_800F4D90;
    a1 = (unsigned char *)(a0 * 0x2C + (int)base);
    a2 = a1;
L:
    {
        short *v;
        s0 = sp;
        v = (short *)((int)s0 + i * 2);
        i++;
        a3[0] = *(unsigned short *)(a2 + 0x10);
        a3[1] = *(unsigned short *)(a2 + 0x12);
        a3[2] = *(unsigned short *)(a2 + 0x14);
        a2 += 8;
        v[0] = a1[4] << 4;
        v[3] = a1[5] << 4;
        v[6] = a1[6] << 4;
        a3 += 3;
        a1 += 4;
    }
    if (i < 3) goto L;
    {
        char *b2 = D_800F4D90;
        unsigned char *q = (unsigned char *)(a0 * 0x2C + (int)b2);
        func_80091274(q[0x28], q[0x29], q[0x2A], a3);
    }
    func_80091114(s0);
}
