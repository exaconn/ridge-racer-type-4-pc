void func_8005A91C(int a0, int a1, int a2)
{
    int *g = (int *)0x1F800000;
    char *p = (char *)g[0];
    int *o;
    int off = a2 * 4;
    p[3] = 4;
    p[7] = 0x65;
    *(short *)(p + 0x10) = 8;
    *(short *)(p + 0x12) = 0x10;
    p[0xC] = 0x3C;
    p[0xD] = 0x48;
    *(short *)(p + 8) = a0 - 4;
    *(short *)(p + 0xA) = a1 - 8;
    *(short *)(p + 0xE) = 0x1B96;
    *(int *)p = (*(int *)p & 0xFF000000) | (*(int *)(off + g[1]) & 0xFFFFFF);
    o = (int *)(off + g[1]);
    *o = (*o & 0xFF000000) | ((int)p & 0xFFFFFF);
    g[0] = (int)(p + 0x14);
}
