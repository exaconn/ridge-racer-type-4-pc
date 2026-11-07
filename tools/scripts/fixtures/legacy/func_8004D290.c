extern int CurrentDrawBuf;

void AddLineF2Prim(unsigned short *a0, unsigned char *a1, int a2, int a3) {
    unsigned char *p = *(unsigned char **)0x1F800000;
    int *a;
    int w;
    p[3] = 3;
    p[7] = 0x40;
    p[7] = a2 ? 0x42 : 0x40;
    w = *(int *)p;
    p[4] = a1[0];
    p[5] = a1[1];
    p[6] = a1[2];
    *(unsigned short *)(p + 8) = a0[0];
    *(unsigned short *)(p + 10) = a0[1];
    *(unsigned short *)(p + 12) = a0[2];
    *(unsigned short *)(p + 14) = a0[3];
    a = (int *)(a3 * 4 + CurrentDrawBuf);
    *(int *)p = (w & 0xFF000000) | (a[0x70 / 4] & 0xFFFFFF);
    *(volatile int *)&a[0x70 / 4] = (a[0x70 / 4] & 0xFF000000) | ((int)p & 0xFFFFFF);
    p += 16;
    *(unsigned char **)0x1F800000 = p;
}
