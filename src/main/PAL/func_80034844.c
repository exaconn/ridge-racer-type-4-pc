extern int func_800347BC();
extern int func_8004F24C();
extern unsigned char D_8009F6DC[];
extern signed char D_800F2FDC;
extern unsigned short D_800F3A40;
void func_80034844(int a0) {
    if (D_800F3A40 & 0x1000) {
        if (D_800F2FDC > 0) {
            D_800F2FDC = D_800F2FDC - 1;
            func_8004F24C(0);
        }
    }
    if (D_800F3A40 & 0x4000) {
        if (D_800F2FDC < D_8009F6DC[a0]) {
            D_800F2FDC = D_800F2FDC + 1;
            func_8004F24C(0);
        }
    }
    func_800347BC();
}
