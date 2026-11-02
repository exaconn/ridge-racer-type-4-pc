void func_80091228(int a0, int a1, int a2) {
    __asm__ volatile ("ctc2 %0, $0" : : "r"(a0));
    __asm__ volatile ("ctc2 %0, $2" : : "r"(a1));
    __asm__ volatile ("ctc2 %0, $4" : : "r"(a2));
}
