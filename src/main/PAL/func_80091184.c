void func_80091184(int *a0) {
    __asm__ volatile ("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" : : "r"(a0));
}
