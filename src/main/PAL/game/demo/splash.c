#include "game/demo/splash.h"
extern int D_800ACC14, D_8010E7A0, D_8010E7A4;
extern int func_8001FDD4();
extern int func_80020074();
extern int func_80092CB8();

int func_80071570(int a0) {
    int s5, cur;
    int *g = (int *)0x1F800000;
    g[0] = func_8001FDD4(D_800ACC14 + 0xB6C, g[0], 0, 0, 0x140, 0x200, 0, 0, 0);
    D_8010E7A4 = D_8010E7A4 + 1;
    if (D_8010E7A4 >= 3) {
        if (D_8010E7A4 == 3) {
            func_80092CB8(1);
            D_8010E7A0 = 0;
        }
        s5 = D_800ACC14 + 0x70;
        cur = g[0];
        if (D_8010E7A4 < 0x56) {
            D_8010E7A0 = D_8010E7A0 + 3;
        } else if (D_8010E7A4 < 0x60) {
            D_8010E7A0 = 0xFF;
        } else if (a0 == 0) {
            D_8010E7A4 = D_8010E7A4 - 1;
        } else if (D_8010E7A4 >= 0xB6) {
            return 1;
        } else {
            D_8010E7A0 = D_8010E7A0 - 3;
        }
        if (D_8010E7A0 < 0) {
            D_8010E7A0 = 0;
        } else if (D_8010E7A0 >= 0x100) {
            D_8010E7A0 = 0xFF;
        }
        *(int *)0x1F800000 = func_80020074(s5, func_80020074(s5, func_80020074(s5, cur, 0x64, 0xC4, 0x78, 0x20, 0, 0, 0x2C, 0x3FF0, 1, D_8010E7A0), 0xDC, 0xC4, 8, 0x10, 0, 0x20, 0x2C, 0x3FF0, 1, D_8010E7A0), 0x64, 0xEC, 0x7C, 0x18, 0x80, 0, 0x2C, 0x3FB0, 1, D_8010E7A0);
    }
    return 0;
}
