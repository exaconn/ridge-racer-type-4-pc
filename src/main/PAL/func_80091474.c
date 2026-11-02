void func_80091474(int *a0, int *a1, int *a2) {
    __asm__ volatile ("lwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)\n\tlwc2 $11, 8(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a1));
    __asm__ volatile ("nop\n\t.word 0x4B38041C");
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(a2));
}
