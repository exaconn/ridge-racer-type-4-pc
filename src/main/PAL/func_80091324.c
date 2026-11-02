void func_80091324(int *a0, int *a1) {
    __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(a0));
    __asm__ volatile ("nop\n\t.word 0x4AC8041E");
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(a1));
}
