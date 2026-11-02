void func_800914C4(int *a0, int *a1) {
    __asm__ volatile ("lwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)\n\tlwc2 $11, 8(%0)" : : "r"(a0));
    __asm__ volatile ("nop\n\t.word 0x4A4DA412");
    __asm__ volatile ("swc2 $9, 0(%0)\n\tswc2 $10, 4(%0)\n\tswc2 $11, 8(%0)" : : "r"(a1));
}
