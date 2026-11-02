extern int func_80090EC4();
extern int func_800910B4();
extern int func_80091144();

void func_8006EE3C(short *a0, short *a1, int *a2) {
    short *g = (short *)0x1F800000;
    a0[0] = a1[0] - g[4];
    a0[1] = a1[2] - g[6];
    a0[2] = a1[4] - g[8];
    func_80090EC4(0x1F800028, a0, a0 + 4);
    *(int *)(a0 + 0x16) = *(int *)(a0 + 4);
    *(int *)(a0 + 0x18) = *(int *)(a0 + 6);
    *(int *)(a0 + 0x1A) = *(int *)(a0 + 8);
    func_800910B4(a2);
    func_80091144(a0 + 12);
}
