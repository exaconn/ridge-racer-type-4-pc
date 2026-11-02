void func_80091174(int *a0) {
    __asm__ volatile ("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" : : "r"(a0));
}
