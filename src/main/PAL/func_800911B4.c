void func_800911B4(int *a0, int *a1, int *a2) {
    __asm__ volatile ("lwc2 $20, 0(%0)" : : "r"(a0));
    __asm__ volatile ("lwc2 $21, 0(%0)" : : "r"(a1));
    __asm__ volatile ("lwc2 $22, 0(%0)" : : "r"(a2));
}
