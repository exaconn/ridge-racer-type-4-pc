extern short D_800F3A26, D_800F4C6A, D_800F4D8A;
extern unsigned short D_800F3A40, D_800F3A42;
extern int D_8010BFC0;
void func_8003244C(void) {
    if (D_800F3A42 & 0x1000) {
        D_8010BFC0 = D_8010BFC0 - 1;
    } else if (D_800F3A42 & 0x4000) {
        D_8010BFC0 = D_8010BFC0 + 1;
    }
    if (D_8010BFC0 < 0) {
        D_8010BFC0 = 1;
    } else if (D_8010BFC0 >= 2) {
        D_8010BFC0 = 0;
    }
    if (D_800F3A40 & 0x800) {
        D_800F4C6A = 1;
        D_800F3A26 = 0xF;
        D_800F4D8A = 0;
    }
}
