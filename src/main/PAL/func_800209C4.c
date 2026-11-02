/* RODATA 0x800103c0 0x1c */
extern int func_80020920();
extern int func_80095A44();
extern char D_8009EBBC[];
extern char D_8009EB1C[];
extern char D_8009EC48[];
extern char D_800D0160[];
extern short D_80107178;
void func_800209C4(int a0)
{
    int n, i, j, a, b;
    char *tab;
    char *base;
    char *s1;
    char *s0;
    switch (a0) {
    case 0:
    case 5:
    default:
        n = 4;
        tab = D_8009EBBC;
        break;
    case 1:
        n = 8;
        tab = D_8009EB1C;
        break;
    case 2:
    case 3:
    case 4:
    case 6:
        if (D_80107178 == 2 && a0 == 4) goto l60;
        if (a0 != 6) { n = 12; goto l64; }
    l60:
        n = 6;
    l64:
        tab = D_8009EC48;
        break;
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < 2; j++) {
            s1 = D_800D0160 + j * 0x22778;
            s0 = s1 + (i * 0x14 + 0x18);
            func_80020920(s0, tab + i * 0x14);
            if (i != 0) func_80095A44(s1 + (i * 0x14 + 4), s0);
        }
    }
}
