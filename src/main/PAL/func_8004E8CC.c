extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_800436E8();
extern int func_80070108();
extern int D_800A021C;
extern char *D_8010C1F0;

int func_8004E8CC(void) {
    int v;
    switch (D_800A021C) {
    case 0:
        v = func_8001292C(func_8001223C(2, 2), D_8010C1F0);
        if (v != 0) {
            D_800A021C += 1;
    case 1:
            if (func_80012A58() != 0) {
                func_80070108(D_8010C1F0 + *(int *)(D_8010C1F0 + 4), 8);
                func_800436E8(D_8010C1F0 + *(int *)(D_8010C1F0 + 8));
                D_800A021C = 0;
                return 1;
            }
            v = 0;
        }
        return v;
    default:
        return 0;
    }
}
