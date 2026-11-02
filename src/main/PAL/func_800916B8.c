int func_800916B8(int a0) {
    int v;
    __asm__ volatile ("mtc2 %0, $30" : : "r"(a0));
    __asm__ volatile ("nop\n\tnop");
    __asm__ volatile ("mfc2 %0, $31" : "=r"(v));
    return v;
}
