extern char D_800F3920[];
int func_8002F8E4() {
    int b, off, i;
    /* ASM_BARRIER: pin reproduces original register assignment */
    register int n __asm__("$7");
    int a;
    unsigned v;
    n = 0;
    a = 0;
    do {
        char *p = D_800F3920;
        b = 0;
        off = a << 4;
      L1:
        i = 0;
        v = *(unsigned *)(off + (int)p + 0x10);
        do {
            if ((v >> i) & 1) n++;
            i++;
        } while (i < 20);
        b++;
        off += 4;
        if (b < 4) goto L1;
        a++;
    } while (a < 4);
    return n;
}
