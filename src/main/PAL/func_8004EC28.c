extern char D_800F74B0[];
extern char D_800AC650[];
extern struct { char pad[0x24]; short h[8]; char pad2[0x2860 - 0x34]; short a, b; } D_800F4DB8;
extern int func_8008BFD0();
extern int func_800825B4();
extern int func_80083CD4();
extern int func_80083DE4();
extern int func_80084004();
extern int func_80082C74();
extern int func_800829F4();
extern int func_80096434();
extern int func_80055300();
extern int func_8005D858();
extern int func_80052800();
extern int func_80052760();
void func_8004EC28() {
    char *s1;
    register char *s0 asm("$16");
    int i; int n; short m;
    func_8008BFD0(1);
    func_800825B4(0, 0, 1);
    func_80083CD4(0, 0x7F, 0x7F);
    s0 = D_800F74B0;
    func_80083DE4(s0, 2, 1);
    func_80084004(1);
    s1 = s0 - 0x26F8;
    func_80082C74();
    func_800829F4(0x7F, 0x7F);
    func_80096434(s1, 0xA8D0);
    func_80096434(D_800AC650, 0x57C);
    s0 -= 0x1498;
    func_80055300();
    func_8005D858();
    *(int *)(s1 + 8) = 0x80;
    n = -1;
    for (i = 3; i >= 0; i--) {
        *(int *)(s0 + 0x3C) = n;
        s0 -= 0x620;
    }
    m = -1;
    for (i = 7; i >= 0; i--)
        D_800F4DB8.h[i] = m;
    D_800F4DB8.a = -1;
    D_800F4DB8.b = -1;
    func_80052800();
    func_80052760();
}
