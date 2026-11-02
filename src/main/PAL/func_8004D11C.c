extern int D_800ACC14;

void func_8004D11C(unsigned short *a0, unsigned char *a1, unsigned char *a2, unsigned char *a3, unsigned char *a4, int a5) {
    unsigned char *p = *(unsigned char **)0x1F800000;
    int *a;
    p[3] = 8;
    p[7] = 0x3A;
    *(unsigned short *)(p + 8) = a0[0];
    *(unsigned short *)(p + 10) = a0[1];
    *(short *)(p + 0x10) = a0[0] + a0[2];
    *(unsigned short *)(p + 0x12) = a0[1];
    *(unsigned short *)(p + 0x18) = a0[0];
    *(short *)(p + 0x1A) = a0[1] + a0[3];
    *(short *)(p + 0x20) = a0[0] + a0[2];
    *(short *)(p + 0x22) = a0[1] + a0[3];
    p[4] = a1[0];
    p[5] = a1[1];
    p[6] = a1[2];
    p[0xC] = a2[0];
    p[0xD] = a2[1];
    p[0xE] = a2[2];
    p[0x14] = a3[0];
    p[0x15] = a3[1];
    p[0x16] = a3[2];
    p[0x1C] = a4[0];
    p[0x1D] = a4[1];
    p[0x1E] = a4[2];
    a = (int *)(a5 * 4 + D_800ACC14);
    *(int *)p = (*(int *)p & 0xFF000000) | (a[0x70 / 4] & 0xFFFFFF);
    *(volatile int *)&a[0x70 / 4] = (a[0x70 / 4] & 0xFF000000) | ((int)p & 0xFFFFFF);
    p += 0x24;
    *(unsigned char **)0x1F800000 = p;
}
