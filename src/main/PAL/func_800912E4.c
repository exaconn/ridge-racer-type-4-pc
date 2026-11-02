void func_800912E4(int *a0, int *a1) {
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("nop\n\t.word 0x4A4A6412");
    __asm__ volatile ("swc2 $9, 0(%0)\n\tswc2 $10, 4(%0)\n\tswc2 $11, 8(%0)" : : "r"(a1));
}
