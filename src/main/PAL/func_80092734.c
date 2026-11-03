extern int func_80092FFC();
extern unsigned short func_800959E4();

int func_80092734(int *pix, int tp, int abr, int x, int y, int w, int h) {
    short sp[4];
    int r;
    sp[0] = x;
    sp[1] = y;
    sp[3] = h;
    switch (tp) {
    case 0:
        sp[2] = w / 4;
        break;
    case 1:
        sp[2] = w / 2;
        break;
    case 2:
        sp[2] = w;
        break;
    }
    func_80092FFC(sp, pix);
    /* ASM_BARRIER: boost x and abr reference counts so global-alloc assigns $s1 to x, $s2 to abr, and $s3 to y */
    __asm__ volatile("" : : "r"(x), "r"(abr));
    r = func_800959E4(tp, abr, x, y);
    /* ASM_BARRIER: keep andi right after the call, before the epilogue loads */
    __asm__ volatile("" : "=r"(r) : "0"(r));
    return r;
}
