extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int D_800102F0;
extern int D_800AC5A0, D_800F3B24;
extern short D_800F4D44;
extern unsigned char D_800F4D7C;
int func_8003B294(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        if (D_800F4D44 == 1) {
            D_800F4D7C = 3;
        } else {
            D_800F4D7C = 1;
        }
        D_800AC5A0 = D_800102F0 + 0xAF000;
    case 1:
        if (func_8001292C(func_8001223C(1, 0x12), D_800AC5A0)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (func_80012A58()) {
            D_800F4D7C = 3;
            D_800F3B24 = D_800AC5A0;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}
