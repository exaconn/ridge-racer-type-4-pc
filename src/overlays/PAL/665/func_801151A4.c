#include "game/system/pad.h"
#include "rr/overlay665_audio_options.h"
#include "rr/frame_storage_types.h"

void ovl665_func_801151A4(void) {
    s32 row;
    s16 closingTimer;
    u8 monoState;
    u8 musicState;
    u8 effectsState;
    u8 detailState;

    switch (ovl665_D_80119768.layout0C) {
    case 0:
        ovl665_func_80114FD8();
        FadeCountdown = 0x40;
        ovl665_D_80119768.layout0C = (ovl665_D_80119768.layout0C + 1U);
        break;
    case 1:
        if (FadeCountdown > 0) {
            FadeCountdown--;
            AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[702], FadeCountdown * 4, 0x45);
        }
        SetPresentationDrawEnabled(1);
        if (UiRows[0].byte14 == 2 && FadeCountdown == 0) {
            ovl665_D_80119768.layout0C = (ovl665_D_80119768.layout0C + 1U);
        }
        break;
    case 2:
        UiRows[ovl665_D_80119768.exit34.word].cells[0] = 0;
        StepMenuValue(0, 2, (char *)&ovl665_D_80119768.exit34.halves[0], 4, 0);
        UiRows[ovl665_D_80119768.exit34.word].cells[0] = 2;
        if (PlayerControllers[0].action28 != 0) {
            PlaySfx(1);
            UiRows[ovl665_D_80119768.exit34.word].cells[0] = 1;
            ovl665_D_80119768.layout0C = (ovl665_D_80119768.exit34.word + 3);
            SetMenuFrameOptions(1, 1, 0);
        } else if (PlayerControllers[0].action2C != 0) {
            PlaySfx(2);
            FadeCountdown = 0x40;
            ovl665_D_80119768.layout0C = 6;
        }
        break;
    case 3:
        musicState = UiRows[4].byte14;
        if (musicState == 2) {
            UiRows[4].cells[0] = musicState;
            StepMenuValue(1, -1, (char *)&Options.bytes[0x30], 0x11, 0);
            ovl665_D_80119754 = Options.bytes[0x30];
            SeqSetVolume(D_800F7640);
            if (D_800F3A60 != 0) {
                PlaySfx(1);
                UiRows[4].cells[0] = 1U;
                ovl665_D_80119768.layout0C = 7U;
                SetMenuFrameOptions(1, 1, 1);
            }
        }
        DrawUiRow(4);
        break;
    case 4:
        effectsState = UiRows[5].byte14;
        if (effectsState == 2) {
            UiRows[5].cells[0] = effectsState;
            StepMenuValue(1, -1, (char *)&Options.bytes[0x31], 0x11, 0);
            ovl665_D_80119758 = Options.bytes[0x31];
            if (D_800F3A60 != 0) {
                PlaySfx(1);
                UiRows[5].cells[0] = 1U;
                ovl665_D_80119768.layout0C = 7U;
                SetMenuFrameOptions(1, 1, 1);
            }
        }
        DrawUiRow(5);
        break;
    case 5:
        monoState = UiRows[6].byte14;
        if (monoState == 2) {
            UiRows[Options.audio.mono32 + 6].cells[0] = 0;
            StepMenuValue(0, 2, (char *)&Options.audio.mono32, 2, 0);
            UiRows[Options.audio.mono32 + 6].cells[0] = monoState;
            if (D_800F3A60 != 0) {
                PlaySfx(1);
                UiRows[Options.audio.mono32 + 6].cells[0] = 1;
                ovl665_D_80119768.layout0C = 7U;
                SetMenuFrameOptions(1, 1, 1);
                SetCdMixMode(Options.audio.mono32);
            }
        }
        DrawUiRow(6);
        DrawUiRow(7);
        break;
    case 6:
        for (row = 0; row < 4; row++) {
            if (UiRows[row].byte14 == 2) {
                UiRows[row].byte14 = 3U;
            }
        }
        FadeCountdown = 0x40;
        ovl665_D_80119768.layout0C = 9U;
        break;
    case 7:
        for (row = 4; row < 8; row++) {
            detailState = UiRows[row].byte14;
            if (detailState == 2) {
                UiRows[row].byte14 = 3U;
            } else if (detailState == 6) {
                ovl665_D_80119768.layout0C = 8;
            }
            if ((u8)UiRows[row].byte14 >= 2U) {
                DrawUiRow(row);
            }
        }
        break;
    case 8:
        for (row = 4; row < 8; row++) {
            if (UiRows[row].byte14 != 0) {
                UiRows[row].byte14 = 0U;
            }
        }
        ovl665_D_80119768.layout0C = 1U;
        break;
    case 9:
        closingTimer = (u16)FadeCountdown - 1;
        FadeCountdown = closingTimer;
        AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (0x40 - closingTimer) * 4, 0x45);
        if (FadeCountdown <= 0) {
            ovl665_func_80114A24();
            ovl665_D_80119768.layout0C = 0;
            ovl665_D_80119768.scene00 = (ovl665_D_80119768.scene00 - 1U);
        }
        break;
    }
    for (row = 0; row < 4; row++) {
        DrawUiRow(row);
    }
    UpdateScreenFade(0x1006A);
}
