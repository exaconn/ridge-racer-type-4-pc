/* CC1_FLAGS: -fno-cse-skip-blocks */
extern int func_80052800();
extern int func_800528A8();
extern int func_80096464();
extern signed char D_800AD4D8[];
extern int D_800A0C9C[];
extern short D_800F3914;
extern int D_800F3970;
extern short D_800F4D44;
extern int D_800F765C;
extern int D_800F7660;
void func_80059168() {
    int a0, a1, v;
    func_80052800();
    if (D_800F4D44 == 1) {
        a0 = D_800AD4D8[D_800AD4D8[0x17] + 8] < 3;
    } else {
        a0 = 1;
    }
    if (D_800F3970 != 0 && D_800F4D44 != 1) {
        D_800F7660 = 0x10;
    } else {
        D_800F7660 = 0xF;
    }
    if (a0 == 0) {
        a1 = D_800F3914 + 3;
        func_800528A8(0, a1);
    } else if (D_800F765C == 0) {
        a1 = func_80096464() % 14 + 0xB;
        func_800528A8(0, a1);
    } else {
        a1 = D_800A0C9C[D_800F765C - 1];
        func_800528A8(0, a1);
    }
}
