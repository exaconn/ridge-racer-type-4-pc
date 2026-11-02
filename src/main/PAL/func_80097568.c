/* CC1_FLAGS: -mno-split-addresses */
extern int func_80096F04(), func_80096F14(), func_80097704(), func_80097714();
extern int D_801143E8, D_801143F4, func_80097618(), func_80097680();
extern int D_801143EC[];
int func_80097568(void) {
    int *p;
    char *s;
    func_80096F04();
    p = D_801143EC;
    s = (char *)p - 4;
    p[0] = (int)func_80097618;
    p[1] = (int)func_80097680;
    D_801143E8 = 0;
    D_801143F4 = 0;
    func_80097714(1, s);
    func_80097704(1, s);
    func_80096F14();
    /* ASM_BARRIER: keeps li v0,1 as a live constant so it is emitted before the epilogue loads (original order) */
    { int r; __asm__("" : "=r"(r) : "0"(1)); return r; }
}
