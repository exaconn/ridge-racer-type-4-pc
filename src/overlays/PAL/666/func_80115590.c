#include "rr/overlay666_text_controller.h"

void ovl666_func_80115590(void) {
    s32 group;
    s32 i;
    s16 timer;
    s32 ready;
    RrMenuTextRow60 *rows;

    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    D_800ABEB0++;
    switch (D_800F4D8A) {
    case 0:
        if (D_800F4C64 == -1) {
            group = 1;
            if (D_800FFC14[11].signedBytes[2] >= 2) {
                D_800F4D8A = 3;
            }
        } else {
            group = 0;
            if (D_800FFC14[5].signedBytes[3] >= 21) {
                func_8002AF8C(1, 1, 0);
                if (D_800F3A60 != 0) {
                    func_8004F24C(1);
                    D_800F4D8A = 3;
                    func_8002AF8C(0, 1, 0);
                }
            }
            if (ovl666_D_80116F94 == 0) {
                D_800FFC14[7].marquee.at28[(D_800ABEB0 >> 2) % 22U] = 0x20;
                D_800FFC14[7].marquee.at28[((D_800ABEB0 >> 2) + 1U) % 22U] = 0x5B;
                D_800FFC14[7].marquee.at28[((D_800ABEB0 >> 2) + 2U) % 22U] = 0x5D;
                D_800FFC14[7].marquee.at4[((D_800ABEB0 >> 2) + 2U) % 22U] = 0x40;
            }
        }
        break;
    case 3:
        group = 3;
        if (D_800FFC14[38].signedBytes[3] >= 12) {
            func_8002AF8C(1, 1, 0);
            if (ovl666_func_80115510(0) >= 0) {
                func_8003A198();
                func_8003A1B4();
                D_800AC5D0 = 64;
                D_800F4D8A = D_800AC64A + 4;
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
            ovl666_D_80116F94 = 1;
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
            ovl666_func_801148D4();
            D_800F4C6A = ready;
            D_800F3A26 = 79;
            D_800F4D8A = 0;
        }
        return;
    default:
        break;
    }
    func_8002A344(0x1000C);
    func_8003662C(D_800F2DE4);
    if (group >= 0) {
        i = 0;
        do {
            func_80040858(i + group * 11);
            i++;
        } while (i < 11);
    }
    rows = D_800FFC14;
    if ((u32)(rows[0].bytes[2] - 3) < 5U || (u32)(rows[11].bytes[2] - 3) < 5U) {
        func_800411D4();
    }
}
