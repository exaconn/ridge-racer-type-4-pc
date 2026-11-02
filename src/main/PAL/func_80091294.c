void func_80091294(int a0, int a1, int a2) {
    a0 <<= 4; a1 <<= 4; a2 <<= 4;
    __asm__ volatile ("ctc2 %0, $21" : : "r"(a0));
    __asm__ volatile ("ctc2 %0, $22" : : "r"(a1));
    __asm__ volatile ("ctc2 %0, $23" : : "r"(a2));
}
