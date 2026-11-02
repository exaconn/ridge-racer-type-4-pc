extern int func_80096434();
extern char D_800ABE08[];
extern unsigned short D_8009EA80[];

void func_8001F194(void)
{
    /* ASM_BARRIER: register pins reproduce original t/a register allocation */
    register int a2 asm("$6");
    register int a3 asm("$7");
    int t1;
    int t2;
    register int t4 asm("$12");
    register int e asm("$13");
    unsigned short *t6;
    register unsigned short *t0 asm("$8");
    short *t3;
    char *a1;
    unsigned short u;
    func_80096434(D_800ABE08, 0x90);
    func_80096434(D_800ABE08 + 0x24, 0x90);
    t4 = 0;
    e = 8;
    t6 = D_8009EA80;
    t2 = 0x20;
    t1 = t4;
    do {
        a2 = 0;
        t3 = (short *)(t2 + D_800ABE08);
        t0 = t6;
        a3 = t1;
loop:
        a1 = a3 + D_800ABE08;
        if (a2 == e) {
            int m;
            __asm__ volatile("" : "=r"(m) : "0"(-1)); /* ASM_BARRIER: keep -1 inside branch */
            *t3 = m;
        } else {
            u = *t0;
            a1[3] = (a2 / 2) + (a2 < 4);
            *(unsigned short *)a1 = u;
        }
        t0++;
        a2++;
        a3 += 4;
        if (a2 < 9) goto loop;
        t2 += 0x24;
        t4++;
        t1 += 0x24;
    } while (t4 < 4);
}
