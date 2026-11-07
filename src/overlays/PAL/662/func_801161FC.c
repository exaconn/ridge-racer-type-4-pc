#include "rr/overlay662_progress_controller.h"

void ovl662_func_801161FC(void)
{
    s32 ready, changed, state, i, flags;

    ModeFrame = (s32)((u32)ModeFrame + 1U);
    ScreenFrame = (s32)((u32)ScreenFrame + 1U);
    switch ((u16)D_800F4D8A) {
    case 0:
        ready = func_8003B47C();
        if (ready == 1) {
            SetMenuFrameOptions(1, 1, 1);
            D_800F4D8A = ready;
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
            if (D_800F3A38[0].a) {
                if ((s16)ovl662_D_801272D8[SelectionRows.bytes[0]] > 0) {
                    PlaySfx(1);
                    ModeFrame = 0;
                    D_800F4D8A = 2;
                    UiRows->byte14 = state;
                    BadgeRows->state10 = 4;
                } else {
                    PlaySfx(4);
                }
            } else if (D_800F3A38[0].b) {
                PlaySfx(2);
                D_800F4D8A = 8;
                D_800AC5D0 = 64;
                UiRows->byte14 = state;
                BadgeRows->state10 = 4;
            }
        }
        flags = 0x10002;
        break;
    case 2:
        if (BadgeRows->state10 == 11 && UiRows->byte14 == 5) {
            ovl662_func_80115EC4(0);
            D_800F4D8A = 3;
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
            if (D_800F3A38[0].a) {
                if ((s16)ovl662_D_80127328[SelectionRows.bytes[0]][SelectionRows.bytes[1]] > 0) {
                    PlaySfx(1);
                    ModeFrame = 0;
                    D_800F4D8A = 4;
                    UiRows->byte14 = state;
                    BadgeRows->state10 = 4;
                } else {
                    PlaySfx(4);
                }
            } else if (D_800F3A38[0].b) {
                PlaySfx(2);
                ovl662_D_801272D4 = 1;
                D_800F4D8A = 4;
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
                GlyphRows[i].phase18 = (s32)((u32)GlyphRows[i].phase18 - 0x30000U);
            }
            ovl662_func_80116020();
            D_800F4D8A = 1;
            ovl662_D_801272D4 = 0;
        }
        flags = 0x10006;
        break;
    case 8:
        D_800AC5D0--;
        SetMenuFrameOptions(0, 1, 1);
        AddScreenFadeTiles(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70,
                     (64 - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            ovl662_func_8011512C();
            ModeGroup = 1;
            D_800F3A26 = 64;
            D_800F4D8A = 0;
        }
        flags = 0x10002;
        break;
    }
    DrawUiRow(0);
    UpdateBadgeRowAnim(0);
    UpdateScreenFade(flags);
}
