extern int func_80096434();
extern int D_800F3A28;

int func_80041A18(int a, int n) {
    int sz = n * 56;
    D_800F3A28 = a;
    func_80096434(a, sz);
    return sz;
}
