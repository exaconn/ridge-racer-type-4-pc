#include "rr/overlay666_menu_controller.h"

void ovl666_func_80114CCC(void) {
    s32 sound;
    register s32 selectedOne __asm__("$2");
    s16 glyphIndex;
    RrMenuGlyphRow1C *glyphBase;
    s32 selected;
    s32 ready;
    s16 timer;
    s32 glyphCount;
    s32 glyphStep;
    register s32 choiceRow __asm__("$4");
    register s32 cancelRow __asm__("$4");
    s32 choiceIndex;
    s32 cancelIndex;
    s32 closing;
    s8 cellState;
    RrMenuGlyphRow1C *glyph;
    RrUiRow24 *choice;
    RrUiRow24 *cancel;
    register RrMenuGlyphRow1C *glyphCursor __asm__("$3");

    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    D_800ABEB0 = (s32)((u32)D_800ABEB0 + 1U);
    ready = func_8003B47C();
    if (ready == 1) {
        switch (D_800F4D8A) {
            case 0:
                glyphCount = 0;
                glyphStep=0x30000;
                glyphCursor = D_800F4D8C;
                do {
                    glyphCount += 1;
                    glyphCursor->phase18 = (s32)((u32)glyphCursor->phase18 + (u32)glyphStep);
                    glyphCursor++;
                } while (glyphCount < 4);
                D_800F4D8A = 1;
                goto title;
        case 1:
            func_8002AF8C(1, 1, 1);
            D_800F4D80[D_800AC64A].cells[0] = 0;
            func_8002B330(0, 2, (char *)&D_800AC64A, 2, 0);
            cellState = 2;
            if (ovl666_D_80116F88[D_800F3826] <= 0) {
                D_800F4D80[0].half20 = ready;
                if (D_800AC64A == 0) {
                    cellState = 4;
                }
            } else {
                D_800F4D80[0].half20 = 0;
            }
            D_800F4D80[D_800AC64A].cells[0] = cellState;
            ovl666_func_80115148();
            closing = 0;
            if (D_800F3A38[0].a != 0) {
                selected = D_800AC64A;
                if (selected == 0) {
                    if (ovl666_D_80116F88[D_800F3826] > 0) {
                        closing = 1;
                        D_800F4D8A = 3;
                        func_8004F24C(1);
                        choiceRow = 0;
                        choiceIndex = 0;
                        do {
                            choice = (RrUiRow24 *)((unsigned long)(choiceIndex*sizeof(RrUiRow24))+(unsigned long)D_800F4D80);
                            if (choice->byte14 == 2) {
                                choice->byte14 = 3U;
                            }
                            choiceRow += 1;
                            choiceIndex++;
                        } while (choiceRow < 3);
                    } else {
                        func_8004F24C(4);
                        /* Keep rejection separate from the cancellation call. */
                    __asm__("" : "+r"(closing));
                    }
                } else {
                    selectedOne=1;
                    if (selected != selectedOne) goto decision;
                    /* Retain the initialized comparison value after the branch. */
                    __asm__("" : "+r"(selectedOne));
                    sound = selectedOne;
                    goto play_cancel;
                }
            } else {
                sound = 2;
                if (D_800F3A38[0].b != 0) {
play_cancel:
                    closing = 2;
                    func_8004F24C(sound);
                }
            }
decision:
            if (closing > 0) {
                if (closing == 2) {
                    cancelRow = 0;
                    cancelIndex = 0;
                    do {
                        cancel = (RrUiRow24 *)((unsigned long)(cancelIndex*sizeof(RrUiRow24))+(unsigned long)D_800F4D80);
                        if (cancel->byte14 == 2) {
                            cancel->byte14 = 3U;
                        }
                        cancelRow += 1;
                        cancelIndex++;
                    } while (cancelRow < 3);
                    D_800F4D8A = 8;
                    D_800AC5D0 = 0x40;
                    func_80053AF4(0, 0x3C);
                }
            }
            goto title;
            case 3:
                func_8002AF8C(0, 1, 1);
                if (D_800F4D80[0].byte14 == 5) {
                    ovl666_func_80115A88();
                    D_800F4C6A = ready;
                    D_800F3A26 = 0x50;
                    D_800F4D8A = 0;
                } else {
                    goto title;
                }
                break;
            case 8:
                func_8002AF8C(0, 1, 1);
                timer = (u16)D_800AC5D0 - 1;
                D_800AC5D0 = (u16) timer;
                func_800207D4(&D_800ACC14->head70, (0x40 - timer) * 4, 0x45);
                if ((s16) D_800AC5D0 <= 0) {
                    D_800F4C6A = ready;
                    D_800F3A26 = 3;
                    D_800F4D8A = 0;
                    func_80053CB0();
                }
                goto update_rows;
        default:
title:
            func_8002B12C(0xA0, 0xCC, 0x30, 0, (s32) D_800F3826, 0, 4);
            glyphIndex = D_800F3826;
            if ((u8)D_800F3920.prefix.flags5C[glyphIndex] != 0) {
                glyphBase=D_800F4D8C;
                glyph = (RrMenuGlyphRow1C *)((unsigned long)(long)(glyphIndex*(s32)sizeof(RrMenuGlyphRow1C))+(unsigned long)glyphBase);
                ovl666_func_801152B0(glyph->x08 + 0x14, glyph->y0A + 0x28);
                func_8003E13C(D_800F3826);
            } else {func_8003E13C(glyphIndex);}
            goto update_rows;
        }
    } else {
update_rows:
        func_8003EC88(0);
        func_8003EC88(1);
        func_8003EC88(2);
    }
    func_8002A344(0x10092);
}
