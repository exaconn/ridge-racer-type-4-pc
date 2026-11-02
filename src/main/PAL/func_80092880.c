extern int func_80092FFC();
extern int func_80095A24();
int func_80092880(int a, int b, int c) {
    short sp[4];
    int r;
    sp[2] = 0x10;
    sp[0] = b;
    sp[1] = c;
    sp[3] = 1;
    func_80092FFC(sp, a);
    /* ASM_BARRIER: keeps the u16 mask (andi) scheduled right after the call, before the epilogue loads */
    r = func_80095A24(b, c) & 0xFFFF; __asm__ volatile("" : "=r"(r) : "0"(r)); return r;
}
