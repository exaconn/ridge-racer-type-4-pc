extern int func_80096434();
extern int D_800ACC24;
extern int D_800F2AB0;

int func_8003FAA0(int a) {
    int sz = D_800ACC24 * 28;
    D_800F2AB0 = a;
    func_80096434(a, sz);
    return sz;
}
