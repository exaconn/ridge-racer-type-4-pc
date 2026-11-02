/* CC1_FLAGS: -mno-split-addresses */
extern int D_800ABDE8;
extern int D_800ABDE4;
extern char D_800ABDC0[];
extern unsigned int *D_800ABDAC;
extern short *D_800ABDA8;
extern int func_80097714();
extern int func_80097704();
extern int func_8009BC3C();
int func_8009BB58() {
    volatile unsigned int *g;
    short *h;
    if (D_800ABDE8) {
        g = D_800ABDAC;
        D_800ABDE4 = 0x11;
        D_800ABDE8 = 0;
        g[0] = ~0x80;
        g[1] &= ~0x80;
        h = D_800ABDA8;
        h[5] = 0;
        func_80097714(1, D_800ABDC0);
        D_800ABDA8[5] = 0x40;
        D_800ABDA8[7] = 0x88;
        D_800ABDA8[4] = 0xD;
        D_800ABDA8[5] = 0;
        return 0;
    }
    if (D_800ABDE4 & 1) return 0;
    func_80097714(1, D_800ABDC0);
    func_80097704(1, D_800ABDC0);
    func_8009BC3C();
    return 0;
}
