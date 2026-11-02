void func_80091660(int *a0, int *a1, int *a2) {
    register int t0 __asm__("$8");
    register int t1 __asm__("$9");
    register int t2 __asm__("$10");
    register int t5 __asm__("$13");
    register int t6 __asm__("$14");
    register int t7 __asm__("$15");
    __asm__ volatile ("cfc2 %0, $0" : "=r"(t5));
    __asm__ volatile ("cfc2 %0, $2" : "=r"(t6));
    __asm__ volatile ("cfc2 %0, $4" : "=r"(t7));
    t0 = a0[0];
    t1 = a0[1];
    t2 = a0[2];
    __asm__ volatile ("ctc2 %0, $0" : : "r"(t0));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(t1));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(t2));
    __asm__ volatile ("lwc2 $11, 8(%0)\n\tlwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)" : : "r"(a1));
    __asm__ volatile ("nop\n\t.word 0x4B70000C");
    __asm__ volatile ("swc2 $25, 0(%0)\n\tswc2 $26, 4(%0)\n\tswc2 $27, 8(%0)" : : "r"(a2));
    __asm__ volatile ("ctc2 %0, $0" : : "r"(t5));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(t6));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(t7));
}
