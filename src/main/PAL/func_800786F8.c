extern int func_80078204();
extern int func_80096484();
extern int func_80096F24();
extern int func_80096F34();
extern int func_80096F54();
extern int func_80096F64();
extern char D_800113E4[];
extern int D_8010EF78;
int func_800786F8(int a0, int a1, int a2, int a3, int a4, int a5) {
    char sp18[32];
    int h; register int r asm("$17"); int t;
    func_80096484(sp18, D_800113E4, a0, a1, a2);
    t = a4;
    h = func_80096F24(sp18, 0x8002);
    /* ASM_BARRIER: loads -1 into r after the call, not hoisted before it */
    __asm__ volatile("" : "=r"(r) : "0"(-1));
    if (h != r) {
        if (func_80096F34(h, a5, 0) != r) {
            D_8010EF78 = h;
            func_80078204();
            while (func_80096F54(h, a3, t) == 0) ;
            r = 0;
        }
        func_80096F64(h);
        return r;
    }
    /* ASM_BARRIER: re-launders r so the -1 return uses s1 (not the equal h in s0) */
    __asm__ volatile("" : "=r"(r) : "0"(r));
    return r;
}
