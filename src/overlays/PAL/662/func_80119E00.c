#include "rr/overlay662_palette_controller.h"

void ovl662_func_80119E00(void) {
    s32 badgePhase;
    s32 ready;
    void *palette;
    s32 paletteIndex;
    s32 index;
    s8 highlight;
    u16 previous;
    s32 rowPhase;
    s16 key;
    RrUiRow24 *acceptRow;
    RrUiRow24 *closingRow;
    RrUiRow24 *cancelRow;

    D_800F2DE4 = (u32)D_800F2DE4 + 1;
    D_800ABEB0 = (u32)D_800ABEB0 + 1;
    switch ((u16)D_800F4D8A) {
    case 0:
        if (ovl662_D_801272E8 == 0) func_8002AF8C(1,1,1);
        else func_8002AF8C(1,1,0);
        previous = ovl662_D_801272BE;
        if (ovl662_D_80127398 >= 0x11) {
            func_8002B330(1, 2, (char *)&ovl662_D_801272BE, 0x19, 0);
        }
        if ((s16) previous != (s16) ovl662_D_801272BE) {
            key = (s8)ovl662_D_801272A0[(s16)ovl662_D_801272BE];
            ovl662_D_801272E2 = key;
            if (key == 0) {
                palette = D_800F3B20;
                paletteIndex = 0;
            } else {
                palette = D_800F3B24;
                paletteIndex = key - 1;
            }
            func_8001E854(palette, paletteIndex, 0);
            ovl662_D_80127398 = 0;
        }
        D_800F4D80[(s16)D_800AC64A].cells[0] = 0;
        func_8002B330(0, 2, (char *)&D_800AC64A, D_800AC5E0, 0);
        highlight = 2;
        if (ovl662_D_801272E2 >= 0x19) {
            D_800F4D80->half20 = 1;
            if ((s16) D_800AC64A == 0) {
                highlight = 4;
            }
        } else {
            D_800F4D80->half20 = 0;
        }
        D_800F4D80[(s16)D_800AC64A].cells[0] = highlight;
        rowPhase = D_800F4D80->byte14;
        if (rowPhase == 2) {
            badgePhase = D_800F3AFC->state10;
            if (badgePhase == 3) {
                if (D_800F3A38[0].a != 0) {
                    D_800F4D80[(s16)D_800AC64A].cells[0] = 1;
                    if ((s16) D_800AC64A == 0) {
                        if (ovl662_D_801272E2 < 0x19) {
                            func_8004F24C(1);
                            index = 0;
                            ovl662_D_801272D4 = (s16) rowPhase;
                            ovl662_D_80127398 = 0;
                            D_800F4D8A = (u16) badgePhase;
                            do {
                                acceptRow = (RrUiRow24 *)(index * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                                if (acceptRow->byte14 == 2) {
                                    acceptRow->byte14 = 3U;
                                }
                                index += 1;
                            } while (index < 2);
                            D_800F3AFC->state10 = 4;
                        } else {
                            func_8004F24C(4);
                        }
                    } else if ((s16) D_800AC64A == 1) {
                        func_8004F24C(1);
                        index = 0;
                        ovl662_D_80127398 = 0;
                        ovl662_D_801272D4 = (s16) rowPhase;
                        D_800F3AFC->state10 = 4;
                        do {
                            closingRow = (RrUiRow24 *)(index * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                            if (closingRow->byte14 == 2) {
                                closingRow->byte14 = 3U;
                            }
                            index += 1;
                        } while (index < 2);
                        goto closeMenu;
                    }
                } else if (D_800F3A38[0].b != 0) {
                    if (ovl662_D_801272E8 == 0) {
                        func_8004F24C(2);
                        index = 0;
                        ovl662_D_801272D4 = (s16) rowPhase;
                        ovl662_D_80127398 = 0;
                        D_800F3AFC->state10 = 4;
                        do {
                            cancelRow = (RrUiRow24 *)(index * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                            if (cancelRow->byte14 == 2) {
                                cancelRow->byte14 = 3U;
                            }
                            index += 1;
                        } while (index < 2);
closeMenu:
                        D_800F4D8A = 4;
                    }
                }
            }
        }
        goto drawMenu;
    case 3:
        if (ovl662_D_801272E8 == 0) func_8002AF8C(0,1,1);
        else func_8002AF8C(0,1,0);
        if ((D_800F4D80->byte14 == 5) && (D_800F3AFC->state10 == 11)) {
            D_800ACBD0[0].second = (u16)ovl662_D_801272E2+1;
            if (ovl662_D_801272E8 == 0) goto enterPalette;
            goto publishScene;
        }
        goto drawMenu;
    case 4:
        if (ovl662_D_801272E8 == 0) func_8002AF8C(0,1,1);
        else func_8002AF8C(0,1,0);
        if ((D_800F4D80->byte14 == 5) && (D_800F3AFC->state10 == 11)) {
            if (ovl662_D_801272E8 != 0) goto publishScene;
enterPalette:
            ovl662_func_80118ED8();
            break;
publishScene:
            D_800F4C6A=3;D_800F3A26=1;D_800F4D8A=0;
            break;
        }
        goto drawMenu;
    default:
drawMenu:
        if (ovl662_D_801272D4 == 0) {
            ready = func_8003DCEC(0);
            if (ready == 1) {
                ovl662_D_801272D4 = ready;
            }
        } else {
            ovl662_func_80119898(ovl662_D_801272D4 - 1);
            func_8002B12C(0xA0, 0xC0, 0x30, 0, (s32) (s16) ovl662_D_801272BE, 0, (s32) ovl662_D_8012739A);
        }
        index = 0;
        if (D_800AC5E0 > 0) {
            do {
                func_8003EC88(index);
                index += 1;
            } while (index < D_800AC5E0);
        }
        func_8003D6F0(0);
        break;
    }
    func_8002A344(0x1004A);
}
