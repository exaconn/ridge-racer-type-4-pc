int func_80091734(int a0, int a1, int a2) {
    int v;
    __asm__ volatile ("mtc2 %0, $12" : : "r"(a0));
    __asm__ volatile ("mtc2 %0, $14" : : "r"(a2));
    __asm__ volatile ("mtc2 %0, $13" : : "r"(a1));
    __asm__ volatile ("nop\n\tnop\n\t.word 0x4B400006");
    __asm__ volatile ("mfc2 %0, $24" : "=r"(v));
    return v;
}
