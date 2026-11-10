/* CC1_FLAGS: -fno-strength-reduce */

#include "rr/menu_transition.h"

/* legacy typing of shared globals in this part of the unit */
#define SetDispMask ((int (*)())SetDispMask)

#define IntroScriptStep ((int (*)())IntroScriptStep)

#define MenuArenas (*(int (*))&MenuArenas)

#define ModeGroup (*(short (*))&ModeGroup)

#undef SetDispMask

#undef IntroScriptStep

#undef MenuArenas

#undef ModeGroup

void UpdateMenuScript(void) {
    s32 next_frame;
    s32 mode;
    s32 status;
    s8 next_step;
    s8 opcode;
    s32 step;

    next_frame = (s32)((u32)ModeFrame + 1U);
    ModeFrame = next_frame;
    if (next_frame == 2) {
        SetDispMask(1);
    }
    if (GameModeType == 4) {
        UpdateLinkHandshake(1);
    }
    mode=MenuTransitionOverlayMode;
    if (mode == 1) {
        status = UpdateRaceOpeningOverlay(ModeFrame);
    } else {
        if (mode == 0) {
            status = IntroScriptStep();
        } else {
            status = 1;
        }
    }
    step = MenuScriptStep;
    if (step >= 0) {
        if (MenuScriptOps[MenuScripts[MenuScriptId][step]]() == 1) {
            LoadState = 0;
            next_step = (u8) MenuScriptStep + 1;
            MenuScriptStep = next_step;
            opcode = MenuScripts[MenuScriptId][next_step];
            if (opcode < 0) {
                MenuScriptId = (u16) -opcode;
                MenuScriptStep = -1;
                if (MenuTransitionOverlayMode == 0) {
                    StartDemoFadeOutAndCloseVab();
                }
            }
        }
    } else {
        if((status==1 && MenuTransitionOverlayMode==status) || (MenuTransitionOverlayMode==0 && status==2) || (MenuTransitionOverlayMode>=2 && status==1)) {
            MenuSceneEntries[MenuScriptId]();
            SubmodeState = 0;
            ModeGroup = (u16) ((u16) ActiveSceneId >> 8);
            ModeIndex = ActiveSceneId & 0xFF;
        }
    }
    if (ModeGroup != 1) {
        *(void **)0x1F800000 = AddTileGrid2x2((u32 *)((u8 *)CurrentDrawBuf + 0xB6C), *(void **)0x1F800000, 0, 0, 0x140, 0x200, 0, 0, 0);
    }
}
