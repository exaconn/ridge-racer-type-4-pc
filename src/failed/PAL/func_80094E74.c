/* GCC_VERSION: 2.7.2 */
extern int func_8008B3C4(), func_8008B4E0();
extern int D_800ABAC8, D_800ABACC, D_800ABBF4;
extern volatile int D_800ABBE8, D_800ABBEC;
extern volatile unsigned *D_800ABBC8, *D_800ABBD4;
struct E { void (*f)(); int a, b; int pad[21]; };
extern struct E D_801128D8[];
extern int D_800ABAC0[];
int func_80094E74() {
    int r;
    r = 1;
    if ((*D_800ABBD4 & 0x1000000) == 0) {
        D_800ABBF4 = func_8008B4E0(0);
        if (D_800ABBE8 != D_800ABBEC && (*D_800ABBD4 & 0x1000000) == 0) {
            do {
                if (((D_800ABBEC + 1) & 0x3F) == D_800ABBE8 && D_800ABACC == 0) func_8008B3C4(2, 0);
                if ((*D_800ABBC8 & 0x4000000) == 0) { while ((*D_800ABBC8 & 0x4000000) == 0) ; }
                D_801128D8[D_800ABBEC].f(D_801128D8[D_800ABBEC].a, D_801128D8[D_800ABBEC].b);
                D_800ABBEC = (D_800ABBEC + 1) & 0x3F;
            } while (D_800ABBE8 != D_800ABBEC && (*D_800ABBD4 & 0x1000000) == 0);
        }
        func_8008B4E0(D_800ABBF4);
        if (D_800ABBE8 == D_800ABBEC && (*D_800ABBD4 & 0x1000000) == 0) {
            if (D_800ABAC8 != 0 && D_800ABAC0[3] != 0) {
                D_800ABAC0[2] = 0;
                ((void (*)())D_800ABAC0[3])();
            }
        }
        r = (D_800ABBE8 - D_800ABBEC) & 0x3F;
    }
    return r;
}
