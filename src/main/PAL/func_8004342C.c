extern int func_80043130();
extern int func_80096424();
extern int func_80096464();
extern char D_8010C158[];
extern char D_8010C168[];
int func_8004342C(a0, a1, a2, a3, a4, a5) int a0; int a1; int a2; int a3; int a4; int a5;
{
    unsigned s0; int r;
    s0 = a1 << 4;
    a0 &= 0xF;
    s0 |= a0;
    D_8010C168[0] = s0;
    r = func_80096464(a0);
    a2 = a2 * 4 + a3;
    D_8010C168[1] = (s0 >> 8) + (r & ~0x3F);
    D_8010C168[2] = a2;
    D_8010C168[3] = a2 >> 8;
    D_8010C168[4] = (a2 >> 16) + (func_80096464() & ~0x3F);
    D_8010C168[5] = a4 & 0xF;
    func_80043130(D_8010C168, D_8010C158);
    D_8010C158[0xD] = 0;
    func_80096424(a5, D_8010C158);
    return 0;
}
