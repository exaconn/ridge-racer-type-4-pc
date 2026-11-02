void func_80091408(int *a0, int *a1, int *a2, int *a3, int * volatile a4, int * volatile a5, int * volatile a6) {
    register int *t0 __asm__("$8");
    register int *t1 __asm__("$9");
    register int *t2 __asm__("$10");
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" : : "r"(a2));
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a3));
    __asm__ volatile ("nop\n\t.word 0x4B18043F");
    t0 = a4;
    t1 = a5;
    t2 = a6;
    __asm__ volatile ("swc2 $20, 0(%0)" : : "r"(t0));
    __asm__ volatile ("swc2 $21, 0(%0)" : : "r"(t1));
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(t2));
}
