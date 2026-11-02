extern int func_8003A1B4();
extern int func_8004E008();
extern int func_8004E184();
extern int func_8004F600();
extern int D_800ACC18, D_800F4D40, D_800F4D74;
extern unsigned char D_800F4D7C;
int func_8003AAE8(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_800ACC18 = D_800F4D40;
        func_8004E008();
    case 1:
        if (func_8004E184()) {
            D_800F4D40 = D_800ACC18;
            func_8004F600();
            D_800F4D7C = 2;
            func_8003A1B4();
            D_800ACC18 = D_800F4D74;
    case 2:
            r = 1;
        }
        break;
    }
    return r;
}
