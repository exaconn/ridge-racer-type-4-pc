void func_800911C8(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $9" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $10" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $11" : : "r"(a2));
}
