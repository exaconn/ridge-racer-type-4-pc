#include "types.h"
#include "m2c_macros.h"

extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_8004ED38();
extern int D_800A021C;
extern int D_800A0228[];
extern char *D_8010C1F0;
extern int D_8010C210[2];
extern int func_8004DDF8();
extern int func_8004DEBC();
extern int D_800AC650[];
extern int D_800ACBBC;
extern int D_800ACC18;

int func_8004E03C(char *a0);
s32 func_8004E184(void);
s32 func_8004E18C(void);
int func_8004E194(void);

int func_8004E03C(char *a0) {
    int v;
    int *t = D_800A0228;
    switch (D_800A021C) {
    case 0:
        v = func_8001292C(func_8001223C(2, 0x11), a0);
        if (v != 0) {
            D_800A021C += 1;
    case 1:
            if (func_80012A58() != 0) {
                D_800A021C += 1;
            }
            v = 0;
        }
        return v;
    case 2:
        v = func_8001292C(func_8001223C(2, 0x12), D_8010C1F0);
        if (v != 0) {
            D_800A021C += 1;
    case 3:
            if (func_80012A58() != 0) {
                D_8010C210[0] = (int)a0;
                D_8010C210[1] = (int)D_8010C1F0;
                func_8004ED38(D_8010C210, t[1], t[2]);
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

s32 func_8004E184(void) {
    return 1;
}

s32 func_8004E18C(void) {
    return 1;
}

int func_8004E194(void) {
    switch (D_800A021C) {
    case 0:
        if (func_8004DEBC(3, 0) != 0) {
            D_800A021C += 1;
        }
        return 0;
    case 1:
        if (func_8004DEBC(D_800ACBBC, 0) != 0) {
            D_800A021C += 1;
        }
        return 0;
    case 2:
        if (func_8004DDF8(D_800AC650[0x570 / 4], D_800AC650[0x574 / 4], D_800AC650[0x578 / 4]) != 0) {
            D_800A021C = 0;
            D_800ACC18 = D_8010C1F0;
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}
