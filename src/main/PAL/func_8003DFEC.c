extern int func_80096434();
extern int func_8001E84C();
extern int D_800F4C1C;
extern int D_800F4D8C;
extern char D_800108D8[];

int func_8003DFEC(int a) {
    int sz = D_800F4C1C * 28;
    D_800F4D8C = a;
    func_80096434(a, sz);
    func_8001E84C(D_800108D8, sz);
    return sz;
}
