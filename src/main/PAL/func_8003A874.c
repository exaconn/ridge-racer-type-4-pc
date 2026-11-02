extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int D_800102F0;
extern unsigned char D_8009F8FC;
extern int D_800ACC18, D_800F4D40, D_800F4D70;
extern short D_800F4D2C, D_800F4D44;
extern unsigned char D_800F4D7C;
int func_8003A874(void) {
    int r = 0;
    int v;
    switch (D_800F4D7C) {
    case 0:
        if (D_800F4D44 == 4) {
            r = 1;
            D_800F4D70 = D_800F4D40;
            D_800ACC18 = D_800F4D40;
            break;
        }
        if (D_800F4D44 == 5 && D_800F4D2C == 6) {
            r = 1;
            D_800ACC18 = D_800F4D40;
            break;
        }
        D_800F4D7C = 1;
        D_800ACC18 = D_800102F0;
        D_8009F8FC = *(unsigned char *)&D_800F4D44;
        if (*(unsigned char *)&D_800F4D44 >= 5) {
            D_8009F8FC = 0;
        }
    case 1:
        if (func_8001292C(func_8001223C(1, D_8009F8FC + 0xB), D_800ACC18)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        v = func_80012A58();
        if (v) {
            D_800ACC18 += v;
    case 3:
            r = 1;
            D_800F4D40 = D_800ACC18;
        }
        break;
    }
    return r;
}
