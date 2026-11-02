extern char D_800AD528[];

void func_8005C0C4(void) {
    int i;
    char *b;
    char *q;

    i = 0;
    b = D_800AD528;
    q = b;
    do {
        *(short *)(b + i * 4) = 0;
        *(short *)(b + i * 4 + 2) = 0;
        *(short *)(q + i * 16 + 0x10) = 1;
        *(short *)(q + i * 16 + 0x12) = 1;
        *(short *)(q + i * 16 + 0x14) = 0;
        b[i * 4 + 0x28] = 1;
        b[i * 4 + 0x29] = 0;
        b[i * 4 + 0x2A] = 2;
        b[i * 4 + 0x2B] = 0;
        i++;
    } while (i < 2);
    D_800AD528[0x30] = 0x10;
    D_800AD528[0x31] = 0x10;
    *((short *) (D_800AD528 + 0x32)) = 0;
    *((int *) (D_800AD528 + 0x34)) = 0;
    *((int *) (D_800AD528 + 0x38)) = 0x10;
}
