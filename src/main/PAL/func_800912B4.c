void func_800912B4(int a0, int a1) {
    a0 <<= 16; a1 <<= 16;
    __asm__ volatile ("ctc2 %0, $24" : : "r"(a0));
    __asm__ volatile ("ctc2 %0, $25" : : "r"(a1));
}
