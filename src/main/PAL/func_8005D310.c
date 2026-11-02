extern int D_800AC650[];
extern short D_800F3A26;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern int func_8004EA8C();
extern void func_80052800();
extern void func_800528A8();
extern void func_8005CA14();
extern int func_8005CA88();
extern void func_80092CB8();

void func_8005D310(void) {
    int s1 = func_8005CA88();
    switch (D_800AC650[0x444 / 4]) {
    case 0:
        func_80092CB8(1);
        D_800AC650[0x444 / 4]++;
    case 1:
        if (func_8004EA8C() != 0) {
            D_800AC650[0x444 / 4]++;
            func_8005CA14();
            func_80052800();
            func_800528A8(0, 0x25);
        }
        break;
    case 2:
        if (s1 == D_800AC650[0x444 / 4]) {
            func_80092CB8(0);
            D_800F4C6A = s1;
            D_800F3A26 = 7;
            D_800AC650[0x444 / 4] = 0;
            D_800F4D8A = 0;
        }
        break;
    }
}
