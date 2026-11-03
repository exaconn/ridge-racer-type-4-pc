extern int func_80096434();
extern int func_8001E84C();
extern int D_800F3AFC, D_800F2ECC, D_80010874[];

int func_8003D1D4(int a0) {
    int n = D_800F2ECC * 28;
    D_800F3AFC = a0;
    func_80096434(a0, n);
    func_8001E84C(D_80010874, n);
    return n;
}
