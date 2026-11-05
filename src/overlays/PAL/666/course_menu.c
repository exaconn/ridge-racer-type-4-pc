#include "rr/overlay666.h"

void ovl666_init_menu(void) {
    s32 i;
    D_800F2DE4 = 0;
    D_800ABEB0 = 0;
    D_800AC64A = 0;
    D_800AC5E0 = 3;
    func_8003A198();
    D_800F4D70.second = D_800102F0 + 0x2D000;
    func_8003A1B4();
    D_800ACC18 = D_800F4D70.second;
    for (i = 0; i < 4; i++)
        func_8003E040(i, -2, 0, 1);
    func_8003DC9C(D_800F4D8C->glyph07, D_800F4D8C->x08, D_800F4D8C->y0A, D_800F4D8C->w0C,
                  D_800F4D8C->h0E, 0, 0);
    func_8003E928(0, 12, 64, 68, 32, 0, 0, 1, 0, 83);
    func_8003E928(1, 12, 104, 68, 32, 0, 0, 1, 0, 85);
    func_8003E928(2, 12, 368, 248, 32, 0, 0, 4, 12, D_800F3826 + 8);
    func_8003E928(3, 24, 88, 68, 32, 0, 0, 1, 0, 84);
    func_8003E928(4, 24, 128, 68, 32, 0, 0, 1, 0, 85);
    func_8003E928(5, 116, 88, 28, 32, 0, 0, 1, 0, 55);
    func_8003E928(6, 116, 128, 28, 32, 0, 0, 1, 0, 56);
}

void ovl666_init_scene(void) {
    s32 i;
    s32 j;

    SetDispMask(0);
    func_8002A604();
    func_8001ECE8(0, 0, 0, 2);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 9; j++) {
            ovl666_available_courses[i][j].glyph = -1;
        }
    }
    for (i = 0; i < 4; i++) {
        ovl666_course_counts[i] = 0;
        for (j = 0; j < 9; j++) {
            func_80037040((s8 *)&D_800F4D98, D_800ABE08[1][j].glyph);
            if (D_800F4D98.selection.variant == i && D_800F4D98.selection.pad02[0] < 39) {
                ovl666_available_courses[i][ovl666_course_counts[i]] = D_800ABE08[1][j];
                ovl666_course_counts[i]++;
            }
        }
    }
    D_800AC648 = 0;
    D_800F3826 = 0;
    func_8001E84C(ovl666_scene_name);
    ovl666_init_menu();
    D_800F4C6A = 1;
    D_800F3A26 = 79;
    D_800F4D8A = 0;
    func_80053BB4(0);
}

void ovl666_update_menu(void) {
    s32 sound;
    s16 glyphIndex;
    RrMenuGlyphRow1C *glyphBase;
    s32 selected;
    s32 ready;
    s16 timer;
    s32 glyphCount;
    s32 choiceRow;

    s32 closing;
    s8 cellState;

    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    D_800ABEB0 = (s32)((u32)D_800ABEB0 + 1U);
    ready = func_8003B47C();
    if (ready == 1) {
        switch (D_800F4D8A) {
        case 0:
            glyphCount = 0;
            do {
                D_800F4D8C[glyphCount].phase18 =
                    (s32)((u32)D_800F4D8C[glyphCount].phase18 + 0x30000U);
                glyphCount++;
            } while (glyphCount < 4);
            D_800F4D8A = 1;
            goto title;
        case 1:
            func_8002AF8C(1, 1, 1);
            D_800F4D80[D_800AC64A].cells[0] = 0;
            func_8002B330(0, 2, (char *)&D_800AC64A, 2, 0);
            cellState = 2;
            if (ovl666_course_counts[D_800F3826] <= 0) {
                D_800F4D80[0].half20 = ready;
                if (D_800AC64A == 0) {
                    cellState = 4;
                }
            } else {
                D_800F4D80[0].half20 = 0;
            }
            D_800F4D80[D_800AC64A].cells[0] = cellState;
            ovl666_update_course_selection();
            closing = 0;
            if (D_800F3A38[0].a != 0) {
                selected = D_800AC64A;
                if (selected == 0) {
                    if (ovl666_course_counts[D_800F3826] > 0) {
                        closing = 1;
                        D_800F4D8A = 3;
                        func_8004F24C(1);
                        choiceRow = 0;

                        do {
                            if (D_800F4D80[choiceRow].byte14 == 2) {
                                D_800F4D80[choiceRow].byte14 = 3U;
                            }
                            choiceRow += 1;
                        } while (choiceRow < 3);
                    } else {
                        func_8004F24C(4);
                    }
                } else {
                    if (selected != 1)
                        goto decision;
                    sound = 1;
                    goto play_cancel;
                }
            } else {
                sound = 2;
                if (D_800F3A38[0].b != 0) {
                play_cancel:
                    func_8004F24C(sound);
                    closing = 2;
                }
            }
        decision:
            if (closing > 0) {
                if (closing == 2) {
                    choiceRow = 0;

                    do {
                        if (D_800F4D80[choiceRow].byte14 == 2) {
                            D_800F4D80[choiceRow].byte14 = 3U;
                        }
                        choiceRow += 1;
                    } while (choiceRow < 3);
                    D_800F4D8A = 8;
                    D_800AC5D0 = 0x40;
                    func_80053AF4(0, 0x3C);
                }
            }
            goto title;
        case 3:
            func_8002AF8C(0, 1, 1);
            if (D_800F4D80[0].byte14 == 5) {
                ovl666_init_preview();
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
            D_800AC5D0 = (u16)timer;
            func_800207D4(&D_800ACC14->ordering.ordering70[0], (0x40 - timer) * 4, 0x45);
            if ((s16)D_800AC5D0 <= 0) {
                D_800F4C6A = ready;
                D_800F3A26 = 3;
                D_800F4D8A = 0;
                func_80053CB0();
            }
            goto update_rows;
        default:
        title:
            func_8002B12C(0xA0, 0xCC, 0x30, 0, (s32)D_800F3826, 0, 4);
            glyphIndex = D_800F3826;
            if ((u8)D_800F3920.prefix.flags5C[glyphIndex] != 0) {
                glyphBase = D_800F4D8C;

                ovl666_draw_selection_pulse(glyphBase[glyphIndex].x08 + 0x14,
                                            glyphBase[glyphIndex].y0A + 0x28);
                func_8003E13C(D_800F3826);
            } else {
                func_8003E13C(glyphIndex);
            }
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

void ovl666_update_course_selection(void) {
    {
        RrUiRow24 *ui = D_800F4D80;
        s32 selected = D_800F3826;
        ui[2].cells[selected] = 0;
    }
    if (D_800F3A42 & 0x2000) {
        if (D_800F3826 < 3) {
            D_800F3826++;
            D_8009FDA4 = 2;
            goto changed;
        }
    }
    if (D_800F3A42 & 0x8000) {
        if (D_800F3826 > 0) {
            D_800F3826--;
            D_8009FDA4 = 1;
            goto changed;
        }
    }
    D_8009FDA4 = 0;
    goto active;
changed:
    func_8004F24C(0);
    if (D_800F3826 < 0)
        D_800F3826 = 0;
    else if (D_800F3826 >= 4)
        D_800F3826 = 3;
    D_800F4D8C[D_800F3826].control02 = -6;
    D_800F4D8C[D_800F3826].flags00 = 0;
    D_800F4D80[2].byte16 = (u8)D_800F3826 + 8;
active:
    {
        RrUiRow24 *ui = D_800F4D80;
        s32 selected = D_800F3826;
        ui[2].cells[selected] = 2;
    }
}

void ovl666_draw_selection_pulse(s32 x, s32 y) {
    RrGpuScratchPointers *scratch = RR_GPU_SCRATCH;
    u8 *cursor = scratch->cursor;
    s32 wave = rsin(((u32)D_800ABEB0 << 5) & 0xFE0) / 32;
    u16 mode = 57;
    if (wave < 0) {
        mode = 89;
        wave = -wave;
    }
    cursor = func_8001FF84(D_800ACC14->ordering.ordering70, cursor, x, y, &ovl666_selection_sprite,
                           0x7FA4, ((wave + 1) * 2) | 1);
    cursor = func_8001FCC8(D_800ACC14->ordering.ordering70, cursor, mode);
    scratch->cursor = cursor;
}
