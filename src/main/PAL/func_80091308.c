void func_80091308(int *a0, int a1, int *a2) {
    __asm__ volatile ("lwc2 $6, 0(%0)" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $8" : : "r"(a1));
    __asm__ volatile ("nop\n\t.word 0x4A780010");
    __asm__ volatile ("swc2 $22, 0(%0)" : : "r"(a2));
}
