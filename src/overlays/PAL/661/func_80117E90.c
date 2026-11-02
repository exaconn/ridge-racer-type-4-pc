#include "rr/menu_text_rows.h"

extern s32 D_800F2DE4, D_800F4C64, D_800F3A60;
extern u16 D_800F4D8A;
extern s16 D_800F4C6A, D_800F3A26, D_800AC5D0, D_800AC64A, D_800F4BE8;
extern s16 ovl661_D_80118410[2];
extern s16 ovl661_D_80118418[2];
extern char ovl661_D_80118420[2][12];
extern s16 ovl661_D_80118448[2];
extern char ovl661_D_801148EC[];
extern char ovl661_D_801148F8[];
extern u8 *D_800ACC14;

void func_8002AF8C(s32, s32, s32);
void func_8004F24C(s32);
void func_8002B75C(char *, char *);
void func_8002B7B4(char *, char *, s32);
void func_8002B814(char *, s32, s32);
void func_800207D4(u32 *, s32, u32);
void func_8003A198(void);
void func_8003A1B4(void);
void func_8002A344(s32);
void func_8003662C(s32);
void func_80040858(s32);
void func_800411D4(void);
s32 func_8003B12C(void);
void func_80053BB4(s32);
s32 ovl661_func_80117E10(s32);
void ovl661_func_80115598(s32);

void ovl661_func_80117E90(void) {
    s32 group;
    s32 i;
    s32 player;
    s32 ready;

    D_800F2DE4++;
    switch (D_800F4D8A) {
    case 0:
        if (D_800F4C64 == -1) {
            group = 1;
            if (D_800FFC14[11].signedBytes[2] >= 2) {
                D_800F4D8A = 3;
            }
            break;
        }
        group = 0;
        if (D_800FFC14[3].signedBytes[3] >= 17) {
            func_8002AF8C(1, 1, group);
            if (D_800F3A60 != 0) {
                func_8004F24C(1);
                D_800F4D8A = 3;
                func_8002AF8C(group, 1, group);
            }
        }
        func_8002B814(D_800FFC14[4].marquee.at28 + 8, ovl661_D_80118410[0] + 1, 0);
        func_8002B7B4(D_800FFC14[5].marquee.at28 + 1, ovl661_D_80118420[ovl661_D_80118410[0]], 9);
        func_8002B75C(D_800FFC14[5].marquee.at28 + 10, ovl661_D_801148EC);
        for (i = 0; i < 2; i++) {
            player = ovl661_D_80118410[i];
            func_8002B7B4(D_800FFC14[7 + i].marquee.at28 + 5, ovl661_D_80118420[player], 9);
            func_8002B75C(D_800FFC14[7 + i].marquee.at28 + 14, ovl661_D_801148F8);
            func_8002B814(D_800FFC14[7 + i].marquee.at28 + 18, ovl661_D_80118418[player], 1);
        }
        break;
    case 3:
        group = 3;
        if (D_800FFC14[39].signedBytes[3] >= 17) {
            func_8002AF8C(1, 1, 0);
            if (ovl661_func_80117E10(0) >= 0) {
                func_8003A198();
                func_8003A1B4();
                D_800AC5D0 = 64;
                D_800F4D8A = D_800AC64A + 4;
                func_8002AF8C(0, 1, 0);
            }
        }
        break;
    case 4:
        D_800AC5D0--;
        func_800207D4((u32 *)(D_800ACC14 + 0x70), (64 - D_800AC5D0) * 4, 69);
        group = 3;
        if (D_800AC5D0 <= 0) {
            D_800F4BE8 = 0;
            ovl661_D_80118448[0] = -1;
            D_800F4C6A = 1;
            D_800F3A26 = 35;
            D_800F4D8A = 0;
        }
        break;
    case 5:
        D_800AC5D0 -= 2;
        func_800207D4((u32 *)(D_800ACC14 + 0x70), (64 - D_800AC5D0) * 4, 69);
        group = 3;
        if (D_800AC5D0 <= 0) {
            group = -1;
            D_800F4D8A = 8;
        }
        break;
    case 6:
        D_800AC5D0--;
        func_800207D4((u32 *)(D_800ACC14 + 0x70), (64 - D_800AC5D0) * 4, 69);
        group = -1;
        if (D_800AC5D0 <= 0) {
            D_800F4C6A = 1;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        }
        break;
    case 8:
        ready = func_8003B12C();
        if (ready == 1) {
            func_80053BB4(0);
            ovl661_func_80115598(0);
            D_800F4C6A = ready;
            D_800F3A26 = 56;
            D_800F4D8A = 0;
        }
        return;
    }
    func_8002A344(0x1000C);
    func_8003662C(D_800F2DE4);
    if (group >= 0) {
        for (i = 0; i < 11; i++) {
            func_80040858(i + group * 11);
        }
    }
    if ((u8)(D_800FFC14[0].bytes[2] - 3) < 5 || (u8)(D_800FFC14[11].bytes[2] - 3) < 5) {
        func_800411D4();
    }
}
