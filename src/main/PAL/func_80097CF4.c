/* CC1_FLAGS: -mno-split-addresses */
extern int (*D_800ABC6C)();
extern int D_800ABC9C;
extern int D_800ABCA0;
extern int D_800ABCB8;
extern int D_800ABCBC;
extern int D_80114428;
extern int D_8011442C;

int func_80097CF4(int a0) {
    int s1;
    /* ASM_BARRIER: pins p to s2 to match original register allocation */
    register int *p __asm__("$18");
    int *q;
    int t1, t2;
    s1 = (D_800ABCBC * 2) | (D_800ABCB8 == 0);
    if (s1 != a0) {
        D_800ABCA0 = 0;
        if (a0 & 1) {
            p = &D_80114428;
            t1 = *p;
            D_800ABCB8 = 0;
            if (t1 >= 0x96) {
                D_800ABC6C(D_800ABC9C);
            }
            *p = 0;
        } else {
            D_800ABCB8 = 1;
        }
        if (a0 & 2) {
            q = &D_8011442C;
            D_800ABCBC = 1;
            if (*q >= 0x96) {
                D_800ABC6C(D_800ABC9C + 0xF0);
            }
            *q = 0;
        } else {
            D_800ABCBC = 0;
        }
        D_800ABCA0 = 1;
    }
    return s1;
}
