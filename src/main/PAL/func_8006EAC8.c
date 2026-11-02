extern int func_80090B14();
extern int func_80091144();
extern int func_80090F74();
extern int func_800910B4();
struct S8 { int w[8]; };
extern struct S8 D_800FF690, D_800F39A8;

void func_8006EAC8(void) {
    int v[4], w[4], x[4];
    int *h = (int *)0x1F800000;
    int *g = (int *)0x1F800008;
    v[0] = 0x8000 - (g[0] >> 2);
    v[1] = -(g[1] >> 2);
    v[2] = 0x8000 - (g[2] >> 2);
    func_80090B14(0x1F800028, v, w);
    D_800FF690.w[5] = w[0];
    D_800FF690.w[6] = w[1];
    D_800FF690.w[7] = w[2];
    D_800F39A8 = D_800FF690;
    v[0] = 0x20000 - g[0];
    v[1] = -g[1];
    v[2] = 0x20000 - g[2];
    D_800F39A8.w[5] >>= 1;
    D_800F39A8.w[6] >>= 1;
    D_800F39A8.w[7] >>= 1;
    func_80090B14(0x1F800028, v, w);
    h[15] = w[0];
    h[16] = w[1];
    h[17] = w[2];
    func_80091144(0x1F800028);
    x[2] = 0x4000;
    x[1] = 0x4000;
    x[0] = 0x4000;
    func_80090F74(0x1F800028, x);
    func_800910B4(0x1F800028);
}
