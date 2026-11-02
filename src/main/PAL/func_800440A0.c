extern int func_80055330();
extern int func_8005534C();
extern int D_800ACBA8;
extern char D_8010B2E0[];

void func_800440A0(int a0) {
    int s2, s0, x, s1, v1, y;
    int *v0;
    s1 = D_800ACBA8;
    s2 = 0;
    s0 = -1;
    a0 = ((unsigned)a0 >> 4) & 0xFF;
    /* ASM_BARRIER: legacy, reason not recorded (forces reg copy/CSE/scheduling); retire & retest with fd.py */
    __asm__("" : "=r"(x) : "0"(a0));
    if (a0 < 8) {
        s0 = (a0 - 1) % 4;
    }
    if (x >= 5) {
        s2 = 1;
    }
    if (s0 >= 0) {
        func_8005534C(1, x);
        s1 += s0 * 8;
        func_80055330(*(int *)s1, *(unsigned char *)(s1 + 4), *(unsigned char *)(s1 + 5), *(unsigned char *)(s1 + 6));
        v1 = 7;
    } else {
        func_8005534C(0, x);
        v1 = 7;
    }
    v0 = (int *)D_8010B2E0;
    v0 = (int *)((char *)v0 + 0x578);
    do {
        v0[0x70 / 4] = s2;
        v1--;
        v0 = (int *)((char *)v0 - 0xC8);
    } while (v1 >= 0);
}
