#include "rr/overlay666_race_controller.h"

void ovl666_func_80116730(void) {
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
        func_800349F4(&D_800ACC14->head70, D_800F2FDC);
        if (D_800FF6B0 < 4) {
            func_80033F3C(&D_800ACC14->head70, D_800F2DE4);
        }
        if (((u32)D_800FF6B0 - 1) < 3U) {
            func_80021350(D_800F4C70.first, 0);
            func_80021684((D_800ABEF8[0].input * 160) / 1168, 0);
            func_800219BC(D_800ABEF8[0].half27A, 0);
            func_80021A7C(D_800ABEF8[0].half1EE, 0);
            func_8002BD18(D_800AC5A4, 0);
            func_80021C94(0, D_800ABEF8[0].steering288.count22);
            func_80021E2C(0);
            func_80022098(&D_800ACC14->head70, 0);
            func_80021C00(1);
            func_800211A4(0);
            func_80020B10(4);
        }
        if (D_800FF6B0 == 0 && D_800F2DE4 < 75U) {
            func_80033474(D_800ABEF8, D_800F2DE4);
        } else {
            func_80014A8C(-1);
            func_80033BE4((struct w3g2p *)(void *)D_800ABEF8);
        }
        func_8006E1AC();
        func_8002E0D8(0);
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
            D_800AC5D0 = 75;
            func_8004FC18(0, 33);
        }
    }

    if (D_800F2DE4 >= 75U && D_800FF6B0 == 0) {
        D_800FF6B0 = 1;
        func_8004FED4();
        func_800172DC();
        func_80014A8C(-1);
    } else if (D_800FF6B0 == 1) {
        if (D_800F2DE4 >= 176U) {
            func_80022CD4(D_800ABEF8);
            D_800FF6B0 = 2;
            func_80052A30(1);
        }
    } else if (D_800FF6B0 >= 4) {
        s32 a2;
        if (D_800F4C64 == -1 || D_800ABEF8[0].half1EE != 0) {
            a2 = 69;
        } else {
            a2 = 37;
        }
        if (D_800AC5D0 < 25) {
            func_800207D4(&D_800ACC14->head70, (25 - D_800AC5D0) * 10, a2);
            RR_666_SCRATCH->cursor = func_8001FC28(&D_800ACC14->head70, RR_666_SCRATCH->cursor, 0, 0, 320, 256);
        }
        if (D_800AC5D0 <= 0) {
            if (D_800FF6B0 == 4 && (ovl666_D_80116F94 = D_800ABEF8[0].half1EE) == 0) {
                D_800F4C6A = 1;
                D_800F3A26 = 21;
                D_800F4D8A = 0;
            } else {
                func_8003660C();
            }
        } else {
            D_800AC5D0--;
        }
    } else if (D_800FF6B0 == 0) {
        if (D_800F2DE4 >= 51U) {
            func_800207D4(&D_800ACC14->head70, (D_800F2DE4 - 50) * 10, 37);
            D_800ADAF0[1].environment.rgb24 = 0;
            D_800ADAF0[0].environment.rgb24 = 0;
        } else if (D_800F2DE4 < 25U) {
            func_800207D4(&D_800ACC14->head70, (25 - D_800F2DE4) * 10, 69);
            D_800ADAF0[1].environment.rgb24 = 1;
            D_800ADAF0[0].environment.rgb24 = 1;
        }
        RR_666_SCRATCH->cursor = func_8001FC28(&D_800ACC14->head70, RR_666_SCRATCH->cursor, 0, 0, 320, 256);
    }

    ovl666_func_8011656C((RrRaceCheckpointVehicle *)(void *)D_800ABEF8);
    func_8003C96C();
    if (D_800FF6B0 < 4) {
        func_80033F3C(&D_800ACC14->head70, D_800F2DE4);
        func_80034630(D_800F2DE4);
    }
    func_800396E4((char *)D_800ABEF8);
    func_80037D88(D_800F4D44);
    if (((u32)D_800FF6B0 - 1) < 3U) {
        func_80021350(D_800F4C70.first, 0);
        func_80021684((D_800ABEF8[0].input * 160) / 1168, 0);
        func_800219BC(D_800ABEF8[0].half27A, 0);
        func_80021A7C(D_800ABEF8[0].half1EE, 0);
        func_8002BD18(D_800AC5A4, 0);
        func_80021C94(0, D_800ABEF8[0].steering288.count22);
        func_80021E2C(0);
        func_80022098(&D_800ACC14->head70, 0);
        func_80021C00(1);
        func_800211A4(0);
        func_80020B10(4);
    }
    if (D_800FF6B0 == 0 && D_800F2DE4 < 75U) {
        func_80033474(D_800ABEF8, D_800F2DE4);
    } else {
        func_80033BE4((struct w3g2p *)(void *)D_800ABEF8);
    }
    func_8006E1AC();
    func_8002E0D8(0);
    if (D_800ABEF8[0].half1EA != 0) {
        func_800225DC(0);
    }
    func_8006ECE8(0);
    func_800735D8(0);
    func_800747EC(0);
    func_80036FD4(0);
    func_80070734(D_800ABEB0);
    func_8005007C(D_800F4C70.first, D_800F2AB4.first, 0);
    if (D_800FF6B0 >= 4) {
        func_80036030();
        if (D_800FF6B0 == 6) {
            func_80036068();
        }
    }
}
