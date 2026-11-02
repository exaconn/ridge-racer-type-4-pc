extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_800436E8();
extern char D_8009F8FC;
extern int D_800ACC18, D_800F4D74;
extern unsigned char D_800F4D7C;
int func_8003B398(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_8009F8FC = 0;
        D_800F4D7C = 1;
        D_800ACC18 = D_800F4D74;
    case 1:
        if (func_8001292C(func_8001223C(1, 0x13), D_800ACC18)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (func_80012A58()) {
            func_800436E8(D_800ACC18);
            D_800F4D7C = 3;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}
