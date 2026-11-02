extern int func_80096EA4();
extern int D_8010ECC0[];
extern int D_8010EF58, D_8010EF5C, D_8010EF60, D_8010EF64;

int func_800780B8(int arg) {
    int r;
    int t;
    r = func_80096EA4(D_8010EF58) == 1;
    if (func_80096EA4(D_8010EF5C) == 1) r = 2;
    if (func_80096EA4(D_8010EF60) == 1) r = 3;
    if (func_80096EA4(D_8010EF64) == 1) r = 4;
    t = D_8010ECC0[arg]++;
    if (t >= 0x78) r = 2;
    return r;
}
