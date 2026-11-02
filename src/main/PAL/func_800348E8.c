extern int func_8003660C();
extern int D_8009F6E4[];
extern short D_800AC5D0;
extern char D_800AD4D8[];
extern int D_800F2A68;
extern int D_800F2EB8;
extern signed char D_800F2FDC;
extern short D_800F3A26;
extern int D_800F4C64;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern int D_800FF6B0;

void func_800348E8(int arg0) {
    int *t;
    int b;
    if (D_800F2FDC == 0) {
        D_800AC5D0 = 0x1E;
        return;
    }
    D_800AD4D8[0x19] = 0;
    D_800F4C64 = -1;
    if (arg0 == 1) {
        D_800AD4D8[0x19] = 9;
        D_800FF6B0 = 6;
        D_800F2A68 = -D_800F2EB8;
        if (D_800F2FDC == 2) {
            D_800F4D8A = 0;
            D_800F4C6A = D_8009F6E4[3] >> 8;
            D_800F3A26 = *(unsigned char *)&D_8009F6E4[3];
            return;
        }
        func_8003660C(D_800AD4D8, D_800F2FDC);
        return;
    }
    b = (int)D_8009F6E4;
    D_800FF6B0 = 6;
    t = (int *)((D_800F2FDC - 1) * 4 + arg0 * 8 + b);
    D_800F4C6A = *t >> 8;
    {
        int v = *(unsigned char *)t;
        D_800F4D8A = 0;
        D_800F3A26 = v;
    }
}
