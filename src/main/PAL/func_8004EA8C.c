/* RODATA 0x800110c0 0x18 */
extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_800131D0();
extern int func_80013200();
extern int func_800436E8();
extern int func_8004D4BC();
extern int D_800A021C;
extern int D_800ACC18;
extern short D_800AD520;
extern int D_800AD564;
extern short D_800F3914;
extern int D_800F4D40;
extern int D_8010C1F0;
int func_8004EA8C(void) {
    int v;
    int t;
    int u;
    int w;
    int g;
    int h;
    switch (D_800A021C) {
    case 0:
        v = func_8001292C(func_8001223C(1, 10), D_8010C1F0);
        if (v != 0) {
            D_800A021C += 1;
    case 1:
            t = func_80012A58();
            if (t != 0) {
                u = func_8004D4BC(t);
                w = D_8010C1F0 + u;
                D_800F3914 = 8;
                D_800ACC18 = w;
                D_8010C1F0 = w;
                D_800F4D40 = w;
                D_800A021C += 1;
            }
    default:
            v = 0;
        }
        return v;
    case 2:
        v = func_800131D0(D_800F3914);
        if (v != 0) {
            D_800A021C += 1;
    case 3:
            if (func_80013200() != 0) {
                D_800A021C += 1;
            }
            v = 0;
        }
        return v;
    case 4:
        g = D_800AD564 << 2;
        h = D_800AD520 + 0x97;
        v = func_8001292C(func_8001223C(2, g + h), D_800ACC18);
        if (v != 0) {
            D_800A021C += 1;
    case 5:
            if (func_80012A58() != 0) {
                func_800436E8(D_800ACC18);
                D_800A021C = 0;
                return 1;
            }
            v = 0;
        }
        return v;
    }
}
