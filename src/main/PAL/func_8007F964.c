/* GCC_VERSION: 2.7.2 */
extern int func_800879F4();
extern unsigned short D_800A4904[], D_800A4914[];
extern int D_8010F05C, D_8010F060, D_8010F064;
extern int D_8010F0E8[32][16];
void func_8007F964(void) {
    unsigned short *d, *s, *p; int i, j;
    d = (unsigned short *)0x1F801C00;
    i = 0;
    p = D_800A4904;
    do {
        j = 0;
        s = p;
        do { *d++ = *s++; j++; } while (j < 8);
        i++;
    } while (i < 0x18);
    d = (unsigned short *)0x1F801D80;
    i = 0;
    s = D_800A4914;
    do { *d++ = *s++; i++; } while (i < 0x10);
    func_800879F4(0x18, j, d, p);
    for (j = 0; j < 0x20; j++)
        for (i = 0; i < 16; i++) D_8010F0E8[j][i] = 0;
    D_8010F064 = 0x3C;
    D_8010F060 = 0;
    D_8010F05C = 0;
}
