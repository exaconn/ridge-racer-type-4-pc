extern int *D_8010E698[];
extern int D_800F2FD8;

void func_800701C4(int a0) {
    int *v = D_8010E698[a0];
    int *w;
    int a2;
    int *g = (int *)0x1F800000;
    /* ASM_BARRIER: legacy, reason not recorded (forces reg copy/CSE/scheduling); retire & retest with fd.py */
    __asm__("" : "=r"(w) : "0"(v));
    a2 = v[0];
    g[0x15] = w[1];
    g[0x16] = D_8010E698[a0][2];
    D_800F2FD8 = a2;
    g[0x14] = (int)D_8010E698[a0] + 0xC;
}
