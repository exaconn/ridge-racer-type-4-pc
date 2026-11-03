/* RODATA 0x800102cc 0x24 */
#include "types.h"
#include "m2c_macros.h"

extern s16 D_800AC5D0;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;
extern s32 D_8010BF84;
extern short *D_8010BF80;
extern int D_800ACC18;
extern int D_800ACC14;
extern int D_800F3A60;
extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_80052800();
extern int func_800528A8();
extern int func_80052A30();
extern int func_800436E8();
extern int func_8002AF8C();
extern int func_800207D4();
extern int func_80052B8C();
extern int func_8001B754();
extern int func_8001BAF8();
extern int func_8001E1F0();
extern int func_8002A344();

void func_8001E344(void);
void func_8001E378(void);
void func_8001E390(void);

void func_8001E344(void) {
    D_8010BF84 = 0;
    D_800AC5D0 = -1;
    D_800F4C6A = 0;
    D_800F3A26 = 6;
    D_800F4D8A = 0;
}

void func_8001E378(void) {
    D_8010BF84 = 0;
    D_800AC5D0 = -1;
}

void func_8001E390(void) {
    short *p;
    short *q;
    short *r;
    volatile int *g;
    int *s_;
    int *d_;
    int v;

    switch (D_8010BF84) {
    case 0:
        v = func_8001223C(0, 0x15);
        if (func_8001292C(v, D_800ACC18)) {
            D_8010BF84 = 1;
        }
        return;
    case 1:
        if (func_80012A58()) {
            D_8010BF84 = 2;
        }
        return;
    case 2:
        func_80052800();
        func_800528A8(0, 0x24);
        func_80052A30(0);
        func_800436E8(D_800ACC18);
        D_8010BF84 = 7;
        q = (short *)D_800ACC18;
        D_8010BF80 = q;
        D_800ACC18 = (int)q + 0x10;
        q[1] = 0;
        q[2] = 0;
        q[3] = 0;
        q[4] = 0;
        q[5] = 0;
        q[6] = 8;
        q[7] = 0;
        q[0] = 0;
    case 7:
        r = D_8010BF80;
        if (r[0] < 0x40) {
            if (r[0] == 8) {
                r[4] = 4;
            } else if (r[0] == 0x20) {
                r[5] = 4;
            }
            goto common;
        }
        D_8010BF84 = 8;
    case 8:
        func_8002AF8C(1, 1, 0);
        if (D_800AC5D0 > 0) {
            D_800AC5D0--;
            func_800207D4(D_800ACC14 + 0x70, (0x40 - D_800AC5D0) * 4, 0x45);
            if (D_800AC5D0 == 0) {
                D_800F4C6A = 1;
                D_800F3A26 = 3;
                D_800F4D8A = 0;
            }
        } else if (D_800F3A60) {
            D_800AC5D0 = 0x40;
            func_80052B8C(0, 0x40);
        }
    case 3: case 4: case 5: case 6:
    default:
    common:
        p = D_8010BF80;
        func_8001B754(p + 1, p + 4, 0x80);
        p = D_8010BF80;
        func_8001B754(p + 2, p + 5, 0x80);
        p = D_8010BF80;
        func_8001B754(p + 3, p + 6, 0x80);
        func_8001BAF8(D_8010BF80[1], 0x2F);
        func_8001BAF8(D_8010BF80[2], 0x30);
        p = D_8010BF80;
        func_8001E1F0(p[3]);
        g = (int *)0x1F800000;
        *(int *)(g[0] + 4) = 0xE100022A;
        s_ = (int *)g[1];
        d_ = (int *)g[0];
        *d_ = *s_;
        *(char *)(g[0] + 3) = 1;
        *(int *)g[1] = g[0];
        *(char *)(g[1] + 3) = 0;
        *(int *)0x1F800000 += 8;
        func_8002A344(0x1000C);
        D_8010BF80[0]++;
    }
}
