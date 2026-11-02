#include "rr/overlay659.h"

void ovl659_exit_race(void) {
    func_8003CAB4();
    D_800F4C6A = 1;
    D_800F3A26 = 0x2F;
    D_800F4D8A = 0;
}

void ovl659_update_race(void) {
    D_800F2DE4 = (u32)D_800F2DE4 + 1;
    func_8006EF8C(0);
    if (((u32)D_800FF6B0 - 1) < 2U && (D_800F3A38[0].buttons08 & 0x800) && D_800AC5D0 <= 0) {
        D_800AC5D0 = 15;
        D_800F4C68 = (D_800F4C68 == 0);
        if (D_800F4C68 != 0) {
            func_8004FA04();
            D_800F2FDC = 0;
        } else {
            func_800348E8(D_800F4D44);
            func_8004FA50();
        }
    } else if (D_800AC5D0 > 0) {
        if (D_800FF6B0 < 4) {
            D_800AC5D0--;
        }
    }

    if (D_800F4C68 != 0) {
        func_80034844(D_800F4D44);
        D_800F2DE4 = (u32)D_800F2DE4 - 1;
        func_8003BFC8();
        func_800349F4(&D_800ACC14->ordering.ordering70[0], D_800F2FDC);
        func_8002BA28(0);
        func_8002BC7C(D_800AC5A4);
        func_80033F3C(&D_800ACC14->ordering.ordering70[0], D_800F2DE4);
        func_80021350(D_800F4C70.first, 0);
        func_800211A4(0);
        func_80021684((D_800ABEF8[0].input * 160) / 1168, 0);
        func_800219BC(D_800ABEF8[0].half27A, 0);
        func_80021A7C(D_800ABEF8[0].half1EE, 0);
        func_80021E2C(0);
        func_80022098(&D_800ACC14->ordering.ordering70[0], 0);
        func_80021C00(0);
        func_80020B10(D_800F4D44);
        func_8006E1AC();
        func_8002E0D8(0);
        RR_659_SCRATCH->field78 = D_800F2A64;
        func_8006ECE8(0);
        func_800747EC(0);
        func_80036FD4(0);
        func_80070734(D_800ABEB0);
        return;
    }

    D_800ABEB0 = (u32)D_800ABEB0 + 1;
    if (((u32)D_800FF6B0 - 2) < 2U) {
        if ((D_800AC5A4 = (u32)D_800AC5A4 - 1) <= 0) {
            D_800FF6B0 = 5;
            D_800ABEF8[0].half1EE = 9;
            D_800AC5D0 = 37;
            D_800F2A68 = (s32)(0U - (u32)D_800F2EB8);
            func_8004FC18(0, 33);
        }
    }

    if (D_800AC6EC != 0 && D_800FF6B0 == 0) {
        D_800ABEB0 = 75;
        D_800F2DE4 = 75;
        func_8004FED4();
        D_800FF6B0 = 1;
        D_800ABEF8[0].flags1E4 |= 0x100;
        func_800172DC();
        func_80014A8C(-1);
    } else if (D_800FF6B0 == 1) {
        if ((u32)D_800F2DE4 >= 176U) {
            func_80022CD4(D_800ABEF8);
            D_800FF6B0 = 2;
            func_80052A30(1);
        }
    } else if (D_800FF6B0 >= 4) {
        s32 a2;
        if (D_800AC5D0 < 25) {
            if (D_800FFC20[0]->half1EE <= ovl659_finish_rank_limits[D_800AD4D8.phase17 >> 1]) {
                a2 = 37;
            } else {
                a2 = 69;
            }
            func_800207D4(&D_800ACC14->ordering.ordering70[0], (25 - D_800AC5D0) * 10, a2);
        }
        if (D_800AC5D0 <= 0) {
            D_800AD4D8.pad18[1] = D_800ABEF8[0].half1EE;
            if (D_800ABEF8[0].half1EE <= ovl659_finish_rank_limits[D_800AD4D8.phase17 / 2] &&
                D_800FF6B0 == 4) {
                D_800F4C6A = 1;
                D_800F3A26 = 21;
                D_800F4D8A = 0;
            } else {
                func_8003660C();
            }
        } else {
            D_800AC5D0--;
        }
    }

    func_8003C254(D_800ABEF8);
    if (D_800FF6B0 < 4) {
        func_80033F3C(&D_800ACC14->ordering.ordering70[0], D_800F2DE4);
        func_80034630(D_800F2DE4);
    }
    func_8003C96C();
    func_800396E4((char *)D_800ABEF8);
    if (((u32)D_800FF6B0 - 1) < 3U) {
        func_8002BA28(0);
        func_8002BC7C(D_800AC5A4);
        func_80021350(D_800F4C70.first, 0);
        func_800211A4(0);
        func_80021684((D_800ABEF8[0].input * 160) / 1168, 0);
        func_800219BC(D_800ABEF8[0].half27A, 0);
        func_80021A7C(D_800ABEF8[0].half1EE, 0);
        func_80021E2C(0);
        func_80022098(&D_800ACC14->ordering.ordering70[0], 0);
        func_80021C00(0);
        func_80020B10(D_800F4D44);
    }
    func_80037D88(D_800F4D44);
    if (D_800FF6B0 == 0) {
        func_80045868(D_800ABEF8);
    } else if (D_800FF6B0 == 7) {
        func_80033808(&D_800ABEF8[1]);
    } else {
        func_80033BE4((struct w3g2p *)(void *)D_800ABEF8);
    }
    func_800735D8(0);
    func_8006E1AC();
    func_8002E0D8(0);
    if (D_800ABEF8[0].half1EA != D_800AD522) {
        func_800225DC(0);
    }
    RR_659_SCRATCH->field78 = D_800F2A64;
    func_8006ECE8(0);
    func_800747EC(0);
    func_80036FD4(0);
    func_80070734(D_800ABEB0);
    if (D_800FF6B0 == 0) {
        RR_659_SCRATCH->pointers.cursor = func_8001FC28(
            D_800ACC14->ordering.ordering70 + 2, RR_659_SCRATCH->pointers.cursor, 0, 0, 320, 256);
        D_800ADAF0[1].environment.rgb24 = 1;
        D_800ADAF0[0].environment.rgb24 = 1;
        RR_659_SCRATCH->pointers.cursor =
            func_8001FC28(D_800ACC14->ordering.ordering70 + 702, RR_659_SCRATCH->pointers.cursor, 0,
                          D_80107190, 320, (s32)(256U - (u32)D_80107190 * 2U));
        if ((D_80107190 = (s32)((u32)D_80107190 - 16U)) < 0) {
            D_80107190 = 0;
            D_800ADAF0[1].environment.rgb24 = 0;
            D_800ADAF0[0].environment.rgb24 = 0;
        }
    }
    func_8005007C(D_800F4C70.first, D_800F2AB4.first, 0);
    if (D_800FF6B0 >= 4) {
        func_80036030();
        if (D_800FF6B0 == 6) {
            func_80036068();
        }
    }
}
