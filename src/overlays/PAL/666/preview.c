#include "rr/overlay666.h"

void ovl666_init_preview(void) {
    RrProjectionScratchPrefix *sp;

    D_800F2DE4 = 0;
    D_800ABEB0 = 0;
    ovl666_selected_course.word = 0;
    ovl666_preview_phase = 0;
    D_800AC64A = 0;
    D_800AC5E0 = 2;
    func_8003E928(0, 24, 88, 68, 32, 0, 0, 1, 0, 84);
    func_8003E928(1, 24, 128, 68, 32, 0, 0, 1, 0, 85);
    func_80013890(5);
    func_8003A198();
    func_80037110();
    D_800ACC18 = D_800F4D40;
    D_800F3AFC[0].half02 = 1;
    sp = RR_PROJECTION_SCRATCH;
    sp->word18 = 120;
    sp->word08 = -16;
    sp->word0C = -248;
    sp->word1C = 0;
    sp->word20 = 0;
    sp->word10 = -96;
    func_8002E4F4(D_800ABEF8);
    D_800ABEF8[0].pos20[1] = 10;
    D_800ABEF8[0].pos20[2] = 308;
    D_800ABEF8[0].rot54 = 2048;
    func_80037040((s8 *)&D_800F4D98,
                  ovl666_available_courses[D_800F3826][ovl666_selected_course.word].glyph);
    D_800ACBD0[0].first = ovl666_available_courses[D_800F3826][ovl666_selected_course.word].first;
    D_800ACBD0[0].second = ovl666_available_courses[D_800F3826][ovl666_selected_course.word].second;
    func_8002AF8C(0, 1, 1);
}

void ovl666_update_preview(void) {
    RrPreviewColor color;
    s32 drawFlags;
    s32 scale;
    s8 modelReady;
    s32 rowIndex;
    s32 drawModel;
    s32 loadResult;
    s32 previousCar;
    u8 selectionState;
    u8 confirmationState;
    u8 closingState;
    u16 course;
    s8 brightness;

    D_800F2DE4 = (u32)D_800F2DE4 + 1;
    modelReady = 0;
    D_800ABEB0 = (u32)D_800ABEB0 + 1;
    D_800ABEF8[0].rot54 = ((u32)D_800ABEF8[0].rot54 + 8) & 0xFFF;
    func_80033A74(1);
    func_8006EF8C(6);
    if (D_800F4D8A != 3) {
        loadResult = func_8003AE4C(0, 1);
        if (loadResult != 1) {
            if (((s8 *)D_800F2AB0)[1] == 1) {
                ((s8 *)D_800F2AB0)[1] = 0;
            }
            rowIndex = 0;
            goto draw_selection_rows;
        }
        func_8002E394(D_800ABEF8);
        if (((s8 *)D_800F2AB0)[1] == 7) {
            ((s8 *)D_800F2AB0)[1] = loadResult;
        }
    }
    modelReady = 1;
    switch (D_800F4D8A) {
    case 0:
        func_8002AF8C(1, 1, 1);
        previousCar = func_8003708C((s8 *)&D_800F4D98);
        func_8002B330(1, 2, (char *)&ovl666_selected_course.halves[0],
                      ovl666_course_counts[D_800F3826], 0);
        func_80037040((s8 *)&D_800F4D98,
                      ovl666_available_courses[D_800F3826][ovl666_selected_course.word].glyph);
        if (((u32)previousCar << 16) != ((u32)func_8003708C((s8 *)&D_800F4D98) << 16)) {
            func_8003A198();
            D_800ACC18 = D_800F4D40;
            func_80037040((s8 *)&D_800F4D98,
                          ovl666_available_courses[D_800F3826][ovl666_selected_course.word].glyph);
            D_800ACBD0[0].first =
                ovl666_available_courses[D_800F3826][ovl666_selected_course.word].first;
            D_800ACBD0[0].second =
                ovl666_available_courses[D_800F3826][ovl666_selected_course.word].second;
            ((s8 *)D_800F2AB0)[1] = 6;
        }
        D_800F4D80[D_800AC64A + 3].cells[0] = 0;
        func_8002B330(0, 2, (char *)&D_800AC64A, 2, 0);
        D_800F4D80[D_800AC64A + 3].cells[0] = 2;
        selectionState = D_800F4D80[D_800AC64A + 3].byte14;
        if (selectionState == 2) {
            if (D_800F3A38[0].a != 0) {
                D_800F4D80[D_800AC64A + 3].cells[0] = 1;
                if (D_800AC64A == 0) {
                    func_8004F24C(1);
                    ovl666_preview_phase = selectionState;
                    D_800F4D8A = 4;
                    goto draw_selection;
                } else if (D_800AC64A == 1) {
                    func_8004F24C(1);
                    rowIndex = 0;

                    do {
                        if (D_800F4D80[rowIndex + 3].byte14 == 2) {
                            D_800F4D80[rowIndex + 3].byte14 = 3;
                        }
                        rowIndex += 1;
                    } while (rowIndex < 2);
                    goto close_selection;
                }
            } else if (D_800F3A38[0].b != 0) {
                func_8004F24C(2);
                rowIndex = 0;

                do {
                    if (D_800F4D80[rowIndex + 3].byte14 == 2) {
                        D_800F4D80[rowIndex + 3].byte14 = 3;
                    }
                    rowIndex += 1;
                } while (rowIndex < 2);
            close_selection:
                ((s8 *)D_800F2AB0)[1] = 3;
                D_800F4D8A = 5;
                D_800F3AFC->state10 = 4;
            }
        }
        goto draw_selection;
    case 3:
        func_8002AF8C(0, 1, 1);
        if (D_800AC5D0 > 0) {
            D_800AC5D0 = (u16)D_800AC5D0 - 1;
        }
        func_80014A8C(-1);
        func_800207D4(&D_800ACC14->ordering.ordering70[0], (0x40 - D_800AC5D0) * 4, 0x45);
        loadResult = func_8003AE4C(1, 1);
        if ((loadResult == 1) && (D_800AC5D0 <= 0)) {
            D_800F4D44 = 6;
            course =
                ovl666_course_dispatch[D_800F4D98.selection.primary][D_800F4D98.selection.variant];
            ovl666_result_kind = loadResult;
            D_800AC648 = course;
            D_800F3914 = func_80032E9C((s16)course);
            func_80053CB0();
            D_800F4BE8 = 0;
            D_800F4C6A = loadResult;
            D_800F3A26 = 0x23;
            D_800F4D8A = 0;
            goto save_preview_arena;
        }
        goto draw_selection;
    case 4:
        confirmationState = D_800F4D80[5].byte14;
        if (confirmationState == 2) {
            D_800F4D80[D_800F4D98.selection.pad02[1] + 5].cells[0] = 0;
            func_8002B330(0, 1, (char *)&D_800F4D98.selection.pad02[1], 2, 0);
            D_800F4D80[D_800F4D98.selection.pad02[1] + 5].cells[0] = confirmationState;
            if (D_800F3A38[0].a != 0) {
                func_8004F24C(1);
                func_8002AF8C(0, 1, 1);
                D_800F4D80[D_800F4D98.selection.pad02[1] + 5].cells[0] = 1;
                D_800F4D80[5].byte14 = 7;
                D_800F4D80[6].byte14 = 7;
                D_800F4D98.selection.rows[0] = D_800F4D98.entry;
                D_800F4D98.selection.rows[0].parameter = 0x13;
                D_800F3914 = 0;
                D_800F4D8A = 3;
                func_8003A198();
                D_800AC5D0 = 0x40;
                func_80053AF4(0, 0x3C);
            save_preview_arena:
                D_800F4D70.first = D_800ACC18;
                goto draw_selection;
            }
            if (D_800F3A38[0].b != 0) {
                func_8004F24C(2);
                D_800F4D80[5].byte14 = 3;
                D_800F4D80[6].byte14 = 3;
                D_800F4D8A = 6;
            }
        }
        goto draw_selection;
    case 6:
        if (D_800F4D80[5].byte14 == 5) {
            D_800F4D8A = 0;
            ovl666_preview_phase = 1;
            D_800F4D80[5].byte14 = 0;
            D_800F4D80[6].byte14 = 0;
        }
        goto draw_selection;
    case 5:
        func_8002AF8C(0, 1, 1);
        closingState = D_800F4D80[3].byte14;
        if ((closingState == 5) && (((s8 *)D_800F2AB0)[1] == closingState)) {
            func_8003A198();
            ovl666_init_menu();
            D_800F4C6A = 1;
            D_800F3A26 = 0x4F;
            D_800F4D8A = 0;
            goto end;
        }
        goto draw_selection;
    }
draw_selection:
    rowIndex = 0;
draw_selection_rows:
    do {
        func_8003EC88(rowIndex + 3);
        rowIndex += 1;
    } while (rowIndex < 2);
    if (ovl666_preview_phase == 2) {
        func_8003EC88(5);
        func_8003EC88(6);
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
        if (ovl666_course_counts[D_800F3826] >= 2) {
            func_8002B12C(0xA0, 0x90, 8, 1, ovl666_selected_course.word, 0,
                          ovl666_course_counts[D_800F3826]);
        }
    }
end:
    func_8002A344(0x10036);
}
