extern int func_80096434();
extern int func_8001E84C();
extern int D_800F4BEC;
extern int D_800F4D80;
extern char D_800108D8[];

int func_8003E8D4(int a) {
    int sz = D_800F4BEC * 36;
    D_800F4D80 = a;
    func_80096434(a, sz);
    func_8001E84C(D_800108D8, sz);
    return sz;
}
