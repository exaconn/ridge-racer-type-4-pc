extern int func_8001FD20();
extern int func_80036068();
extern int func_80092CB8();
extern int D_800AC5DC, D_800ACC14;
extern short D_800F3A26, D_800F4BE8, D_800F4C6A;
extern unsigned short D_800F4D8A;
void func_80036098(void) {
    if (D_800F4D8A == 0) {
        func_80092CB8(0);
        func_80036068();
        D_800AC5DC = 0x80;
        D_800F4D8A = 1;
    } else if (D_800F4D8A == 1) {
        D_800F4D8A = 2;
    } else if (D_800F4D8A == 2) {
        D_800F4BE8 = 6;
        D_800F4C6A = 1;
        D_800F3A26 = 0x23;
        D_800F4D8A = 0;
    }
    *(int *)0x1F800000 = func_8001FD20(D_800ACC14 + 0x70, *(int *)0x1F800000, 0, 0, 0x140, 0x100, 8, 8, 8);
}
