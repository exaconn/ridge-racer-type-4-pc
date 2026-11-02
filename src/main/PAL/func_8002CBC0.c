extern int func_80055250();
int func_8002CBC0(char *a, int b)
{
    char *p;
    int t;
    if (*(short *)(a + 0x1D8) < 0x320) goto fail;
    p = a + b * 2;
    t = func_80055250(*(short *)(a + 0x1EA), *(short *)(p + 0x100));
    if (t == -1) {
fail:
        return 0;
    }
    *(short *)(p + 0x1C4) = -(t * *(short *)(a + 0x1D8) / 2800);
    *(short *)(p + 0x1BC) = 1;
    *(unsigned short *)(a + 0x1B8) ^= 1 << b;
    return 1;
}
