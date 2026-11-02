int func_800915E4(int a0, int a1, int a2, int a3) {
    int v;
    __asm__ volatile ("mtc2 %0, $16" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $17" : : "r"(a1));
    __asm__ volatile ("mtc2 %0, $18" : : "r"(a2));
    __asm__ volatile ("mtc2 %0, $19" : : "r"(a3));
    __asm__ volatile ("nop\n\t.word 0x4B68002E");
    __asm__ volatile ("mfc2 %0, $7" : "=r"(v));
    return v;
}
