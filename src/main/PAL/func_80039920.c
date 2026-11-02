extern unsigned char D_8009F810[];
extern unsigned char D_800AD4D8[];
extern unsigned char D_800AD4EB;
extern unsigned char D_800F3920[];
void func_80039920(void) {
    int i = D_800AD4D8[0x1C] * 2 + (D_800AD4D8[0x1D] >> 1);
    int v = D_8009F810[i];
    if (v < 0) {
        v = 0;
    } else if (v >= 9) {
        v = 8;
    }
    D_800AD4EB = D_800F3920[v + 0x60];
}
