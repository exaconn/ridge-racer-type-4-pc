extern int func_8001FC28(), func_8002B5D0(), func_8002B814(), func_8004C73C(), func_8004C928();
extern char D_800103FC[], D_80010408[], D_80010414[];
extern short D_800AC0E6, D_800AC1A0;
extern char *D_800ACC14;
extern int D_800F3A30, D_800F4C64;
extern int D_800F4C80[];

void func_80022448(void) {
    char sp18[8];
    int i;
    int y;
    int *q;
    char *b;
    int col, v;
    int sel;
    *(int *)0x1F800000 = func_8001FC28(D_800ACC14 + 0x70, *(int *)0x1F800000, 0, 0, 0x140, 0x100);
    i = 0;
    sel = D_800AC1A0;
    if (D_800F3A30 > 0) {
        y = 0x86;
        q = D_800F4C80;
    L1:
        v = *q;
        col = 0x7EF8;
        if (i == sel) {
            /* ASM_BARRIER: hide the known value of col so the then-arm reload is kept */
            __asm__ volatile("" : "=r"(col));
            col = 0x7EF8;
        }
        func_8002B5D0(0x78, y, v, col, 1);
        y += 0xA;
        q += 1;
        if (++i < D_800F3A30) goto L1;
    }
    func_8002B5D0(0x78, 0x5E, D_800F4C64, 0x7EF8, 1);
    func_8004C73C(0x78, 0x50, D_800103FC, 0x80, 0);
    func_8004C73C(0x78, 0x78, D_80010408, 0x80, 0);
    i = 7;
    b = sp18 + 7;
    do {
        *b = 0;
        i -= 1;
        b -= 1;
    } while (i >= 0);
    func_8002B814(sp18, D_800AC0E6 + 1, 1);
    func_8004C73C(0x78, 0x3C, sp18, 0x80, 0);
    func_8004C73C(0x78, 0xC8, D_80010414, 0x80, 0);
    func_8004C928(0);
}
