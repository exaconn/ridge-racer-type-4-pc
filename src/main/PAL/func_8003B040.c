extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_800436E8();
extern unsigned char D_8009F8FC;
extern char D_800FFC48[];
extern int D_800ACC18;
extern unsigned char D_800F4D7C;
int func_8003B040(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_800ACC18 = (int)D_800FFC48;
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (func_8001292C(func_8001223C(1, D_8009F8FC + 0x11), D_800ACC18)) {
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
