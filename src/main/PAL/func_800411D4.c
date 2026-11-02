extern unsigned *func_8001FCC8();
extern char *D_800ACC14;
void func_800411D4()
{
    unsigned *a1;
    int a3;
    unsigned char *q;
    char *d;
    unsigned t;
    a1 = func_8001FCC8(D_800ACC14 + 0x98, *(int *)0x1F800000, 5);
    q = (unsigned char *)a1 + 7;
    a3 = 0;
    do {
        q[a3 * 20 - 4] = 4;
        q[a3 * 20] = 0x64;
        *(short *)(q + a3 * 20 + 9) = 0xC;
        *(short *)(q + a3 * 20 + 0xB) = 0x18;
        q[a3 * 20 + 5] = 0xF4;
        q[a3 * 20 + 6] = 0x68;
        *(short *)(q + a3 * 20 + 7) = 0x2678;
        q[a3 * 20] |= 1;
        *(short *)(q + a3 * 20 + 1) = (a3 % 22) * 13 + 0x11;
        *(short *)(q + a3 * 20 + 3) = (a3 / 22) * 0x1C + 0x60;
        d = D_800ACC14;
        a3++;
        t = (unsigned)a1 & 0xFFFFFF;
        *a1 = (*a1 & 0xFF000000) | (*(unsigned *)(d + 0x98) & 0xFFFFFF);
        *(unsigned *)(d + 0x98) = (*(unsigned *)(d + 0x98) & 0xFF000000) | t;
        a1 += 5;
    } while (a3 < 0xF2);
    *(unsigned **)0x1F800000 = func_8001FCC8(d + 0x98, a1, 0xE, a3);
}
