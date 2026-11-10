#include "game/system/pad.h"
#include "rr/overlay662_progress_controller.h"
#include "rr/frame_storage_types.h"

void ovl662_func_801161FC(void)
{
    s32 ready, changed, state, i, flags;

    ModeFrame = (ModeFrame + 1U);
    ScreenFrame = (ScreenFrame + 1U);
    switch ((u16)SubmodeState) {
    case 0:
        ready = LoadGlyphRowsStep();
        if (ready == 1) {
            SetMenuFrameOptions(1, 1, 1);
            SubmodeState = ready;
        }
        flags = 0x10002;
        break;
    case 1:
        changed = ovl662_func_8011662C(ovl662_D_801272C0, 0);
        state = BadgeRows->state10;
        if (state == 3) {
            if (changed) {
                ovl662_func_80116690();
            }
            if (PlayerControllers[0].action28) {
                if ((s16)ovl662_D_801272D8[SelectionRows.bytes[0]] > 0) {
                    PlaySfx(1);
                    ModeFrame = 0;
                    SubmodeState = 2;
                    UiRows->byte14 = state;
                    BadgeRows->state10 = 4;
                } else {
                    PlaySfx(4);
                }
            } else if (PlayerControllers[0].action2C) {
                PlaySfx(2);
                SubmodeState = 8;
                FadeCountdown = 64;
                UiRows->byte14 = state;
                BadgeRows->state10 = 4;
            }
        }
        flags = 0x10002;
        break;
    case 2:
        if (BadgeRows->state10 == 11 && UiRows->byte14 == 5) {
            ovl662_func_80115EC4(0);
            SubmodeState = 3;
        }
        flags = 0x10006;
        break;
    case 3:
        changed = ovl662_func_8011662C(ovl662_D_801272C0, 1);
        state = BadgeRows->state10;
        if (state == 3) {
            if (changed) {
                ovl662_func_80116808();
            }
            if (PlayerControllers[0].action28) {
                if ((s16)ovl662_D_80127328[SelectionRows.bytes[0]][SelectionRows.bytes[1]] > 0) {
                    PlaySfx(1);
                    ModeFrame = 0;
                    SubmodeState = 4;
                    UiRows->byte14 = state;
                    BadgeRows->state10 = 4;
                } else {
                    PlaySfx(4);
                }
            } else if (PlayerControllers[0].action2C) {
                PlaySfx(2);
                ovl662_D_801272D4 = 1;
                SubmodeState = 4;
                UiRows->byte14 = state;
                BadgeRows->state10 = 4;
            }
        }
        flags = 0x10006;
        break;
    case 4:
        if (BadgeRows->state10 == 11 && UiRows->byte14 == 5) {
            i = 0;
            if (ovl662_D_801272D4 == 0) {
                ovl662_func_80116960();
                return;
            }
            for (; i < 4; i++) {
                GlyphRows[i].phase18 = (GlyphRows[i].phase18 - 0x30000U);
            }
            ovl662_func_80116020();
            SubmodeState = 1;
            ovl662_D_801272D4 = 0;
        }
        flags = 0x10006;
        break;
    case 8:
        FadeCountdown--;
        SetMenuFrameOptions(0, 1, 1);
        AddScreenFadeTiles(CurrentDrawBuf->buffer.ordering70.words,
                     (64 - FadeCountdown) * 4, 69);
        if (FadeCountdown <= 0) {
            ovl662_func_8011512C();
            ModeGroup = 1;
            ModeIndex = 64;
            SubmodeState = 0;
        }
        flags = 0x10002;
        break;
    }
    DrawUiRow(0);
    UpdateBadgeRowAnim(0);
    UpdateScreenFade(flags);
}
