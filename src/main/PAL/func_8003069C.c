extern int func_80020074();
extern unsigned char D_8009F074[];
extern signed char D_8009F108[];
int func_8003069C(a, p, x, y, w, z) int a; int p; int x; int y; int w; int z; {
    int t0 = x;
    int s1 = y;
    int i = 0;
    do {
        signed char *q = &D_8009F108[i + a * 8];
        int v = *q;
        unsigned char *e;
        int s0, d, c0, c1, c3;
        i++;
        if (v == -1) break;
        e = D_8009F074 + v * 4;
        d = q[4];
        c0 = e[0]; c1 = e[1]; s0 = e[2]; c3 = e[3];
        s1 += d;
        t0 = func_80020074(p, t0, s1, w, s0, c3, c0, c1, 0x4A, 0x2E28, 1, z);
        s1 += s0;
    } while (i < 4);
    return t0;
}
