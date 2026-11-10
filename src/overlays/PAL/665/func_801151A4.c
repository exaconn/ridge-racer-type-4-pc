#include "game/system/pad.h"
#include "rr/overlay665_audio_options.h"
#include "rr/frame_storage_types.h"

void ovl665_func_801151A4(void) {
    s16 openingTimer;
    s32 signedTimer;
    s16 closingTimer;
    s32 closeIndex;
    s32 clearIndex;
    register s32 row __asm__("$16");
    s32 detailIndex;
    u32 layout;
    u8 monoState;
    u8 musicState;
    u8 effectsState;
    u8 detailState;
    UiRow24 *detail;
    UiRow24 *closingRow;
    UiRow24 *clearingRow;

    layout = ovl665_D_80119768.layout0C;
    switch (layout) {
    case 0:
        row = 0;
        ovl665_func_80114FD8();
        D_800AC5D0 = 0x40;
        ovl665_D_80119768.layout0C = (ovl665_D_80119768.layout0C + 1U);
        break;
    case 1:
        signedTimer=D_800AC5D0;
        __asm__("" : "+r"(signedTimer));
        openingTimer = (u16) D_800AC5D0 - 1;
        if (signedTimer > 0) {
            D_800AC5D0 = openingTimer;
            AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[702], openingTimer*4, 0x45);
        }
        SetPresentationDrawEnabled(1);
        if (UiRows[0].byte14 == 2) {
            row = 0;
            if (D_800AC5D0 == 0) {
                ovl665_D_80119768.layout0C = (ovl665_D_80119768.layout0C + 1U);
            }
        } else {
            goto reset_rows;
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
            row = 0;
        } else {
            if (PlayerControllers[0].action2C != 0) {
                PlaySfx(2);
                D_800AC5D0 = 0x40;
                ovl665_D_80119768.layout0C = 6;
            }
            goto reset_rows;
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
        row = 0;
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
        row = 0;
        break;
    case 5:
        monoState = UiRows[6].byte14;
        if (monoState == 2) {
            UiRows[Options.audio.mono32+6].cells[0] = 0;
            StepMenuValue(0, 2, (char *)&Options.audio.mono32, 2, 0);
            UiRows[Options.audio.mono32+6].cells[0] = monoState;
            if (D_800F3A60 != 0) {
                PlaySfx(1);
                UiRows[Options.audio.mono32+6].cells[0] = 1;
                ovl665_D_80119768.layout0C = 7U;
                SetMenuFrameOptions(1, 1, 1);
                SetCdMixMode(Options.audio.mono32);
            }
        }
        DrawUiRow(6);
        DrawUiRow(7);
        row = 0;
        break;
    case 6:
        row = 0;
        closeIndex = 0;
        do {
            closingRow = (UiRow24 *)((closeIndex*sizeof(UiRow24))+(unsigned long)UiRows);
            if (closingRow->byte14 == 2) {
                closingRow->byte14 = 3U;
            }
            row += 1;
            closeIndex++;
        } while (row < 4);
        D_800AC5D0 = 0x40;
        ovl665_D_80119768.layout0C = 9U;
        goto reset_rows;
    case 7:
        row = 4;
        detailIndex = 4;
        do {
            detail = (UiRow24 *)((detailIndex*sizeof(UiRow24))+(unsigned long)UiRows);
            detailState = detail->byte14;
            if (detailState == 2) {
                detail->byte14 = 3U;
            } else if (detailState == 6) {
                ovl665_D_80119768.layout0C = 8;
            }
            if ((u8) UiRows[detailIndex].byte14 >= 2U) {
                DrawUiRow(row);
            }
            row += 1;
            detailIndex++;
        } while (row < 8);
        row = 0;
        break;
    case 8:
        row = 4;
        clearIndex = 4;
        do {
            clearingRow = (UiRow24 *)((clearIndex*sizeof(UiRow24))+(unsigned long)UiRows);
            if (clearingRow->byte14 != 0) {
                clearingRow->byte14 = 0U;
            }
            row += 1;
            clearIndex++;
        } while (row < 8);
        ovl665_D_80119768.layout0C = 1U;
        goto reset_rows;
    case 9:
        closingTimer = (u16) D_800AC5D0 - 1;
        D_800AC5D0 = closingTimer;
        AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (0x40 - closingTimer) * 4, 0x45);
        row = 0;
        if (D_800AC5D0 <= 0) {
            ovl665_func_80114A24();
            ovl665_D_80119768.layout0C = 0;
            ovl665_D_80119768.scene00 = (ovl665_D_80119768.scene00 - 1U);
            goto reset_rows;
        }
        break;
    default: goto reset_rows;
    }
    goto draw_rows;
reset_rows:
    row=0;
draw_rows:
    do {
        DrawUiRow(row);
        row += 1;
    } while (row < 4);
    UpdateScreenFade(0x1006A);
}
