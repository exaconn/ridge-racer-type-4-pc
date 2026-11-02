void func_80091144(int *a0) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    t0 = a0[5];
    t1 = a0[6];
    t2 = a0[7];
    __asm__ volatile ("ctc2 %0, $5" : : "r"(t0));
    __asm__ volatile ("ctc2 %0, $6" : : "r"(t1));
    __asm__ volatile ("ctc2 %0, $7" : : "r"(t2));
}
