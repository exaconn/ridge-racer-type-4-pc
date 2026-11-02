extern int func_8006E300();
extern int func_80090B14();
extern int func_80090F74();
extern int func_8006E9D4();
extern int D_8010E690;

void func_8006E9E8(int a0, int *a1) {
    int v[4], w[4], x[4];
    int *g = (int *)0x1F80007C;
    if (a0) {
        func_8006E300();
        v[0] = (0x8000 - a1[0]) << 2;
        v[1] = (0x60 - a1[1]) << 2;
        v[2] = (0x8000 - a1[2]) << 2;
        func_80090B14(0x1F800108, v, w);
        x[2] = 0x4000;
        x[1] = 0x4000;
        x[0] = 0x4000;
        g[0x28] = w[0];
        g[0x29] = w[1];
        g[0x2A] = w[2];
        func_80090F74(0x1F800108, x);
        func_8006E9D4(D_8010E690, a0, a1);
    }
}
