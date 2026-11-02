extern int func_8001FDD4();
extern int func_8002A604();
extern int func_8002F688();
extern int func_80031E3C();
extern int func_8003A198();
extern int func_8003A1B4();
extern int func_80041A18();
extern int func_80041A54();
extern int func_80096434();
extern int D_800ABEB0;
extern int D_800AC5DC;
extern char *D_800ACC14;
extern int D_800ACC18;
extern short D_800AD4F2;
extern int D_800F2DE4;
extern short D_800F3A26;
extern int *D_800F3A28;
extern short D_800F4C6A;
extern short D_800F4CB0;
extern int D_800F4D40;
extern short D_800F4D8A;
extern char D_800FFC48[];
void func_8003016C(void)
{
    int *sp = (int *)0x1F800000;
    int *p;
    func_80041A18(D_800FFC48, 1);
    func_80041A54(0, 0, 3, 0xF0, 0xA0);
    func_80096434(0x1F800008, 0x20);
    p = D_800F3A28;
    p[7] = 0x60;
    p[8] = -0x110;
    p[9] = -0x160;
    p[5] = 0x1A;
    sp[6] = 0x400;
    func_8002F688();
    D_800F2DE4 = 0;
    D_800ABEB0 = 0;
    D_800AC5DC = 0x80;
    func_8003A198();
    if (D_800AD4F2 == 6) {
        func_80031E3C();
        func_8002A604();
        func_8003A1B4();
        D_800ACC18 = D_800F4D40;
    }
    D_800F4CB0 = 0;
    sp[0] = func_8001FDD4(D_800ACC14 + 0x70, sp[0], 0, 0, 0x140, 0x200, 8, 8, 8);
    D_800F4C6A = 1;
    D_800F3A26 = 0x10;
    D_800F4D8A = 0;
}
