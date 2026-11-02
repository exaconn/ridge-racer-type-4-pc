extern int func_8009AEAC();
extern int func_80096F24();
extern int D_800105C8[], D_800F2AAC, D_800F2AA8;
extern char func_80034E70[];

void func_80034E84(void) {
    func_8009AEAC(1, 3, 0x7E900);
    func_8009AEAC(1, 4, 1);
    D_800F2AAC = func_80096F24(D_800105C8, 2);
    D_800F2AA8 = func_80096F24(D_800105C8, 1);
    func_8009AEAC(4, 0, (int)func_80034E70);
    func_8009AEAC(2, 0, 0);
}
