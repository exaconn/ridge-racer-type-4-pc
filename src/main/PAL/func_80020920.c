void func_80020920(unsigned char *a0, unsigned char *a1) {
    a0[3] = 4;
    a0[7] = 0x64;
    *(unsigned short *)(a0 + 8) = *(unsigned short *)(a1 + 0);
    *(unsigned short *)(a0 + 0xA) = *(unsigned short *)(a1 + 2);
    *(unsigned short *)(a0 + 0x10) = *(unsigned short *)(a1 + 4);
    *(unsigned short *)(a0 + 0x12) = *(unsigned short *)(a1 + 6);
    a0[0xC] = a1[8];
    a0[0xD] = a1[0xA];
    *(unsigned short *)(a0 + 0xE) = *(unsigned short *)(a1 + 0xC);
    if (*(int *)(a1 + 0x10) != 0) {
        a0[7] |= 2;
    } else {
        a0[7] &= 0xFD;
    }
    a0[7] |= 1;
}
