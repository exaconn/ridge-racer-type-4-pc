struct C { unsigned char r, g, b, c; };
extern int D_800AD568;
extern struct C D_800F3A18;
extern struct C D_800F3A20;

void func_80013890(int a0)
{
    short *g;
    *(int *)0x1F800060 = a0;
    g = (short *)0x1F800000;
    *(int *)(g + 0x32) = D_800AD568;
    D_800F3A20.b = 0x80;
    D_800F3A20.g = 0x80;
    D_800F3A20.r = 0x80;
    D_800F3A20.c = 0x2C;
    D_800F3A18.b = 0xFF;
    D_800F3A18.g = 0xFF;
    D_800F3A18.r = 0xFF;
    D_800F3A18.c = 0x3C;
    g[0x38] = 0x140;
    g[0x36] = 0;
    g[0x37] = 0;
    g[0x39] = 0x100;
}
