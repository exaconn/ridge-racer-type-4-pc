#include "rr/overlay666.h"

void ovl666_init_results(void) {
    s32 i;
    s32 base;

    /* The selected label bank is0..2; the two progress selectors are0..3. */
    InitMenuTextRows(D_800FFC48);
    MenuCursor = 0;
    MenuItemCount = 3;
    if (ovl666_result_kind == 0) {
        D_800F3920.prefix.flags5C[D_800F4D98.choice.bytes[1]] = 1;
        D_800F3920.prefix.progress
            .progress10[D_800F4D98.choice.bytes[0]][D_800F4D98.choice.bytes[1]] |= 0x80000;
    }
    base = ovl666_result_kind * 11;
    for (i = 0; i < 11; i++) {
        SetMenuTextRow(i, i, 0, 0, ovl666_result_labels[i + base]);
    }
    for (i = 11; i < 22; i++) {
        SetMenuTextRow(i, i - 11, 0, 0, -1);
    }
    for (i = 33; i < 44; i++) {
        SetMenuTextRow(i, i - 33, 2, 0, ovl666_result_labels[i - 11]);
    }
    func_8002AF8C(0, 1, 0);
    D_800F2DE4 = 0;
    ScreenFrame = 0;
}

s32 ovl666_update_retry_choice(s32 mode) {
    func_80041320(MenuCursor + 37, 0, 0);
    StepMenuValue(0, 2, (char *)&MenuCursor, 3, 0);
    func_80041320(MenuCursor + 37, 1, 1);
    if (D_800F3A60) {
        PlaySfx(1);
        return MenuCursor;
    }
    return -1;
}

void ovl666_update_results(void) {
    s32 group;
    s32 i;
    s16 timer;
    s32 ready;
    RrMenuTextRow60 *rows;

    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    ScreenFrame++;
    switch (D_800F4D8A) {
    case 0:
        if (D_800F4C64 == -1) {
            group = 1;
            if (MenuTextRows[11].signedBytes[2] >= 2) {
                D_800F4D8A = 3;
            }
        } else {
            group = 0;
            if (MenuTextRows[5].signedBytes[3] >= 21) {
                func_8002AF8C(1, 1, 0);
                if (D_800F3A60 != 0) {
                    PlaySfx(1);
                    D_800F4D8A = 3;
                    func_8002AF8C(0, 1, 0);
                }
            }
            if (ovl666_result_kind == 0) {
                MenuTextRows[7].marquee.at28[(ScreenFrame >> 2) % 22U] = 0x20;
                MenuTextRows[7].marquee.at28[((ScreenFrame >> 2) + 1U) % 22U] = 0x5B;
                MenuTextRows[7].marquee.at28[((ScreenFrame >> 2) + 2U) % 22U] = 0x5D;
                MenuTextRows[7].marquee.at4[((ScreenFrame >> 2) + 2U) % 22U] = 0x40;
            }
        }
        break;
    case 3:
        group = 3;
        if (MenuTextRows[38].signedBytes[3] >= 12) {
            func_8002AF8C(1, 1, 0);
            if (ovl666_update_retry_choice(0) >= 0) {
                ResetScreenCounters();
                InitMenuArenas();
                D_800AC5D0 = 64;
                D_800F4D8A = MenuCursor + 4;
            }
        }
        break;
    case 4:
        func_8002AF8C(0, 1, 0);
        timer = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = timer;
        func_800207D4(D_800ACC14->ordering.ordering70, (64 - timer) * 4, 69);
        group = 3;
        if (D_800AC5D0 <= 0) {
            ovl666_result_kind = 1;
            D_800F4BE8 = 0;
            D_800F4C6A = 1;
            D_800F3A26 = 35;
            D_800F4D8A = 0;
        }
        break;
    case 5:
        func_8002AF8C(0, 1, 0);
        timer = (u16)D_800AC5D0 - 2;
        D_800AC5D0 = timer;
        func_800207D4(D_800ACC14->ordering.ordering70, (64 - timer) * 4, 69);
        group = 3;
        if (D_800AC5D0 <= 0) {
            group = -1;
            D_800F4D8A = 8;
        }
        break;
    case 6:
        func_8002AF8C(0, 1, 0);
        timer = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = timer;
        func_800207D4(D_800ACC14->ordering.ordering70, (64 - timer) * 4, 69);
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
            ovl666_init_menu();
            D_800F4C6A = ready;
            D_800F3A26 = 79;
            D_800F4D8A = 0;
        }
        return;
    default:
        break;
    }
    UpdateScreenFade(0x1000C);
    func_8003662C(D_800F2DE4);
    if (group >= 0) {
        i = 0;
        do {
            func_80040858(i + group * 11);
            i++;
        } while (i < 11);
    }
    rows = MenuTextRows;
    if ((u32)(rows[0].bytes[2] - 3) < 5U || (u32)(rows[11].bytes[2] - 3) < 5U) {
        func_800411D4();
    }
}
