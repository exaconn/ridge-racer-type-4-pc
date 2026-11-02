void func_800911E8(int a0, int a1, int a2) {
    __asm__ volatile ("mtc2 %0, $17" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $18" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $19" : : "r"(a2));
}
