
extern int func_80092FFC();
extern unsigned short func_80095A24();
int func_8009281C(int a, int b, int c) {
    short sp[4];
    int r;
    sp[2] = 0x100;
    sp[0] = b;
    sp[1] = c;
    sp[3] = 1;
    func_80092FFC(sp, a);
    r = func_80095A24(b, c);
    /* ASM_BARRIER: keep andi right after the call, before the epilogue loads */
    __asm__ volatile("" : "=r"(r) : "0"(r));
    return r;
}
