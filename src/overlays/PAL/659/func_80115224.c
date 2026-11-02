#include "rr/overlay659_results_controller.h"

void ovl659_func_80115224(void) {
    s32 group;
    s32 lapOffset;
    s32 rowOffset;
    s32 i;
    s16 timer;
    s32 ready;

    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    switch (D_800F4D8A) {
    case 0:
        group = 0;
        if (D_800FFC14[3].signedBytes[3] >= 16) {
            func_8002AF8C(1, 1, 0);
            if (D_800F3A60 != 0) {
                func_8002AF8C(0, 1, 0);
                func_8004F24C(1);
                if (D_800AD4D8.mode1A == 4) {
                    func_8003A1B4();
                    func_8003A198();
                    D_800F4D8A = 5;
                    break;
                } else {
                    D_800F4D8A = 3;
                }
            }
        }
        lapOffset = 0;
        if (D_800F3914 == 7) {
            if (func_80090424((D_800F2DE4 & 0xFF) << 4) > 0) {
                lapOffset = 3;
            }
        }
        rowOffset = 6;
        if (D_800AD4D8.mode1A != 4) {
            rowOffset = 5;
        }
        for (i = 0; i < 3; i++) {
            if (i >= D_800F3A30) {
                D_800FFC14[rowOffset + 2].state.d34 = -1;
                break;
            }
            if (i + lapOffset == D_800ABEF8[0].steering288.finish20) {
                func_80041320(i + rowOffset, 1, -1);
            } else {
                func_80041320(i + rowOffset, 0, -1);
            }
            func_8002B814((char *)&D_800FFC14[i + rowOffset].bytes[32], i + lapOffset + 1, 0);
            func_8002B934((char *)&D_800FFC14[i + rowOffset].bytes[35], D_800F4C80[i + lapOffset]);
        }
        break;
    case 3:
        group = 1;
        if (D_800AD4D8.retries08[D_800AD4D8.phase17] == 0) {
            D_800F4D8A = 7;
        } else if (D_800FFC14[38].signedBytes[3] >= 14) {
            func_8002AF8C(1, 1, 0);
            func_80041320(D_800AC64A + 37, 0, 0);
            func_8002B330(0, 2, (char *)&D_800AC64A, D_800AC5E0, 0);
            func_80041320(D_800AC64A + 37, 1, 1);
            if (D_800F3A60 != 0) {
                func_8002AF8C(0, 1, 0);
                func_8004F24C(1);
                if (D_800AC64A == 0) {
                    D_800F4D8A = 4;
                    D_800AC5D0 = 64;
                    D_800AD4D8.retries08[D_800AD4D8.phase17]--;
                    func_8002B814((char *)&D_800FFC14[40].bytes[36], D_800AD4D8.retries08[D_800AD4D8.phase17], 0);
                } else {
                    D_800F4D8A = 7;
                }
            }
        }
        break;
    case 4:
        timer = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = timer;
        func_800207D4(&D_800ACC14->head70, (64 - timer) * 4, 69);
        group = 1;
        if (D_800AC5D0 <= 0) {
            D_800F4BE8 = 0;
            D_800F4C6A = 1;
            D_800F3A26 = 35;
            D_800F4D8A = 0;
        }
        break;
    case 5:
        group = 0;
        for (i = 0; i < 11; i++) {
            if (D_800FFC14[i].signedBytes[2] < 8) {
                D_800FFC14[i].signedBytes[2] = 8;
                D_800FFC14[i].signedBytes[0] = 0;
            }
        }
        if (D_800FFC14[0].signedBytes[2] == 9) {
            D_800F4D8A = 6;
            ovl659_D_801165BC = 64;
        }
        break;
    case 6:
        func_8002AF8C(0, 1, 0);
        group = -1;
        if (ovl659_D_801165BC > 0) {
            ovl659_D_801165BC--;
        }
        ready = func_8003B12C();
        if (ready == 1 && ovl659_D_801165BC <= 0) {
            if (D_800AD4D8.phase17 == 7) {
                func_8002F958();
                D_800F4C6A = 0;
                D_800F3A26 = 3;
            } else {
                func_8003A198();
                func_80053BB4(1);
                D_800F4C6A = ready;
                D_800F3A26 = 11;
            }
            D_800F4D8A = 0;
            func_8002A344(0x1000C);
            return;
        }
        break;
    case 7:
        group = 0;
        for (i = 0; i < 11; i++) {
            if (D_800FFC14[i].signedBytes[2] < 8) {
                D_800FFC14[i].signedBytes[2] = 8;
                D_800FFC14[i].signedBytes[0] = 0;
            }
        }
        if (D_800FFC14[0].signedBytes[2] == 9) {
            D_800F4D8A = 8;
        }
        break;
    case 8:
        func_8003A1B4();
        D_800F4C6A = 1;
        D_800F3A26 = 31;
        D_800F4D8A = 0;
        func_8002A344(0x1000C);
        return;
    default:
        break;
    }
    func_8002A344(0x1000C);
    if (ovl659_D_801165BC == -2) {
        func_8003662C(D_800F2DE4);
    } else {
        func_8003662C(ovl659_D_801165BC);
    }
    if (group >= 0) {
        i = 0;
        do {
            func_80040858(i + group * 33);
            i++;
        } while (i < 11);
    }
    if (D_800FFC14[0].signedBytes[2] >= 3 && D_800F4D8A < 6) {
        func_800411D4();
    }
}
