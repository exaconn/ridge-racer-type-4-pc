extern int func_8004326C();
extern void func_80096424();
extern unsigned char D_8010C168[];
extern char D_8010C158[];

int func_80043508(int *a, int *b, int *c, int *d, int *e, int f) {
    unsigned t;
    func_80096424(D_8010C158, f);
    if (func_8004326C(D_8010C168, D_8010C158) == 0) {
        *a = D_8010C168[0] & 0xF;
        *b = (D_8010C168[1] * 16 + (D_8010C168[0] >> 4)) & 0x3FF;
        t = (D_8010C168[4] << 16) + (D_8010C168[3] << 8) + D_8010C168[2];
        *c = (t >> 2) & 0xFFFFF;
        *d = t & 3;
        *e = D_8010C168[5] & 0xF;
        return 0;
    }
    return -1;
}
