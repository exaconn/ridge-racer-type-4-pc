extern void func_8006E1AC();

void func_8001B640(void)
{
    unsigned short *g;
    func_8006E1AC();
    g = (unsigned short *)0x1F800000;
    g[0x17] <<= 1;
    g[0x18] <<= 1;
    g[0x19] <<= 1;
}
