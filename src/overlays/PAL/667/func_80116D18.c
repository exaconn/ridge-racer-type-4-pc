#include "rr/menu_overlay_runtime.h"

/* Four-player selection: per-pair page, row and course group tables. */
extern RrCourseChoice4 D_8010BFD8[2][2][9];
extern s16 D_8010C0B8;
extern s16 D_8010C0C0[2][2];
extern s16 D_8010C0D0[2];
extern s16 D_8010C0D8[2];
extern s8 D_800ABEBC[];
extern s32 UiRowCount;
/* Resident word reached through its enclosing array object (aliases row stores). */
extern s32 D_800F4CB8[1];
extern s16 D_800FFC18;
extern s16 D_800AD508;
extern u8 ovl667_D_80118E10[4];
extern s8 ovl667_D_80118E14[2][4];
extern s8 ovl667_D_80118E1C[2][2][4];
s32 func_80034FC0(void);
void func_80035024(s32);
void ovl667_func_80116BB4(void);
void ovl667_func_80117EF8(void);

void ovl667_func_80116D18(void) {
    RrPreviewColor color;
    s16 modelReady;
    s32 drawFlags;
    s32 drawModel;
    s32 scale;
    s32 loadResult;
    s32 previous;
    s32 state;
    s32 i;
    s32 cancel;
    s32 row;
    u16 player;
    u16 page;
    u16 other;
    s8 brightness;
    u8 closing;

    modelReady = 0;
    D_800F2DE4++;
    if (func_80034FC0() != 0) {
        func_80053CB0();
        return;
    }
    func_80035024(2);
    PlayerCars[D_800AC5D4 & 1].rot54 = (PlayerCars[D_800AC5D4 & 1].rot54 + 8) & 0xFFF;
    func_80033A74(1);
    SetupViewport(6);
    loadResult = func_8003AE4C(D_800AC5D4, 1);
    if (loadResult != 1) {
        if ((s8)D_800F2AB0[1] == 1) {
            D_800F2AB0[1] = 0;
        }
        goto draw_rows;
    }
    SetCarBounds(&PlayerCars[D_800AC5D4 & 1]);
    if ((s8)D_800F2AB0[1] == 7) {
        D_800F2AB0[1] = loadResult;
    }
    modelReady = 1;
    switch (D_800F4D8A) {
    case 7:
        row = ovl667_D_80118E1C[D_800AC5D4 & 1][(s16)(D_8010C0D8[D_800AC5D4 & 1] ^ 1)][0];
        if (UiRows[row].byte14 == 5) {
            D_800F4D8A = 0;
        }
    case 0:
        func_8002AF8C(1, 1, 1);
        previous = PackChoiceGlyph((s8 *)&D_800F4D98 + D_800AC5D4 * 4);
        StepMenuValue(((D_800AC5D4 & 1) * 4) | 1, 2, (char *)&D_8010C0D0[D_800AC5D4 & 1],
                      D_8010C0C0[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]], 0);
        UnpackChoiceGlyph((s8 *)&D_800F4D98 + D_800AC5D4 * 4,
                      D_8010BFD8[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]][D_8010C0D0[D_800AC5D4 & 1]].glyph);
        if (((u32)previous << 16) != ((u32)PackChoiceGlyph((s8 *)&D_800F4D98 + D_800AC5D4 * 4) << 16)) {
            ResetScreenCounters();
            LoadCursor = D_800F4D70.first;
            UnpackChoiceGlyph((s8 *)&D_800F4D98 + D_800AC5D4 * 4,
                          D_8010BFD8[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]][D_8010C0D0[D_800AC5D4 & 1]].glyph);
            D_800ACBD0[D_800AC5D4].first = D_8010BFD8[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]][D_8010C0D0[D_800AC5D4 & 1]].first;
            D_800ACBD0[D_800AC5D4].second = D_8010BFD8[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]][D_8010C0D0[D_800AC5D4 & 1]].second;
            D_800F2AB0[1] = 6;
        }
        row = ovl667_D_80118E1C[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]][MenuCursor];
        UiRows[row].cells[0] = 0;
        StepMenuValue((D_800AC5D4 & 1) * 4, 2, (char *)&MenuCursor,
                      ovl667_D_80118E10[D_8010C0D8[D_800AC5D4 & 1]], 0);
        row = ovl667_D_80118E1C[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]][MenuCursor];
        UiRows[row].cells[0] = 2;
        state = UiRows[row].byte14;
        cancel = 0;
        if (state != 2) {
            break;
        }
        if (BadgeRows->state10 != 3) {
            break;
        }
        if (D_800F3A38[D_800AC5D4 & 1].a != 0) {
            PlaySfx(1);
            UiRows[row].cells[0] = 1;
            if (MenuCursor == ovl667_D_80118E14[D_8010C0D8[D_800AC5D4 & 1]][0]) {
                D_8010C0B8 = state;
                D_800F4D8A = 4;
            } else if (MenuCursor == ovl667_D_80118E14[D_8010C0D8[D_800AC5D4 & 1]][1] && D_8010BFD8[D_800AC5D4][1][0].glyph != -1) {
                D_8010C0D0[D_800AC5D4 & 1] = 0;
                MenuCursor = 0;
                for (i = 0; i < 4; i++) {
                    row = ovl667_D_80118E1C[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]][i];
                    if (row == -1) {
                        break;
                    }
                    if (UiRows[row].byte14 == 2) {
                        UiRows[row].byte14 = 3;
                    }
                }
                for (i = 0; i < 4; i++) {
                    row = ovl667_D_80118E1C[D_800AC5D4 & 1][(s16)(D_8010C0D8[D_800AC5D4 & 1] ^ 1)][i];
                    if (row == -1) {
                        break;
                    }
                    UiRows[row].byte14 = 0;
                }
                D_800F4D8A = 7;
                D_8010C0D8[D_800AC5D4 & 1] ^= 1;
            } else if (MenuCursor == ovl667_D_80118E14[D_8010C0D8[D_800AC5D4 & 1]][2]) {
                cancel = 1;
            } else if (MenuCursor == ovl667_D_80118E14[D_8010C0D8[D_800AC5D4 & 1]][3]) {
                for (i = 0; i < UiRowCount; i++) {
                    if (UiRows[i].byte14 == 2) {
                        UiRows[i].byte14 = 3;
                    }
                }
                D_800F2AB0[1] = 3;
                D_800AC5D0 = 0x40;
                D_800F4D8A = 10;
                BadgeRows->state10 = 4;
            }
        } else if (D_800F3A38[D_800AC5D4 & 1].b != 0) {
            PlaySfx(2);
            cancel = 1;
        }
        if (cancel == 1) {
            for (i = 0; i < UiRowCount; i++) {
                if (UiRows[i].byte14 == 2) {
                    UiRows[i].byte14 = 3;
                }
            }
            D_800F2AB0[1] = 3;
            D_800AC5D0 = 0x40;
            D_800F4D8A = 8;
            BadgeRows->state10 = 4;
            func_80053AF4(0, 0x3C);
        }
        break;
    case 3:
        row = ovl667_D_80118E1C[(D_800AC5D4 - 1) & 1][1][3] + 1;
        if (UiRows[row].byte14 == 5) {
            D_8010C0B8 = 1;
            D_800F4D8A = 0;
        }
        break;
    case 4:
        row = ovl667_D_80118E1C[D_800AC5D4 & 1][1][3] + 1;
        state = UiRows[row].byte14;
        if (state != 2) {
            break;
        }
        UiRows[row + ((RrMenuChoice4 *)&D_800F4D98)[D_800AC5D4].bytes[3]].cells[0] = 0;
        StepMenuValue((D_800AC5D4 & 1) * 4, 1, (char *)&((RrMenuChoice4 *)&D_800F4D98)[D_800AC5D4].bytes[3], 2, 0);
        UiRows[row + ((RrMenuChoice4 *)&D_800F4D98)[D_800AC5D4].bytes[3]].cells[0] = state;
        if (D_800F3A38[D_800AC5D4 & 1].a != 0) {
            PlaySfx(1);
            UiRows[row + ((RrMenuChoice4 *)&D_800F4D98)[D_800AC5D4].bytes[3]].cells[0] = 1;
            UiRows[row].byte14 = 3;
            UiRows[row + 1].byte14 = 3;
            for (i = 0; i < 4; i++) {
                row = ovl667_D_80118E1C[D_800AC5D4 & 1][D_8010C0D8[D_800AC5D4 & 1]][i];
                if (row == -1) {
                    break;
                }
                if (UiRows[row].byte14 == 2) {
                    UiRows[row].byte14 = 3;
                }
            }
            modelReady = 0;
            D_800F4D70.first = LoadCursor;
            if ((D_800AC5D4 & 1) > 0 || D_800ABEBC[(1 - D_800AD508) * 2 + 1] != 1) {
                D_800F4D8A = 5;
                for (i = 0; i < UiRowCount; i++) {
                    if (UiRows[i].byte14 == 2) {
                        UiRows[i].byte14 = 3;
                    }
                }
                D_800F2AB0[1] = 3;
                BadgeRows->state10 = 4;
                break;
            }
            D_800F4D8A = 3;
            D_800AC5D4 = D_800AC5D4 + 1;
            ovl667_func_80116BB4();
        } else if (D_800F3A38[D_800AC5D4 & 1].b != 0) {
            PlaySfx(2);
            UiRows[row].byte14 = 3;
            UiRows[row + 1].byte14 = 3;
            D_800F4D8A = 6;
        }
        break;
    case 5:
        func_8002AF8C(0, 1, 1);
        row = ovl667_D_80118E1C[D_800AC5D4 & 1][1][3] + 1;
        closing = UiRows[row].byte14;
        if (closing == 5 && (s8)D_800F2AB0[1] == closing) {
            ResetScreenCounters();
            ovl667_func_80117EF8();
            D_800F4C6A = 1;
            D_800F3A26 = 61;
            D_800F4D8A = 0;
        }
        break;
    case 6:
        row = ovl667_D_80118E1C[D_800AC5D4 & 1][1][3] + 1;
        if (UiRows[row].byte14 == 5) {
            D_800F4D8A = 0;
            D_8010C0B8 = 1;
            UiRows[row].byte14 = 0;
            UiRows[row + 1].byte14 = 0;
        }
        break;
    case 8:
        SetModelTexturePage(-1);
        func_8002AF8C(0, 1, 1);
        D_800AC5D0 = (u16)D_800AC5D0 - 1;
        func_800207D4(&D_800ACC14->ordering.ordering70[0], (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            func_80053CB0();
            D_800F4C6A = 1;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        }
        break;
    case 10:
        SetModelTexturePage(-1);
        func_8002AF8C(0, 1, 1);
        D_800AC5D0 = (u16)D_800AC5D0 - 1;
        func_800207D4(&D_800ACC14->ordering.ordering70[0], (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            D_800FFC18 = 2;
            D_800F4C6A = 1;
            D_800F3A26 = 71;
            D_800F4D8A = 0;
        }
        break;
    }
draw_rows:
    if (D_8010C0B8 == 0 && (s8)D_800F2AB0[1] == 2) {
        D_8010C0B8 = 1;
        BadgeRows->half02 = 1;
    }
    if (D_800F4D8A == 3) {
        player = (u16)D_800AC5D4 - 1;
    } else {
        player = D_800AC5D4;
    }
    if (D_800F4D8A == 7) {
        page = D_8010C0D8[player & 1];
        other = page ^ 1;
        row = ovl667_D_80118E1C[player & 1][(s16)other][0];
        if (UiRows[row].byte14 == 5) {
            other = page;
        }
    } else {
        other = D_8010C0D8[player & 1];
    }
    for (i = 0; i < 4; i++) {
        row = ovl667_D_80118E1C[player & 1][(s16)other][i];
        if (row == -1) {
            break;
        }
        DrawUiRow(row);
    }
    if (D_8010C0B8 == 2) {
        row = ovl667_D_80118E1C[player & 1][1][3];
        DrawUiRow(row + 1);
        DrawUiRow(row + 2);
    }
    if ((u32)((u8)D_800F2AB0[1] - 1) < 2U) {
        drawModel = modelReady;
        if (drawModel == 1) {
            func_80037198(D_800AC5D4);
            func_8002E5A0(&PlayerCars[D_800AC5D4 & 1]);
            scale = 0x100;
            if ((s8)D_800F2AB0[1] == drawModel) {
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
            func_8002CC88(&PlayerCars[D_800AC5D4 & 1], color.argument, scale, drawFlags);
        }
    }
    UpdateBadgeRowAnim(0);
    func_8003FBA8(0);
    D_800F4CB8[0] = D_800AC5D4 + 1;
    UpdateScreenFade(D_8010C0D8[D_800AC5D4 & 1] != 0 ? 0x10036 : 0x10032);
}
