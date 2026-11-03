extern int D_800AC650[];

extern short D_800F4DB8[];

extern void func_80054D30();

extern void func_8008A674();

void func_8005CA14(void) {
    D_800AC650[0x434 / 4] = 2;
    D_800AC650[0x43C / 4] = 0;
    func_80054D30(0x20, 0x100, 0);
    D_800AC650[0x41C / 4] = 3;
    D_800AC650[0x420 / 4] = 0;
    if (D_800F4DB8[0x32 / 2] >= 0) {
        func_8008A674(D_800F4DB8[0x32 / 2]);
        D_800F4DB8[0x32 / 2] = -1;
    }
}
