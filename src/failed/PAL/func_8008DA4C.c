/* CC1_FLAGS: -mno-split-addresses -fno-schedule-insns */
extern int D_80110330, D_80110334, D_80110338;
extern unsigned char D_800A6764[];
extern unsigned char D_800A64A5;
extern int D_800A652C[], D_800A64AC[];
extern volatile unsigned *D_800A6790;
extern char D_80011A14[], D_80011A24[], D_80011B20[];
extern int func_8008B154(), func_800963DC(), func_8001E84C(), func_8008D65C();
int func_8008DA4C(a0) int a0; {
    int v;
    int r;
    int c;
    D_80110330 = func_8008B154(-1) + 0x3C0;
    D_80110334 = 0;
    D_80110338 = (int)D_80011B20;
    for (;;) {
        v = func_8008B154(-1);
        if (D_80110330 < v) goto timeout;
        c = D_80110334;
        D_80110334 = c + 1;
        if (0x3C0000 < c) {
        timeout:
            func_800963DC(D_80011A14);
            func_8001E84C(D_80011A24, D_80110338, D_800A64AC[D_800A64A5], D_800A652C[D_800A6764[0]], D_800A652C[D_800A6764[1]]);
            func_8008D65C();
            r = -1;
        } else {
            r = 0;
        }
        if (r != 0) return -1;
        if ((*D_800A6790 & 0x1000000) == 0) return 0;
        if (a0 != 0) return 1;
    }
}
