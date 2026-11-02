extern short *D_800ACBE8;
extern int func_80026A74();
void func_80026998(int a0, int a1)
{
    int sp[4];
    register int y asm("$5");
    register int v asm("$2");
    a0 >>= 2;
    y = a1 >> 2;
    sp[0] = a0;
    sp[2] = y;
    if (a0 < 0) a0 += 0x7FF;
    v = y;
    a0 >>= 11;
    if (v < 0) v += 0x7FF;
    a0 = a0 + ((v >> 11) << 5);
    if (a0 < 0) a0 = 0;
    else if (a0 >= 0x400) a0 = 0x3FF;
    func_80026A74(sp, D_800ACBE8[a0], 6);
}
