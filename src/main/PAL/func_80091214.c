void func_80091214(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $12" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $13" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $14" : : "r"(a2));
}
