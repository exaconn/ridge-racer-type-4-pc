extern int func_8009AEAC();
extern short D_800F3A26, D_800F4C6A, D_800F4D8A, D_800FF688;
int func_80034FC0(void) {
    if (D_800FF688 == -1) {
        D_800F4C6A = 1;
        D_800F3A26 = 0x2D;
        D_800F4D8A = 0;
        func_8009AEAC(1, 1, 0);
        return 1;
    }
    return 0;
}
