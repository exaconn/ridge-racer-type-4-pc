int *func_80090C74(int *a0, int *a1) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    int *r;
    t0 = a0[0];
    t1 = a0[1];
    __asm__ volatile ("mtc2 %0, $0" : : "r"(t0));
    __asm__ volatile ("mtc2 %0, $1" : : "r"(t1));
    __asm__ volatile ("nop\n\t.word 0x4A486012");
    __asm__ volatile ("swc2 $9, 0(%0)\n\tswc2 $10, 4(%0)\n\tswc2 $11, 8(%0)" : : "r"(a1));
    /* ASM_BARRIER: legacy, reason not recorded (forces reg copy/CSE/scheduling); retire & retest with fd.py */
    __asm__ volatile ("" : "=r"(r) : "0"(a1));
    return r;
}
