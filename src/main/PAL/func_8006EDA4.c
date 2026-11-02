extern int func_8006E834();
extern int func_800713E0();
extern int func_8006EC64();
extern int func_8006EAC8();
extern int func_80060C70();
extern unsigned short D_8010E280;
extern unsigned int D_8010E290;
extern int D_8010E690;
extern short D_800F3914;

void func_8006EDA4(void) {
    short *g = (short *)0x1F800000;
    unsigned int v; int pad[12];
    D_8010E280 = (unsigned short)g[0x2E];
    g[0x2E] = -1;
    func_8006E834();
    v = func_800713E0();
    if (v < D_8010E290) {
        D_8010E290 = v;
    }
    func_8006EC64();
    func_8006EAC8();
    func_80060C70(g, D_8010E690, D_800F3914);
    g[0x2E] = (short)D_8010E280;
}
