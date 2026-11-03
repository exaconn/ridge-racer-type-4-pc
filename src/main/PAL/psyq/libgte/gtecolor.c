/* GCC_VERSION: 970318 */
void func_80091274(int a0, int a1, int a2);
void func_80091294(int a0, int a1, int a2);
void func_800912B4(int a0, int a1);

void func_80091274(int a0, int a1, int a2) {
    a0 <<= 4; a1 <<= 4; a2 <<= 4;
    __asm__ volatile ("ctc2 %0, $13" : : "r"(a0));
    __asm__ volatile ("ctc2 %0, $14" : : "r"(a1));
    __asm__ volatile ("ctc2 %0, $15" : : "r"(a2));
}

void func_80091294(int a0, int a1, int a2) {
    a0 <<= 4; a1 <<= 4; a2 <<= 4;
    __asm__ volatile ("ctc2 %0, $21" : : "r"(a0));
    __asm__ volatile ("ctc2 %0, $22" : : "r"(a1));
    __asm__ volatile ("ctc2 %0, $23" : : "r"(a2));
}

void func_800912B4(int a0, int a1) {
    a0 <<= 16; a1 <<= 16;
    __asm__ volatile ("ctc2 %0, $24" : : "r"(a0));
    __asm__ volatile ("ctc2 %0, $25" : : "r"(a1));
}
