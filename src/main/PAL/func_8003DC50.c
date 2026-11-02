extern int func_80096434();
extern int func_8001E84C();
extern int D_800AC540, D_800ACC00, D_80010874[];

int func_8003DC50(int a0) {
    int n = D_800AC540 << 4;
    D_800ACC00 = a0;
    func_80096434(a0, n);
    func_8001E84C(D_80010874, n);
    return n;
}
