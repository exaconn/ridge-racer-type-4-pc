/* RODATA 0x8001081c 0x1c */
extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern void func_800436E8();
extern void func_8004E008();
extern int func_8004E03C();
extern void func_8004F600();
extern char D_8009F8FC;
extern int D_800102F0;
extern int D_800ACC18;
extern int D_800F4D40;
extern unsigned char D_800F4D7C;
extern char D_8010C220[];

int func_8003BBBC(void) {
    int r = 0;
    int t;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (func_8001292C(func_8001223C(1, 0), D_800ACC18) != 0) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (func_80012A58() != 0) {
            func_800436E8(D_800ACC18);
            func_8004E008();
            D_800F4D7C = 3;
        }
        break;
    case 3:
        if (func_8004E03C(D_8010C220) != 0) {
            func_8004F600();
            D_800F4D7C = 4;
            D_800ACC18 = D_800102F0;
        }
        break;
    case 4:
        if (func_8001292C(func_8001223C(1, 10), D_800ACC18) != 0) {
            D_800F4D7C = 5;
        }
        break;
    case 5:
        t = func_80012A58();
        if (t != 0) {
            D_800ACC18 += t;
            D_800F4D7C = 6;
            D_800F4D40 = D_800ACC18;
            case 6:
            r = 1;
        }
        break;
    }
    return r;
}
