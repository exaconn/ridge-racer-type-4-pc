#include "game/system/pad.h"
#include "rr/overlay662_palette_controller.h"

void ovl662_update_emblem_selector(void) {
    s32 badgePhase;
    s32 ready;
    void *palette;
    s32 paletteIndex;
    s32 index;
    s8 highlight;
    u16 previous;
    s32 rowPhase;
    s16 key;
    UiRow24 *acceptRow;
    UiRow24 *closingRow;
    UiRow24 *cancelRow;

    ModeFrame = ModeFrame + 1U;
    ScreenFrame = ScreenFrame + 1U;
    switch ((u16)SubmodeState) {
    case 0:
        if (ovl662_emblem_edit_from_menu == 0) SetMenuFrameOptions(1,1,1);
        else SetMenuFrameOptions(1,1,0);
        previous = ovl662_selected_emblem_slot;
        if (ovl662_emblem_preview_fade >= 0x11) {
            StepMenuValue(1, 2, (char *)&ovl662_selected_emblem_slot, 0x19, 0);
        }
        if ((s16) previous != (s16) ovl662_selected_emblem_slot) {
            key = (s8)ovl662_emblem_choice_map[(s16)ovl662_selected_emblem_slot];
            ovl662_selected_emblem_id = key;
            if (key == 0) {
                palette = CustomEmblemTimPtr;
                paletteIndex = 0;
            } else {
                palette = PresetEmblemTimPtr;
                paletteIndex = key - 1;
            }
            UploadCarClut(palette, paletteIndex, 0);
            ovl662_emblem_preview_fade = 0;
        }
        UiRows[(s16)MenuCursor].cells[0] = 0;
        StepMenuValue(0, 2, (char *)&MenuCursor, MenuItemCount, 0);
        highlight = 2;
        if (ovl662_selected_emblem_id >= 0x19) {
            UiRows->half20 = 1;
            if ((s16) MenuCursor == 0) {
                highlight = 4;
            }
        } else {
            UiRows->half20 = 0;
        }
        UiRows[(s16)MenuCursor].cells[0] = highlight;
        rowPhase = UiRows->byte14;
        if (rowPhase == 2) {
            badgePhase = BadgeRows->state10;
            if (badgePhase == 3) {
                if (PlayerControllers[0].action28 != 0) {
                    UiRows[(s16)MenuCursor].cells[0] = 1;
                    if ((s16) MenuCursor == 0) {
                        if (ovl662_selected_emblem_id < 0x19) {
                            PlaySfx(1);
                            index = 0;
                            ovl662_menu_substate = (s16) rowPhase;
                            ovl662_emblem_preview_fade = 0;
                            SubmodeState = (u16) badgePhase;
                            do {
                                acceptRow = (UiRow24 *)(index * sizeof(UiRow24) + (unsigned long)UiRows);
                                if (acceptRow->byte14 == 2) {
                                    acceptRow->byte14 = 3U;
                                }
                                index += 1;
                            } while (index < 2);
                            BadgeRows->state10 = 4;
                        } else {
                            PlaySfx(4);
                        }
                    } else if ((s16) MenuCursor == 1) {
                        PlaySfx(1);
                        index = 0;
                        ovl662_emblem_preview_fade = 0;
                        ovl662_menu_substate = (s16) rowPhase;
                        BadgeRows->state10 = 4;
                        do {
                            closingRow = (UiRow24 *)(index * sizeof(UiRow24) + (unsigned long)UiRows);
                            if (closingRow->byte14 == 2) {
                                closingRow->byte14 = 3U;
                            }
                            index += 1;
                        } while (index < 2);
                        goto closeMenu;
                    }
                } else if (PlayerControllers[0].action2C != 0) {
                    if (ovl662_emblem_edit_from_menu == 0) {
                        PlaySfx(2);
                        index = 0;
                        ovl662_menu_substate = (s16) rowPhase;
                        ovl662_emblem_preview_fade = 0;
                        BadgeRows->state10 = 4;
                        do {
                            cancelRow = (UiRow24 *)(index * sizeof(UiRow24) + (unsigned long)UiRows);
                            if (cancelRow->byte14 == 2) {
                                cancelRow->byte14 = 3U;
                            }
                            index += 1;
                        } while (index < 2);
closeMenu:
                        SubmodeState = 4;
                    }
                }
            }
        }
        goto drawMenu;
    case 3:
        if (ovl662_emblem_edit_from_menu == 0) SetMenuFrameOptions(0,1,1);
        else SetMenuFrameOptions(0,1,0);
        if ((UiRows->byte14 == 5) && (BadgeRows->state10 == 11)) {
            PlayerCourseChoices[0].second = (u16)ovl662_selected_emblem_id+1;
            if (ovl662_emblem_edit_from_menu == 0) goto enterPalette;
            goto publishScene;
        }
        goto drawMenu;
    case 4:
        if (ovl662_emblem_edit_from_menu == 0) SetMenuFrameOptions(0,1,1);
        else SetMenuFrameOptions(0,1,0);
        if ((UiRows->byte14 == 5) && (BadgeRows->state10 == 11)) {
            if (ovl662_emblem_edit_from_menu == 0) {
enterPalette:
                ovl662_init_garage_car_view();
            } else {
publishScene:
                ModeGroup=3;ModeIndex=1;SubmodeState=0;
            }
            break;
        }
        goto drawMenu;
    default:
drawMenu:
        if (ovl662_menu_substate == 0) {
            ready = UpdateRect16Row(0);
            if (ready == 1) {
                ovl662_menu_substate = ready;
            }
        } else {
            ovl662_draw_emblem_preview_fade(ovl662_menu_substate - 1);
            DrawMenuStepArrows(0xA0, 0xC0, 0x30, 0, (s16) ovl662_selected_emblem_slot, 0, ovl662_emblem_choice_count);
        }
        index = 0;
        if (MenuItemCount > 0) {
            do {
                DrawUiRow(index);
                index += 1;
            } while (index < MenuItemCount);
        }
        UpdateBadgeRowAnim(0);
        break;
    }
    UpdateScreenFade(0x1004A);
}
