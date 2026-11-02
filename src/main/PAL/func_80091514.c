void func_80091514(int *a0, int *a1, int *a2, int a3, int * volatile a4, int * volatile a5, int * volatile a6) {
    register int *t0 __asm__("$8");
    register int *t1 __asm__("$9");
    register int *t2 __asm__("$10");
    __asm__ volatile ("lwc2 $20, 0(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $21, 0(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $22, 0(%0)" : : "r"(a2));
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a2));
    __asm__ volatile ("mtc2 %0, $8" : : "r"(a3));
    __asm__ volatile ("nop\n\t.word 0x4AF8002A");
    t0 = a4;
    t1 = a5;
    t2 = a6;
    __asm__ volatile ("swc2 $20, 0(%0)" : : "r"(t0));
    __asm__ volatile ("swc2 $21, 0(%0)" : : "r"(t1));
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(t2));
}
