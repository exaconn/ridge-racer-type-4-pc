void func_800910B4(int *a0) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    register int t3 __asm__("$11");
    register int t4 __asm__("$12");
    t0 = a0[0];
    t1 = a0[1];
    t2 = a0[2];
    t3 = a0[3];
    t4 = a0[4];
    __asm__ volatile ("ctc2 %0, $0" : : "r"(t0));
    __asm__ volatile ("ctc2 %0, $1" : : "r"(t1));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(t2));
    __asm__ volatile ("ctc2 %0, $3" : : "r"(t3));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(t4));
}
