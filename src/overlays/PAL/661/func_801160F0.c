#include "game/system/pad.h"
#include "rr/menu_overlay_runtime.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"

/* Versus course selection: each player picks a course group and course. */
extern u8 ovl661_D_80118358[];            /* option count per course group */
extern s8 ovl661_D_8011835C[2][4];        /* option meaning per group slot */
extern s8 ovl661_D_80118364[2][2][4];     /* UI row per player, group, slot */
extern s16 ovl661_D_80118438;
extern s16 ovl661_D_80118440[2][2];       /* course count per player/group */
extern s16 ovl661_D_8011844C[2];          /* selected course per player */
extern s16 ovl661_D_80118450[2];          /* selected group per player */
extern CourseChoice4 ovl661_D_80118458[2][2][9];
extern s32 UiRowCount, D_800F4CB8;
extern s16 CardMenuOperation;
void SetupMenuCarCamera(s32);
void SetupViewport(s32);
s32 LoadCarRangeStep(s32, s32);
void SetCarBounds(RaceVehicle *);
void UpdateCarWorldTransform(RaceVehicle *);
void SetMenuFrameOptions(s32, s32, s32);
void StepMenuValue(s32, s32, char *, s32, s32);
void UnpackChoiceGlyph(s8 *, s32);
s32 PackChoiceGlyph(s8 *);
void ResetScreenCounters(void);
void SetModelTexturePage(s32);
void PlaySfx(s32);
void DrawUiRow(s32);
void DrawSelectionIconGrid(s32);
void UpdateBadgeRowAnim(s32);
s32 UpdateImageRow(s32);
void DrawMenuStepArrows(s32, s32, s32, s32, s32, s32, s32);
void UpdateScreenFade(s32);
void AddScreenFadeTiles(u32 *, s32, u32);
void SeqFadeVolume(s32, s32);
void SeqMuteVolume(void);
void ovl661_func_80115598(s32);
void ovl661_func_80115F8C(void);
void ovl661_func_80117350(void);

#define MODE (((s8 *)ImageRows)[1])
#define SELECTION ((s8 *)&SelectionRows)
#define ENTRIES ((SelectionRow *)&SelectionRows)
#define PLAYER (D_800AC5D4 & 1)
#define GROUP_ROW(p, g, i) ovl661_D_80118364[p][g][i]

void ovl661_func_801160F0(void)
{
    PreviewColor color;
    s16 active;
    s32 result;
    s32 row;
    u8 status;
    s32 before;
    s32 i;
    s32 close;
    s16 player;
    s16 group;
    s32 brightness;
    s32 flags;
    s32 sound;
    s32 groups;
    s8 last;
    RaceVehicle *vehicle;
    RaceVehicle *vehicles;
    s32 *tick;
    ControllerState5C *input;

    active = 0;
    tick = &ModeFrame;
    vehicles = PlayerCars;
    vehicle = &vehicles[PLAYER];
    (*tick)++;
    vehicle->rot54 = (vehicle->rot54 + 8) & 0xFFF;
    SetupMenuCarCamera(1);
    SetupViewport(6);
    result = LoadCarRangeStep(D_800AC5D4, 1);
    if (result != 1) {
        if (MODE == 1) {
            MODE = 0;
        }
    } else {
        SetCarBounds(&vehicles[PLAYER]);
        if (MODE == 7) {
            MODE = result;
        }
        active = 1;
        switch (D_800F4D8A) {
        case 7:
            row = GROUP_ROW(PLAYER, (s16)(ovl661_D_80118450[PLAYER] ^ 1), 0);
            if (UiRows[row].byte14 == 5) {
                D_800F4D8A = 0;
            }
        case 0:
            SetMenuFrameOptions(1, 1, 1);
            before = PackChoiceGlyph(SELECTION + D_800AC5D4 * 4);
            StepMenuValue(PLAYER * 4 | 1, 2, (char *)&ovl661_D_8011844C[PLAYER],
                          ovl661_D_80118440[PLAYER][ovl661_D_80118450[PLAYER]], 0);
            UnpackChoiceGlyph(SELECTION + D_800AC5D4 * 4,
                          ovl661_D_80118458[PLAYER][ovl661_D_80118450[PLAYER]][ovl661_D_8011844C[PLAYER]].glyph);
            if ((s16)before != (s16)PackChoiceGlyph(SELECTION + D_800AC5D4 * 4)) {
                ResetScreenCounters();
                LoadCursor = MenuArenas.first;
                UnpackChoiceGlyph(SELECTION + D_800AC5D4 * 4,
                              ovl661_D_80118458[PLAYER][ovl661_D_80118450[PLAYER]][ovl661_D_8011844C[PLAYER]].glyph);
                PlayerCourseChoices[D_800AC5D4].first = ovl661_D_80118458[PLAYER][ovl661_D_80118450[PLAYER]][ovl661_D_8011844C[PLAYER]].first;
                PlayerCourseChoices[D_800AC5D4].second = ovl661_D_80118458[PLAYER][ovl661_D_80118450[PLAYER]][ovl661_D_8011844C[PLAYER]].second;
                MODE = 6;
            }
            row = GROUP_ROW(PLAYER, ovl661_D_80118450[PLAYER], MenuCursor);
            UiRows[row].cells[0] = 0;
            StepMenuValue(PLAYER * 4, 2, (char *)&MenuCursor, ovl661_D_80118358[ovl661_D_80118450[PLAYER]], 0);
            row = GROUP_ROW(PLAYER, ovl661_D_80118450[PLAYER], MenuCursor);
            UiRows[row].cells[0] = 2;
            i = UiRows[row].byte14;
            close = 0;
            if (i != 2) {
                break;
            }
            if (BadgeRows->state10 != 3) {
                break;
            }
            {
                ControllerState5C *inputs = PlayerControllers;
                input = inputs + PLAYER;
            }
            if (input->action28 != 0) {
                PlaySfx(1);
                UiRows[row].cells[0] = 1;
                if (MenuCursor == ovl661_D_8011835C[ovl661_D_80118450[PLAYER]][0]) {
                    ovl661_D_80118438 = i;
                    D_800F4D8A = 4;
                } else if (MenuCursor == ovl661_D_8011835C[ovl661_D_80118450[PLAYER]][1]
                           && ovl661_D_80118458[D_800AC5D4][1][0].glyph != -1) {
                    ovl661_D_8011844C[PLAYER] = 0;
                    MenuCursor = 0;
                    for (i = 0; i < 4; i++) {
                        row = GROUP_ROW(PLAYER, ovl661_D_80118450[PLAYER], i);
                        if (row == -1) {
                            break;
                        }
                        if (UiRows[row].byte14 == 2) {
                            UiRows[row].byte14 = 3;
                        }
                    }
                    for (i = 0; i < 4;) {
                        row = GROUP_ROW(PLAYER, (s16)(ovl661_D_80118450[PLAYER] ^ 1), i);
                        i++;
                        if (row == -1) {
                            break;
                        }
                        UiRows[row].byte14 = 0;
                    }
                    D_800F4D8A = 7;
                    ovl661_D_80118450[PLAYER] ^= 1;
                } else if (MenuCursor == ovl661_D_8011835C[ovl661_D_80118450[PLAYER]][2]) {
                    close = 1;
                } else if (MenuCursor == ovl661_D_8011835C[ovl661_D_80118450[PLAYER]][3]) {
                    for (i = 0; i < UiRowCount; i++) {
                        if (UiRows[i].byte14 == 2) {
                            UiRows[i].byte14 = 3;
                        }
                    }
                    MODE = 3;
                    D_800AC5D0 = 0x40;
                    D_800F4D8A = 10;
                    BadgeRows->state10 = 4;
                }
            } else if (input->action2C != 0) {
                PlaySfx(2);
                close = 1;
            }
            if (close == 1) {
                for (i = 0; i < UiRowCount; i++) {
                    if (UiRows[i].byte14 == 2) {
                        UiRows[i].byte14 = 3;
                    }
                }
                MODE = 3;
                D_800AC5D0 = 0x40;
                BadgeRows->state10 = 4;
                SeqFadeVolume(0, 60);
                D_800F4D8A = 8;
            }
            break;
        case 3:
            row = GROUP_ROW((D_800AC5D4 - 1) & 1, 1, 3) + 1;
            if (UiRows[row].byte14 == 5) {
                ovl661_D_80118438 = 1;
                D_800F4D8A = 0;
            }
            break;
        case 4:
            row = GROUP_ROW(PLAYER, 1, 3) + 1;
            i = UiRows[row].byte14;
            if (i != 2) {
                break;
            }
            UiRows[row + ENTRIES[D_800AC5D4].reserved].cells[0] = 0;
            StepMenuValue(PLAYER * 4, 1, (char *)&ENTRIES[D_800AC5D4].reserved, 2, 0);
            UiRows[row + ENTRIES[D_800AC5D4].reserved].cells[0] = i;
            {
                ControllerState5C *inputs = PlayerControllers;
                input = inputs + PLAYER;
            }
            if (input->action28 != 0) {
                PlaySfx(1);
                UiRows[row + ENTRIES[0].reserved].cells[0] = 1;
                UiRows[row].byte14 = 3;
                UiRows[row + 1].byte14 = 3;
                for (i = 0; i < 4; i++) {
                    row = GROUP_ROW(PLAYER, ovl661_D_80118450[PLAYER], i);
                    if (row == -1) {
                        break;
                    }
                    if (UiRows[row].byte14 == 2) {
                        UiRows[row].byte14 = 3;
                    }
                }
                active = 0;
                MenuArenas.first = LoadCursor;
                if ((D_800AC5D4 & 1) > 0) {
                    D_800F4D8A = 5;
                    for (i = 0; i < UiRowCount; i++) {
                        if (UiRows[i].byte14 == 2) {
                            UiRows[i].byte14 = 3;
                        }
                    }
                    MODE = 3;
                    BadgeRows->state10 = 4;
                } else {
                    D_800F4D8A = 3;
                    D_800AC5D4++;
                    ovl661_func_80115F8C();
                }
            } else if (input->action2C != 0) {
                PlaySfx(2);
                UiRows[row].byte14 = 3;
                UiRows[row + 1].byte14 = 3;
                D_800F4D8A = 6;
            }
            break;
        case 5:
            SetMenuFrameOptions(0, 1, 1);
            row = GROUP_ROW(PLAYER, 1, 3) + 1;
            status = UiRows[row].byte14;
            if (status == 5 && MODE == status) {
                ResetScreenCounters();
                ovl661_func_80117350();
                ModeGroup = 1;
                D_800F3A26 = 0x39;
                D_800F4D8A = 0;
            }
            break;
        case 6:
            row = GROUP_ROW(PLAYER, 1, 3) + 1;
            if (UiRows[row].byte14 == 5) {
                D_800F4D8A = 0;
                ovl661_D_80118438 = 1;
                UiRows[row].byte14 = 0;
                UiRows[row + 1].byte14 = 0;
            }
            break;
        case 9:
            row = GROUP_ROW(PLAYER, 1, 3) + 1;
            status = UiRows[row].byte14;
            if (status == 5 && MODE == status) {
                ovl661_func_80115598(0);
                D_800F4D8A = 0;
            }
            break;
        case 8:
            SetMenuFrameOptions(0, 1, 1);
            D_800AC5D0--;
            SetModelTexturePage(-1);
            AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, (0x40 - D_800AC5D0) * 4, 0x45);
            if (D_800AC5D0 <= 0) {
                SeqMuteVolume();
                ModeGroup = 1;
                D_800F3A26 = 3;
                D_800F4D8A = 0;
            }
            break;
        case 10:
            SetMenuFrameOptions(0, 1, 1);
            SetModelTexturePage(-1);
            D_800AC5D0--;
            AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, (0x40 - D_800AC5D0) * 4, 0x45);
            if (D_800AC5D0 <= 0) {
                CardMenuOperation = 2;
                ModeGroup = 1;
                D_800F3A26 = 0x47;
                D_800F4D8A = 0;
            }
            break;
        }
    }
    if (ovl661_D_80118438 == 0 && MODE == 2) {
        ovl661_D_80118438 = 1;
        BadgeRows->half02 = 1;
    }
    if (D_800F4D8A == 3) {
        player = D_800AC5D4 - 1;
    } else {
        player = D_800AC5D4;
    }
    if (D_800F4D8A == 7) {
        group = ovl661_D_80118450[player & 1] ^ 1;
        row = GROUP_ROW(player & 1, group, 0);
        if (UiRows[row].byte14 == 5) {
            group = ovl661_D_80118450[player & 1];
        }
    } else {
        group = ovl661_D_80118450[player & 1];
    }
    for (i = 0; i < 4; i++) {
        row = GROUP_ROW(player & 1, group, i);
        if (row == -1) {
            break;
        }
        DrawUiRow(row);
    }
    if (ovl661_D_80118438 == 2) {
        last = GROUP_ROW(player & 1, 1, 3);
        DrawUiRow(last + 1);
        DrawUiRow(last + 2);
    }
    if ((u8)MODE - 1 < 2U) {
        i = active;
        if (i == 1) {
            DrawSelectionIconGrid(D_800AC5D4);
            UpdateCarWorldTransform(&PlayerCars[PLAYER]);
            brightness = 0x100;
            if (MODE == i) {
                last = ~((ImageRows[2] * 0xFF) / 30);
                color.channels.r = last;
                color.channels.g = last;
                color.channels.b = last;
                flags = 0x102;
                brightness = (ImageRows[2] << 8) / 30;
            } else {
                flags = 1;
                color.channels.r = 0x80;
                color.channels.g = 0x80;
                color.channels.b = 0x80;
            }
            DrawPreviewCar(&PlayerCars[PLAYER], color.argument, brightness, flags);
        }
    }
    UpdateBadgeRowAnim(0);
    if (UpdateImageRow(0) > 0) {
        DrawMenuStepArrows(0xA0, 0x90, 8, 1, ovl661_D_8011844C[PLAYER], 0,
                      ovl661_D_80118440[PLAYER][ovl661_D_80118450[PLAYER]]);
    }
    D_800F4CB8 = D_800AC5D4 + 1;
    sound = 0x10032;
    /* Integer base address: the original's table read is not disambiguated
     * from the D_800F4CB8 store above. */
    groups = ovl661_D_80118450;
    if (*(s16 *)(groups + (D_800AC5D4 & 1) * 2) != 0) {
        sound = 0x10036;
    }
    UpdateScreenFade(sound);
}
