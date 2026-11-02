extern int func_8008A674();
extern short func_8008AA74();
extern short func_8008AEC4();
extern int func_8008B074();
extern int func_800963C4();
extern int func_8001E84C();
extern char D_800110D8[];
extern char D_800110F0[];
extern short D_800F4DB8[];

void func_8004ED38(int *arg0, int arg1, int arg2) {
    int k = (int)D_800F4DB8; short *p = (short *)(k + (arg2 << 1));
    short v;
    /* ASM_BARRIER: original keeps the first result sign-extended in $a1 */
    register int t asm("$5");
    short v2;
    if (p[0x12] >= 0) {
        func_8008A674(p[0x12]);
        p[0x12] = -1;
    }
    v = func_8008AA74(arg0[0], -1, arg1);
    p[0x12] = v;
    /* ASM_BARRIER: keep the result store ahead of the sign-extension in the schedule */
    __asm__ volatile("");
    t = v;
    if (t == -1) {
        func_8001E84C(D_800110D8);
        func_800963C4(1);
    }
    v2 = func_8008AEC4(arg0[1]);
    p[0x12] = v2;
    if (v2 == -1) {
        func_8001E84C(D_800110F0);
        func_800963C4(1);
    }
    func_8008B074(1);
}
