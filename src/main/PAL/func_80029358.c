extern void *D_800ACBFC;
extern void *D_800ACC20;
extern void *D_800F2ED0[];
void func_80029358(a) char *a; {
    int i;
    unsigned short *p;
    void **q;
    D_800ACC20 = a + 0x50;
    i = 0;
    q = D_800F2ED0;
    p = (unsigned short *)a;
    do { *q++ = a + 0x1B8 + *p++ * 2; i++; } while (i < 0x28);
    D_800ACBFC = a + 0x78;
}
