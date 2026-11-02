extern int func_8004F720();
extern int func_800590DC();
extern int func_800207D4();
extern int func_800293D0();
extern int func_80077D90();
extern int func_80014A8C();
extern int D_800ABEF8[];
extern short D_800AC5D0;
extern int D_800ACC14;
extern int D_800ADAC8;
extern short D_800F3A26;
extern short D_800F4C6A;
extern unsigned short D_800F4D44;
extern short D_800F4D8A;
extern int D_800FF6B0;
extern int D_8010BFB0;
void func_80029F34() {
    func_8004F720();
    func_800590DC();
    func_800207D4(D_800ACC14 + 0x70, 0xFF, 0x25);
    func_800293D0(0, 2);
    D_800FF6B0 = D_800ADAC8;
    func_80077D90(1);
    func_80014A8C(10);
    D_8010BFB0 = 0;
    if ((unsigned)(D_800F4D44 - 2) < 2U) {
        if (D_800ABEF8[0x184 / 4] < D_800ABEF8[0x4A4 / 4]) D_8010BFB0 = 1;
    }
    D_800AC5D0 = -1;
    D_800F4C6A = 1;
    D_800F3A26 = 0x16;
    D_800F4D8A = 0;
}
