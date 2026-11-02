int func_800916D4(int *a0, int *a1, int *a2, int *a3) {
    int v, f;
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("nop\n\t.word 0x4A180001");
    __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(a1));
    __asm__ volatile ("swc2 $8, 0(%0)" : : "r"(a2));
    __asm__ volatile ("cfc2 %0, $31" : "=r"(f));
    __asm__ volatile ("mfc2 %0, $19" : "=r"(v));
    *a3 = f;
    return v >> 2;
}
