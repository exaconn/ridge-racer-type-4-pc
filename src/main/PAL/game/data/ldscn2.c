extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int D_800102F0;
extern unsigned char D_8009F8FC, D_800F4D2C;
extern int D_800ACC18, D_800F4D40;
extern unsigned char D_800F4D7C;
extern int func_8003A1B4();
extern int func_8004E008();
extern int func_8004E184();
extern int func_8004F600();
extern int D_800ACC18, D_800F4D40, D_800F4D74;

int func_8003A9E8(void);
int func_8003AAE8(void);

int func_8003A9E8(void) {
    int r = 0;
    int v;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_800ACC18 = D_800102F0;
        D_8009F8FC = D_800F4D2C;
    case 1:
        if (func_8001292C(func_8001223C(1, D_8009F8FC + 1), D_800ACC18)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        v = func_80012A58();
        if (v) {
            int t;
            D_800F4D7C = 3;
            t = D_800ACC18 + v;
            D_800ACC18 = t;
            D_800F4D40 = t;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}

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
