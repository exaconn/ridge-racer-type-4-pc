/* GCC_VERSION: 2.7.2 */
extern struct { unsigned char f[0x20]; struct { short a, b, c; char pad[10]; } e[24]; } D_8010FE18;
extern struct { short w0; char p2[4]; short w6; unsigned char b8; char p9; short wA; char pad[42]; } D_8010F916[];
extern short D_80110114;
extern int func_8008A564();
extern int func_80088608();
int func_80084E74(a0, a1, a2, a3, a4, a5, a6)
int a0;
short a1;
short a2;
short a3;
int a4;
unsigned short a5;
unsigned short a6;
{
    short *t; register int y0 asm("$4");
    register int x4 asm("$4"); register int x5 asm("$5"); register int x3 asm("$3");
    int i;
    if ((unsigned short)a0 >= 0x18) return -1;
    i = (short)a0;
    x4 = D_8010F916[i].wA;
    if (x4 != a1) return -1;
    x5 = D_8010F916[i].w6;
    if (x5 != a2) return -1;
    x3 = D_8010F916[i].w0;
    if (x3 != a3) return -1;
    func_8008A564();
    y0 = a5;
    t = &D_80110114; __asm__("" : "=r"(t) : "0"(t));
    t[0] = 0x21;
    t[2] = a0;
    *((char *)t - 8) = D_8010F916[i].b8;
    D_8010FE18.e[i].c = func_80088608(y0, a6);
    D_8010FE18.f[i] |= 4;
    return 0;
}
