extern unsigned char D_8009F810[];
extern unsigned char D_800AD4D8[];
void func_8003989C(void) {
    int v;
    if (D_800AD4D8[0x1E] == 0x12) {
        int i = D_800AD4D8[0x1C] * 2 + (D_800AD4D8[0x1D] >> 1);
        v = D_8009F810[i];
    } else {
        v = 1;
    }
    if (v < 0) {
        v = 0;
    } else if (v >= 9) {
        v = 8;
    }
    D_800AD4D8[0x12] = v;
    if (v == 1) {
        D_800AD4D8[0x13] = D_800AD4D8[0x18];
    }
}
