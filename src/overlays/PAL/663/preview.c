#include "rr/overlay663.h"

void ovl663_init_preview(void) {
    RrProjectionScratchPrefix *scratch;
    RrRaceVehicle *state;

    ovl663_selected_record = 0;
    func_8002A604();
    D_800ACC68 = 2;
    D_800F2DE4 = 0;
    func_80013890(3);
    scratch = RR_PROJECTION_SCRATCH;
    scratch->word18 = 0;
    scratch->word1C = 0;
    scratch->word20 = 0;
    scratch->word08 = 0;
    scratch->word0C = 0;
    scratch->word10 = 0;
    state = D_800ABEF8;
    state->pos20[1] = -200;
    state->pos20[2] = 2024;
    state->pos20[0] = 0;
    state->rot50 = 40;
    state->rot54 = 0;
    state->rot58 = 0;
    func_8003E928(0, 12, 64, 68, 32, 0, 0, 1, 0, 83);
    func_8003D228(0, 12, 128, 76, 34, 0, 0xC01);
    D_800F3AFC[0].half02 = 1;
}

void ovl663_draw_preview(void) {
    RrPreviewMatrixWorkspace mat;
    s16 rot[4];
    RrGpuRectangleFields light;
    RrRaceVehicle *state;

    func_8006E1AC();
    func_8006EF8C(6);
    state = D_800ABEF8;
    state->rot54 = (s32)(((u32)state->rot54 + 8U) & 0xFFFU);
    rot[0] = state->rot50;
    rot[1] = state->rot54;
    rot[2] = state->rot58;
    RotMatrix(rot, mat.halves);
    light.x = 100;
    light.y = 80;
    light.w = 120;
    light.h = 280;
    func_80019D5C(&light, 128, D_800ACC14->ordering.ordering70);
    func_80019C90(4096);
    func_800195BC((char *)state->pos20, mat.halves);
}

void ovl663_update_preview(void) {
    s32 old;
    if (func_8003BEA8(ovl663_available_courses[ovl663_selected_record])) {
        switch (D_800F4D8A) {
        case 0:
            if (D_800F4D80->byte14 == 2) {
                func_8002AF8C(1, 1, 0);
                D_800F4D8A = 1;
            }
            break;
        case 1:
            old = ovl663_selected_record;
            func_8002B330(1, 4, (char *)&ovl663_selected_record, ovl663_available_course_count, 0);
            if (old != ovl663_selected_record)
                ResetScreenCounters();
            else
                ovl663_draw_preview();
            D_800F4D80->cells[0] = 2;
            if (D_800F3A60) {
                PlaySfx(1);
                func_8002AF8C(0, 1, 0);
                D_800F4D8A = 8;
                D_800F4D80->cells[0] = 1;
                D_800AC5D0 = 64;
                D_800F4D80->byte14 = 3;
            }
            break;
        case 8:
            D_800AC5D0--;
            func_800207D4(D_800ACC14->ordering.ordering70, (64 - D_800AC5D0) * 4, 69);
            if (D_800AC5D0 <= 0) {
                ovl663_init_menu();
                D_800F4C6A = 1;
                D_800F3A26 = 74;
                D_800F4D8A = 0;
                return;
            }
            break;
        }
    }
    if (ovl663_available_course_count >= 2)
        func_8002B12C(160, 384, 16, 1, ovl663_selected_record, 0, ovl663_available_course_count);
    func_8003EC88(0);
    func_8002A344(0x10058);
}

void ovl663_init_result_preview(void) {
    RrRaceVehicle *state;
    RrProjectionScratchPrefix *scratch;
    D_800F2DE4 = 0;
    func_80013890(5);
    state = D_800ABEF8;
    scratch = RR_PROJECTION_SCRATCH;
    scratch->word18 = 120;
    scratch->word08 = -16;
    scratch->word0C = -248;
    scratch->word1C = 0;
    scratch->word20 = 0;
    scratch->word10 = -96;
    func_8002E4F4(state);
    state->pos20[2] = 308;
    state->pos20[1] = 10;
    state->rot54 = 2560;
    func_80037110();
    D_800F3AFC->half02 = 1;
}

void ovl663_update_result_preview(void) {

    RrPreviewColor color;
    s32 scale;
    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    switch ((u16)D_800F4D8A) {
    case 0:
        if (D_800F4D80[3].byte14 == 2) {
            func_8002AF8C(1, 1, 0);
            D_800F4D8A = 1;
        }
        break;
    case 1:
        D_800F4D80[3].cells[0] = 2;
        if (D_800F3A60) {
            PlaySfx(1);
            func_8002AF8C(0, 1, 0);
            D_800F4D80[3].cells[0] = 1;
            D_800F4D80[3].byte14 = 3;
            D_800F4D8A = 2;
            D_800F3AFC->state10 = 4;
            D_800F2AB0[1] = 3;
        }
        break;
    case 2:
        if (D_800F3AFC->state10 == 11 && ((s8 *)D_800F2AB0)[1] == 5) {
            ovl663_init_selection(ovl663_record_kind);
            D_800F4C6A = 1;
            D_800F3A26 = 75;
            D_800F4D8A = 0;
        }
        break;
    }
    D_800ABEF8[0].rot54 = (s32)(((u32)D_800ABEF8[0].rot54 + 8U) & 4095U);
    func_80033A74(1);
    func_8006EF8C(6);
    if ((u32)D_800F2AB0[1] - 1U < 2U) {
        RrSelectionRows *selected = &D_800F4D98.selection;
        selected->pad02[1] = 2;
        func_80037198(0);
        selected->pad02[1] = 0;
        func_8002E5A0(D_800ABEF8);
        if (((s8 *)D_800F2AB0)[1] == 1) {
            s32 level = ~(D_800F2AB0[2] * 255 / 30);
            color.channels.r = level;
            color.channels.g = level;
            color.channels.b = level;
            scale = D_800F2AB0[2] * 256 / 30;
        } else {
            color.channels.r = 128;
            color.channels.g = 128;
            color.channels.b = 128;
            scale = 256;
        }
        func_8002CC88(D_800ABEF8, color.argument, scale, 0);
    }
    func_8003EC88(3);
    func_8003D6F0(0);
    func_8003FBA8(0);
    func_8002A344(0x10022);
}
