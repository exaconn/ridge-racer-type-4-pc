extern int func_80034E84();
extern int func_80034F18();
extern int func_800356A4();
extern int func_80092CB8();
extern int func_8009AEAC();
extern char D_800ABEBE[], D_800AC648[], D_800ACBD8[], D_800F3914[], D_800F3A30[], D_800F4DA0[], D_8010C0A0[];
extern short D_800AD50A, D_800F3A26, D_800F4BE8, D_800F4C6A, D_800F4D8A, D_800FF688, D_800FFC10;

void func_8003597C(void) {
    int s = D_800FF688;
    if (s == 1) goto st1;
    if (s < 2) {
        if (s == 0) goto st0;
        goto other;
    }
    if (s == 2) goto st2;
    goto other;
st0:
    func_80092CB8(0);
    func_80034E84();
    func_8009AEAC(3, 0, 0);
    func_8009AEAC(5, 0, 0);
    func_8009AEAC(3, 0, 0);
    D_800FF688 = 1;
    return;
st1:
    if (func_800356A4(D_800ABEBE, D_800ABEBE - 2, 2, 1) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_8010C0A0, D_8010C0A0 - 0x18, 0x18, 1) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_800F4DA0, D_800F4DA0 - 8, 8, 1) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_800ACBD8, D_800ACBD8 - 8, 8, 1) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_800F3914, D_800F3914, 2, 0) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_800AC648, D_800AC648, 2, 0) == -1) { D_800FF688 = 3; return; }
    if (func_800356A4(D_800F3A30, D_800F3A30, 4, 0) == -1) { D_800FF688 = 3; return; }
    D_800FF688 = 2;
    func_80034F18();
    func_80092CB8(1);
    return;
st2:
    func_8009AEAC(3, 0, 0);
    D_800FFC10 = 0;
    D_800F4BE8 = 0xA;
    D_800AD50A = 0x11A;
    D_800F4C6A = 1;
    D_800F3A26 = 0x23;
    D_800F4D8A = 0;
    return;
other:
    func_80034F18();
    func_80092CB8(1);
    D_800F4C6A = 1;
    D_800F3A26 = 0x2C;
    D_800F4D8A = 0;
}
