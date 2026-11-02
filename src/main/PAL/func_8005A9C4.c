void func_8005A9C4(void)
{
    int *g = (int *)0x1F800000;
    char *p = (char *)g[0];
    int *o;
    p[3] = 1;
    *(int *)(p + 4) = 0xE1000205;
    *(int *)p = (*(int *)p & 0xFF000000) | (*(int *)(g[1] + 0xAFC) & 0xFFFFFF);
    o = (int *)(g[1] + 0xAFC);
    *o = (*o & 0xFF000000) | ((int)p & 0xFFFFFF);
    g[0] = (int)(p + 8);
}
