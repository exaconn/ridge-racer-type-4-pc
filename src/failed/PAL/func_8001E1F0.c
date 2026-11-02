/* CC1_FLAGS: -fno-strength-reduce */
void func_8001E1F0(a0, a1)
int a0;
int a1;
{
    int *p;
    int c = a0;
    int i, j, t0 = 0xB00000, v, t;

    t = (c << 8) | 0x66000000;
    t |= c << 16;
    c |= t;
    p = *(int **)0x1F800000;
    ((unsigned short *)a1)[7]++;
    a1 = ((unsigned short *)a1)[7];
    for (i = 0; i < 0x90; i += 0x30) {
        j = 0;
        a0 = (int)p + 3;
        do {
            v = (a1 / 4) & 0xF;
            if (v == 0xF) v = 0;
            *(int *)(a0 + 5) = t0 | (j + 0xAC);
            *(int *)(a0 + 1) = c;
            *(int *)(a0 + 0xD) = 0x180010;
            *(int *)(a0 + 9) = (v << 4) + 0x2F29A400;
            a1 += 5;
            j += 0x1A;
            p[0] = *(int *)*(int *)0x1F800004;
            *(char *)a0 = 4;
            a0 += 0x14;
            *(int *)*(int *)0x1F800004 = (int)p;
            ((char *)*(int *)0x1F800004)[3] = 0;
            p += 5;
        } while (j < 0x82);
        t0 += 0x300000;
    }
    *(int *)0x1F800000 = (int)p;
    p[1] = 0xE100022A;
    *(int *)*(int *)0x1F800000 = *(int *)*(int *)0x1F800004;
    ((char *)*(int *)0x1F800000)[3] = 1;
    *(int *)*(int *)0x1F800004 = *(int *)0x1F800000;
    ((char *)*(int *)0x1F800004)[3] = 0;
    *(int *)0x1F800000 += 8;
}
