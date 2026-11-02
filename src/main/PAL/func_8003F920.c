extern int D_800ACC14;
extern int func_80022694();
extern int func_8001FD20();
int func_8003F920(a0, a1, a2, a3, a4, a5, a6) int a0; int a1; int a2; int a3; int a4; int a5; int a6;
{
    int *px;
    int *py;
    int t;
    int k;
    int j;
    int x[4];
    int g;
    int y[4];
    int i;

    x[0] = x[3] = a0;
    x[1] = x[2] = a0 + a2;
    y[0] = y[1] = a1;
    y[2] = y[3] = a1 + a3 - 2;
    i = 0;
    do {
        t = D_800ACC14 + 0x70;
        px = x + i;
        py = y + i;
        j = i + 1;
        k = j;
        if (k < 0) k = i + 4;
        k >>= 2;
        k <<= 2;
        k = j - k;
        func_80022694(t, *px, *py, x[k], y[k], 0x100, 1, 0xFF, 0xFF, 0xFF, -1);
        i++;
    } while (i < 4);
    x[0] = x[1] - x[3];
    y[0] = y[3] - y[1];
    g = *(int *)0x1F800000;
    *(int *)0x1F800000 = func_8001FD20(D_800ACC14 + 0x70, g, a0, a1, x[0], y[0], a4, a5, a6);
}
