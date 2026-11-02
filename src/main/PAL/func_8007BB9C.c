extern int func_80078414();
extern int func_8007A310();
extern int func_8007A9CC();
extern struct { char a[4]; unsigned char b; char c[3]; } D_800A4258[];
extern unsigned char D_800A44D8[][4];
extern char D_800A457C[];
extern unsigned char D_800ACBCC[];
extern short D_800F3A1C;
extern unsigned char D_800F3A24[];
extern char D_800F4D50[];
extern short D_800FFC00;
extern short D_800FFC18;
extern short D_80109490;

void func_8007BB9C()
{
    unsigned char *e;
    int s2, s4, s0, s3;
    int a0;
    unsigned char (*tab)[4];
    int i = D_800A4258[D_800F3A1C].b;
    tab = D_800A44D8;
    e = tab[i];
    s2 = e[0];
    s4 = e[1];
    s0 = e[2];
    s3 = e[3];
    if (D_800FFC18 != 0) goto l54;
    if (D_800F3A1C != 0x3D) goto l54;
    if (func_80078414(D_800ACBCC[D_800FFC00], D_800A457C) == 0) goto l48;
    D_80109490 = 1;
    goto l54;
l48:
    D_80109490 = 0;
l54:
    if (func_80078414(D_800ACBCC[D_800FFC00], D_800F4D50) != 1) goto ca0;
    D_800F3A1C = s2;
    if (s0 == 1) { a0 = 2; goto cf0; }
    func_8007A9CC(2, 0);
    return;
ca0:
    if (s0 != 1) goto ce0;
    if (D_800F3A24[D_800FFC00] + s3 < 0x10) goto ce0;
    D_800F3A1C = 0x16;
    return;
ce0:
    D_800F3A1C = s4;
    if (s0 != 1) return;
    a0 = 1;
cf0:
    func_8007A310(a0, 0);
}
