void func_8009123C(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $25" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $26" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $27" : : "r"(a2));
}
