void func_80091114(int *a0) {
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
    __asm__ volatile ("ctc2 %0, $16" : : "r"(t0));
    __asm__ volatile ("ctc2 %0, $17" : : "r"(t1));
    __asm__ volatile ("ctc2 %0, $18" : : "r"(t2));
    __asm__ volatile ("ctc2 %0, $19" : : "r"(t3));
    __asm__ volatile ("ctc2 %0, $20" : : "r"(t4));
}
