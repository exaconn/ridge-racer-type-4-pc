extern int func_80020920(), func_8001FCC8();
extern char D_8009EA90[], D_8009EACC[];
extern char *D_800ACC14;

void func_800211A4(a0)
int a0;
{
    unsigned *s0;
    unsigned *s2;
    char *s1;
    char *s4;
    int s3, s5, c3;
    unsigned m1;
    unsigned h1;
    unsigned m2;
    unsigned h2;

    s0 = *(unsigned **)0x1F800000;
    if (a0 >= 2) {
        s2 = (unsigned *)(D_800ACC14 + 0xB70);
        s5 = 4;
        if (a0 == 3) s5 = 2;
        s3 = 0;
        if (s5 != 0) {
            m1 = 0xFFFFFF;
            h1 = 0xFF000000;
            s1 = D_8009EACC;
            do {
                func_80020920(s0, s1);
                s1 += 0x14;
                s3++;
                *s0 = (*s0 & h1) | (*s2 & m1);
                *s2 = (*s2 & h1) | ((unsigned)s0 & m1);
                s0 += 5;
            } while (s3 < s5);
        }
    } else {
        s3 = 0;
        m2 = 0xFFFFFF;
        h2 = 0xFF000000;
        s4 = D_8009EA90;
        s2 = (unsigned *)(D_800ACC14 + 0x70);
        s1 = (char *)s0 + 0xA;
        do {
            func_80020920(s0, s4);
            __asm__ volatile("" : "=r"(c3) : "0"(3)); /* ASM_BARRIER: keep li 3 inside the loop (not hoisted) */
            if (a0 == c3) *(unsigned short *)s1 -= 0x80;
            s1 += 0x14;
            s4 += 0x14;
            s3++;
            *s0 = (*s0 & h2) | (*s2 & m2);
            *s2 = (*s2 & h2) | ((unsigned)s0 & m2);
            s0 += 5;
        } while (s3 < 3);
    }
    *(int *)0x1F800000 = func_8001FCC8(s2, s0, 5);
}
