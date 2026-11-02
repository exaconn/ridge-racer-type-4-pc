extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_80092D50();
extern int func_80092FFC();
extern int D_8009DF98;
extern int D_800ACC18;
int func_800137D4(void) {
    short sp[4];
    switch (D_8009DF98) {
    case 1:
        if (func_8001292C(func_8001223C(0, 0), D_800ACC18) == 0) return 0;
        D_8009DF98 = 2;
    case 2:
        if (func_80012A58() == 0) return 0;
        sp[0] = 0x340;
        sp[2] = 0x80;
        sp[1] = 0;
        sp[3] = 0xB4;
        func_80092FFC(sp, D_800ACC18);
        func_80092D50(0);
        D_8009DF98 = 0;
        return 1;
    default:
        return 2;
    }
}
