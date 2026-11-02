extern unsigned char D_800A1FC4[];
extern unsigned char D_800AC650[];

void func_8005C26C(int a0) {
    int v1, a, v2;
    D_800AC650[0x3F0] = (D_800A1FC4[0] * a0) / 255;
    D_800AC650[0x3F1] = (D_800A1FC4[1] * a0) / 255;
    v1 = D_800AC650[0x3F0] * *(int *)(D_800AC650 + 0x3E8);
    D_800AC650[0x3F2] = (D_800A1FC4[2] * a0) / 255;
    if (v1 < 0) v1 += 0xFF;
    a = D_800AC650[0x3F1] * *(int *)(D_800AC650 + 0x3E8);
    D_800AC650[0x3F0] = v1 >> 8;
    if (a < 0) a += 0xFF;
    v2 = D_800AC650[0x3F2] * *(int *)(D_800AC650 + 0x3E8);
    D_800AC650[0x3F1] = a >> 8;
    if (v2 < 0) v2 += 0xFF;
    D_800AC650[0x3F2] = v2 >> 8;
}
