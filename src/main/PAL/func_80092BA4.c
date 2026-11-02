/* CC1_FLAGS: -mno-split-addresses */
extern unsigned char D_800ABAC1[];
extern int D_80011E50[];
extern int (*D_800ABABC)();
extern int *D_800ABAB8;
extern int func_8008B3C4();
int func_80092BA4(a0) int a0; {
    register unsigned char *p __asm__("$17"); /* ASM_BARRIER: original keeps the pointer in s1 and a0 in s0 */
    int old;
    p = D_800ABAC1;
    old = p[0];
    if (p[1] >= 2) {
        D_800ABABC(D_80011E50, a0);
    }
    if (a0 != p[0]) {
        ((int (*)())D_800ABAB8[0x34 / 4])(1);
        p[0] = a0;
        func_8008B3C4(2, 0);
    }
    return old;
}
