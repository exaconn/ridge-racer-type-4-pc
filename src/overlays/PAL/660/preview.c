#include "rr/overlay660.h"

void ovl660_func_801163C4(void)
{
    RrRaceVehicle *state;
    RrProjectionScratchPrefix *scratch;

    D_800F2DE4 = 0;
    D_800ABEB0 = 0;
    func_80013890(5);
    D_800AC64A = 0;
    D_800AC5E0 = 3;
    func_8003A198();
    func_80037110();
    state = D_800ABEF8;
    scratch = RR_PROJECTION_SCRATCH;
    scratch->word18 = 120;
    scratch->word08 = -16;
    scratch->word0C = -248;
    scratch->word10 = -96;
    scratch->word1C = 0;
    scratch->word20 = 0;
    D_800ACC18 = D_800F4D40;
    func_8002E4F4(state);
    state->pos20[1] = 10;
    state->pos20[2] = 308;
    state->rot54 = 2048;
    ovl660_preview_confirmation = 0;
    func_80037040((s8 *)&D_800F4D98, D_800ABE08[ovl660_preview_bank][ovl660_preview_selection].glyph);
    D_800ACBD0[0].first = D_800ABE08[ovl660_preview_bank][ovl660_preview_selection].first;
    D_800ACBD0[0].second = D_800ABE08[ovl660_preview_bank][ovl660_preview_selection].second;
    ovl660_available_car_counts[0] = func_800370BC(&D_800ABE08[0][0]);
    ovl660_available_car_counts[1] = func_800370BC(&D_800ABE08[1][0]);
}

void ovl660_func_80116530(void) {
    RrPreviewColor color;
    s32 drawFlags;
    s32 scale;
    s8 modelReady;
    s32 drawModel;
    s32 rowIndex;
    s32 first;
    s32 loadResult;
    s32 previousCar;
    s32 other;
    s32 selectionState;
    u8 confirmationState;
    u8 closingState;
    u8 headerState;
    s8 brightness;

    modelReady = 0;
    D_800F2DE4 = (u32)D_800F2DE4 + 1;
    D_800ABEF8[0].rot54 = ((u32)D_800ABEF8[0].rot54 + 8) & 0xFFF;
    func_80033A74(1);
    func_8006EF8C(6);
    loadResult = func_8003AE4C(0, 1);
    if (loadResult != 1) {
        if (((s8 *)D_800F2AB0)[1] == 1) {
            ((s8 *)D_800F2AB0)[1] = 0;
        }
        goto draw_rows;
    }
    func_8002E394(D_800ABEF8);
    if (((s8 *)D_800F2AB0)[1] == 7) {
        ((s8 *)D_800F2AB0)[1] = loadResult;
    }
    modelReady = 1;
    switch (D_800F4D8A) {
    case 7:
        if (D_800F4D80[(s16)(ovl660_preview_bank ^ 1) * 3 + 4].byte14 == 5) {
            D_800F4D8A = 0;
        }
    case 0:
        func_8002AF8C(1, 1, 1);
        previousCar = func_8003708C((s8 *)&D_800F4D98);
        func_8002B330(1, 2, (char *)&ovl660_preview_selection,
                      ovl660_available_car_counts[ovl660_preview_bank], 0);
        func_80037040((s8 *)&D_800F4D98,
                      D_800ABE08[ovl660_preview_bank][ovl660_preview_selection].glyph);
        if (((u32)previousCar << 16) != ((u32)func_8003708C((s8 *)&D_800F4D98) << 16)) {
            func_8003A198();
            D_800ACC18 = D_800F4D40;
            func_80037040((s8 *)&D_800F4D98,
                          D_800ABE08[ovl660_preview_bank][ovl660_preview_selection].glyph);
            D_800ACBD0[0].first = D_800ABE08[ovl660_preview_bank][ovl660_preview_selection].first;
            D_800ACBD0[0].second = D_800ABE08[ovl660_preview_bank][ovl660_preview_selection].second;
            ((s8 *)D_800F2AB0)[1] = 6;
        }
        D_800F4D80[ovl660_preview_row_map[ovl660_preview_bank][D_800AC64A]].cells[0] = 0;
        func_8002B330(0, 2, (char *)&D_800AC64A, D_800AC5E0, 0);
        D_800F4D80[ovl660_preview_row_map[ovl660_preview_bank][D_800AC64A]].cells[0] = 2;
        selectionState = D_800F4D80[ovl660_preview_bank * 3 + 4].byte14;
        if (selectionState != 2) {
            break;
        }
        if (D_800F3A38[0].a != 0) {
            D_800F4D80[ovl660_preview_row_map[ovl660_preview_bank][D_800AC64A]].cells[0] = 1;
            if (D_800AC64A == 0) {
                func_8004F24C(1);
                ovl660_preview_confirmation = selectionState;
                D_800F4D8A = 4;
            } else if (D_800AC64A == 1 && D_800ABE08[1][0].glyph != -1) {
                func_8004F24C(1);
                ovl660_preview_selection = 0;
                rowIndex = 0;
                do {
                    if (D_800F4D80[ovl660_preview_bank * 3 + rowIndex + 4].byte14 == 2) {
                        D_800F4D80[ovl660_preview_bank * 3 + rowIndex + 4].byte14 = 3;
                    }
                    D_800F4D80[(s16)(ovl660_preview_bank ^ 1) * 3 + rowIndex + 4].byte14 = 0;
                    rowIndex += 1;
                } while (rowIndex < 3);
                D_800F4D8A = 7;
                D_800AC64A = 0;
                ovl660_preview_bank ^= 1;
            } else if (D_800AC64A == 2) {
                func_8004F24C(1);
                rowIndex = 0;
                do {
                    if (D_800F4D80[ovl660_preview_bank * 3 + rowIndex + 4].byte14 == 2) {
                        D_800F4D80[ovl660_preview_bank * 3 + rowIndex + 4].byte14 = 3;
                    }
                    rowIndex += 1;
                } while (rowIndex < 3);
                goto close_selection;
            }
        } else if (D_800F3A38[0].b != 0) {
            func_8004F24C(2);
            rowIndex = 0;
            do {
                if (D_800F4D80[ovl660_preview_bank * 3 + rowIndex + 4].byte14 == 2) {
                    D_800F4D80[ovl660_preview_bank * 3 + rowIndex + 4].byte14 = 3;
                }
                rowIndex += 1;
            } while (rowIndex < 3);
        close_selection:
            ((s8 *)D_800F2AB0)[1] = 3;
            D_800F4D8A = 5;
            D_800F3AFC->state10 = 4;
        }
        break;
    case 3:
        D_800AC5D0 = (u16)D_800AC5D0 - 1;
        func_80014A8C(-1);
        func_800207D4(&D_800ACC14->ordering.ordering70[0], (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            func_80053CB0();
            D_800F4BE8 = 0;
            D_800F4C6A = 1;
            D_800F3A26 = 0x23;
            D_800F4D8A = 0;
            goto save_preview_arena;
        }
        break;
    case 4:
        confirmationState = D_800F4D80[10].byte14;
        if (confirmationState == 2) {
            D_800F4D80[D_800F4D98.selection.pad02[1] + 10].cells[0] = 0;
            func_8002B330(0, 1, (char *)&D_800F4D98.selection.pad02[1], 2, 0);
            D_800F4D80[D_800F4D98.selection.pad02[1] + 10].cells[0] = confirmationState;
            if (D_800F3A38[0].a != 0) {
                func_8004F24C(1);
                func_8002AF8C(0, 1, 1);
                D_800F4D80[D_800F4D98.selection.pad02[1] + 10].cells[0] = 1;
                D_800F4D80[10].byte14 = 7;
                D_800F4D80[11].byte14 = 7;
                D_800F4D8A = 3;
                D_800AC5D0 = 0x40;
                func_80053AF4(0, 0x3C);
            save_preview_arena:
                D_800F4D70.first = D_800ACC18;
            } else if (D_800F3A38[0].b != 0) {
                func_8004F24C(2);
                D_800F4D80[10].byte14 = 3;
                D_800F4D80[11].byte14 = 3;
                D_800F4D8A = 6;
            }
        }
        break;
    case 6:
        if (D_800F4D80[10].byte14 == 5) {
            D_800F4D8A = 0;
            ovl660_preview_confirmation = 1;
            D_800F4D80[10].byte14 = 0;
            D_800F4D80[11].byte14 = 0;
        }
        break;
    case 5:
        closingState = D_800F4D80[ovl660_preview_bank * 3 + 4].byte14;
        if (closingState == 5 && ((s8 *)D_800F2AB0)[1] == closingState) {
            func_8003A198();
            ovl660_func_80114920();
            D_800F4C6A = 1;
            D_800F3A26 = 50;
            D_800F4D8A = 0;
            goto end;
        }
        break;
    }
draw_rows:
    if (ovl660_preview_confirmation == 0 && ((s8 *)D_800F2AB0)[1] == 2) {
        rowIndex = 0;
        do {
            if (D_800F4D80[rowIndex].byte14 == 9) {
                D_800F4D80[rowIndex].byte14 = 3;
            }
            rowIndex += 1;
        } while (rowIndex < 4);
    }
    headerState = D_800F4D80[0].byte14;
    if (headerState == 5) {
        if (ovl660_preview_confirmation == 0) {
            ovl660_preview_confirmation = 1;
            D_800F3AFC->half02 = 1;
        }
        if (D_800F4D8A == 7) {
            other = (s16)(ovl660_preview_bank ^ 1);
            if (D_800F4D80[other * 3 + 4].byte14 == headerState) {
                first = ovl660_preview_bank * 3 + 4;
            } else {
                first = other * 3 + 4;
            }
        } else {
            first = ovl660_preview_bank * 3 + 4;
        }
        rowIndex = 0;
        do {
            func_8003EC88(first + rowIndex);
            rowIndex += 1;
        } while (rowIndex < 3);
        if (ovl660_preview_confirmation == 2) {
            func_8003EC88(10);
            func_8003EC88(11);
        }
    }
    drawModel = modelReady;
    if (((u32)((u8)((s8 *)D_800F2AB0)[1] - 1) < 2U) && (drawModel == 1)) {
        func_80037198(0);
        func_8002E5A0(D_800ABEF8);
        scale = 0x100;
        if (((s8 *)D_800F2AB0)[1] == drawModel) {
            brightness = ~((D_800F2AB0[2] * 0xFF) / 30);
            color.channels.r = brightness;
            color.channels.g = brightness;
            color.channels.b = brightness;
            drawFlags = 0x102;
            scale = (D_800F2AB0[2] << 8) / 30;
        } else {
            drawFlags = 1;
            color.channels.r = 0x80;
            color.channels.g = 0x80;
            color.channels.b = 0x80;
        }
        func_8002CC88(D_800ABEF8, color.argument, scale, drawFlags);
    }
    func_8003D6F0(0);
    if (func_8003FBA8(0) > 0) {
        func_8002B12C(0xA0, 0x90, 8, 1, ovl660_preview_selection, 0,
                      ovl660_available_car_counts[ovl660_preview_bank]);
    }
end:
    func_8002A344(ovl660_preview_bank != 0 ? 0x10036 : 0x10032);
}
