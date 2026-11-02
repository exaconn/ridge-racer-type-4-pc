extern int func_8001223C();
extern int func_800129BC();
extern int func_80012A58();
extern int func_8004139C();
extern char D_8009F8FC;
extern int D_800ACC18, D_800F4C1C;
extern unsigned char D_800F4D7C;
int func_8003B47C(void) {
    int r = 0;
    int v;
    switch (D_800F4D7C) {
    case 0:
        D_8009F8FC = 0;
        D_800F4D7C = 1;
        D_800F4C1C = 4;
    case 1:
        if (func_800129BC(func_8001223C(1, 0x40), 0, 0xC0, D_800ACC18)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        v = func_80012A58();
        if (v) {
            func_8004139C(D_800ACC18, 0);
            D_800F4D7C = 3;
            D_800ACC18 += v;
        }
        break;
    case 3:
        r = 1;
        break;
    }
    return r;
}
