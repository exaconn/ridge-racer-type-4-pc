extern int func_8004DDF8();
extern int func_8004DEBC();
extern int D_800A021C;
extern int D_800AC650[];
extern int D_800ACBBC;
extern int D_800ACC18;
extern int D_8010C1F0;

int func_8004E194(void) {
    switch (D_800A021C) {
    case 0:
        if (func_8004DEBC(3, 0) != 0) {
            D_800A021C += 1;
        }
        return 0;
    case 1:
        if (func_8004DEBC(D_800ACBBC, 0) != 0) {
            D_800A021C += 1;
        }
        return 0;
    case 2:
        if (func_8004DDF8(D_800AC650[0x570 / 4], D_800AC650[0x574 / 4], D_800AC650[0x578 / 4]) != 0) {
            D_800A021C = 0;
            D_800ACC18 = D_8010C1F0;
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}
