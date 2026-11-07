#include "rr/menu_text_rows.h"

extern s32 D_800F2DE4, D_800F4C64, D_800F3A60, D_800AC5D4;
extern u16 D_800F4D8A;
extern s16 D_800F4C6A, D_800F3A26, D_800AC5D0, D_800AC64A, D_800AD508;
extern s16 D_800FF688, D_8010C068, D_80107178;
extern s16 D_8010C070[4];
extern s16 D_8010C080[4];
extern char D_8010C088[4][12];
extern u8 *D_800ACC14;

s32 func_80034FC0(void);
s32 func_80035024(s32);
void func_80034F80(void);
void func_8002AF8C(s32, s32, s32);
void func_8002B75C(char *, char *);
void func_8002B7B4(char *, char *, s32);
void func_8002B814(char *, s32, s32);
void func_800207D4(u32 *, s32, u32);
void ResetScreenCounters(void);
void func_8003A1B4(void);
void func_8002A344(s32);
void func_8003662C(s32);
void func_80040858(s32);
void func_800411D4(void);
s32 func_8003B12C(void);
void func_80053BB4(s32);
s32 ovl667_func_801187DC(s32);
void ovl667_func_801161A8(s32);

void ovl667_func_8011885C(void) {
    s32 pressed;
    s32 group;
    s32 i;
    s32 result;
    s32 player;

    D_800F2DE4++;
    if (func_80034FC0() != 0) {
        return;
    }
    pressed = func_80035024(2);
    if ((u32)D_800F2DE4 >= 0x08000000) {
        D_800F2DE4 = 3;
    }
    switch (D_800F4D8A) {
    case 0:
        if (D_800F4C64 == -1) {
            group = 1;
            if (MenuTextRows[11].signedBytes[2] >= 2) {
                D_800F4D8A = 3;
            }
            break;
        }
        group = 0;
        if (MenuTextRows[3].signedBytes[3] >= 16) {
            func_8002AF8C(1, 1, group);
            if (D_800F3A60 != 0) {
                D_800F4D8A = 3;
                func_8002AF8C(group, 1, group);
            }
        }
        func_8002B814(MenuTextRows[4].marquee.at28 + 8, D_8010C070[0] + 1, 0);
        func_8002B7B4(MenuTextRows[5].marquee.at28 + 1, D_8010C088[D_8010C070[0]], 9);
        /* The original literal occupies 12 bytes (zero word before "WIN "). */
        func_8002B75C(MenuTextRows[5].marquee.at28 + 10, "WON !!!\0\0\0");
        for (i = 0; i < 4; i++) {
            if (i < D_80107178) {
                player = D_8010C070[i];
                func_8002B7B4(MenuTextRows[7 + i].marquee.at28 + 5, D_8010C088[player], 9);
                func_8002B75C(MenuTextRows[7 + i].marquee.at28 + 14, "WIN ");
                func_8002B814(MenuTextRows[7 + i].marquee.at28 + 18, D_8010C080[player], 1);
            } else {
                MenuTextRows[7 + i].marquee.at28[0] = 0;
            }
        }
        break;
    case 3:
        group = 3;
        if (MenuTextRows[39].signedBytes[3] >= 17) {
            func_8002AF8C(1, 1, 0);
            if (ovl667_func_801187DC(0) >= 0) {
                ResetScreenCounters();
                func_8003A1B4();
                D_800AC5D0 = 64;
                D_800F4D8A = D_800AC64A + 5;
                func_80034F80();
                func_8002AF8C(0, 1, 0);
            }
        }
        break;
    case 4:
        group = -1;
        if (pressed == 0 && D_800AC5D0 == 64) {
            break;
        }
        D_800AC5D0--;
        func_800207D4((u32 *)(D_800ACC14 + 0x70), (64 - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            D_8010C068 = 1;
            D_800F4C6A = 1;
            D_800F3A26 = 41;
            D_800F4D8A = 0;
            D_800FF688 = 0;
        }
        break;
    case 5:
        group = 3;
        if (pressed == 0 && D_800AC5D0 == 64) {
            func_8002B75C(MenuTextRows[38].marquee.at28, "*PLEASE WAIT! ");
            break;
        }
        D_800AC5D0 -= 2;
        func_800207D4((u32 *)(D_800ACC14 + 0x70), (64 - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            group = -1;
            D_800F4D8A = 8;
        }
        break;
    case 6:
        group = -1;
        D_800AC5D0--;
        func_800207D4((u32 *)(D_800ACC14 + 0x70), (64 - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            D_800F4C6A = 1;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        }
        break;
    case 8:
        result = func_8003B12C();
        if (result != 1) {
            return;
        }
        func_80053BB4(0);
        D_800FF688 = 0;
        D_800AC5D4 = (result - D_800AD508) * 2;
        ovl667_func_801161A8(D_800AC5D4);
        D_800F4C6A = result;
        D_800F3A26 = 60;
        D_800F4D8A = 0;
        return;
    }
    func_8002A344(0x1000C);
    func_8003662C(D_800F2DE4);
    if (group >= 0) {
        for (i = 0; i < 11; i++) {
            func_80040858(i + group * 11);
        }
    }
    if ((u8)(MenuTextRows[0].bytes[2] - 3) < 5 || (u8)(MenuTextRows[11].bytes[2] - 3) < 5) {
        func_800411D4();
    }
}
