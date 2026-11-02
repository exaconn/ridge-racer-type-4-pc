extern int func_8004F2D8();
extern int D_800ABEC0;
extern short D_800AC5D0;
extern int D_800FF6B0;
void func_80034630(int a0) {
    if (D_800FF6B0 != 0 && a0 < 0x115) {
        if (a0 == 0xC3) {
            D_800AC5D0 = 0x1E;
        }
        if (a0 == 0x65) {
            if (!(D_800ABEC0 & 1)) {
                func_8004F2D8(0, 0);
                D_800ABEC0 |= 1;
                return;
            }
        }
        if (a0 == 0x7E) {
            if (!(D_800ABEC0 & 2)) {
                func_8004F2D8(1, 0);
                D_800ABEC0 |= 2;
                return;
            }
        }
        if (a0 == 0x97) {
            func_8004F2D8(2, 0);
            return;
        }
        if (a0 == 0xB0) {
            if (!(D_800ABEC0 & 4)) {
                func_8004F2D8(3, 0);
                D_800ABEC0 |= 4;
                return;
            }
        }
        if (a0 == 0xE2) {
            if (!(D_800ABEC0 & 8)) {
                func_8004F2D8(0x17, 0);
                D_800ABEC0 |= 8;
                return;
            }
        }
        if (a0 == 0x114) {
            if (!(D_800ABEC0 & 0x10)) {
                func_8004F2D8(0x18, 0);
                D_800ABEC0 |= 0x10;
            }
        }
    }
}
