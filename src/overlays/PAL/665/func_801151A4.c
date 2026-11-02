#include "rr/overlay665_audio_options.h"

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
    RrUiRow24 *detail;
    RrUiRow24 *closingRow;
    RrUiRow24 *clearingRow;

    layout = (u32)ovl665_D_80119768.layout0C;
    switch (layout) {
    case 0:
        row = 0;
        ovl665_func_80114FD8();
        D_800AC5D0 = 0x40;
        ovl665_D_80119768.layout0C = (s32) ((u32)ovl665_D_80119768.layout0C + 1U);
        break;
    case 1:
        signedTimer=D_800AC5D0;
        __asm__("" : "+r"(signedTimer));
        openingTimer = (u16) D_800AC5D0 - 1;
        if (signedTimer > 0) {
            D_800AC5D0 = openingTimer;
            func_800207D4(&((RrFrameOrderingPrefix *)(void *)D_800ACC14)->ordering70[702], (s32)openingTimer*4, 0x45);
        }
        func_8005B2D4(1);
        if (D_800F4D80[0].byte14 == 2) {
            row = 0;
            if (D_800AC5D0 == 0) {
                ovl665_D_80119768.layout0C = (s32) ((u32)ovl665_D_80119768.layout0C + 1U);
            }
        } else {
            goto reset_rows;
        }
        break;
    case 2:
        D_800F4D80[ovl665_D_80119768.exit34.word].cells[0] = 0;
        func_8002B330(0, 2, (char *)&ovl665_D_80119768.exit34.halves[0], 4, 0);
        D_800F4D80[ovl665_D_80119768.exit34.word].cells[0] = 2;
        if (D_800F3A38[0].a != 0) {
            func_8004F24C(1);
            D_800F4D80[ovl665_D_80119768.exit34.word].cells[0] = 1;
            ovl665_D_80119768.layout0C = (s32) (ovl665_D_80119768.exit34.word + 3);
            func_8002AF8C(1, 1, 0);
            row = 0;
        } else {
            if (D_800F3A38[0].b != 0) {
                func_8004F24C(2);
                D_800AC5D0 = 0x40;
                ovl665_D_80119768.layout0C = 6;
            }
            goto reset_rows;
        }
        break;
    case 3:
        musicState = D_800F4D80[4].byte14;
        if (musicState == 2) {
            D_800F4D80[4].cells[0] = musicState;
            func_8002B330(1, -1, (char *)&D_800AD528.bytes[0x30], 0x11, 0);
            ovl665_D_80119754 = (s32) D_800AD528.bytes[0x30];
            func_80053A9C(D_800F7640);
            if (D_800F3A60 != 0) {
                func_8004F24C(1);
                D_800F4D80[4].cells[0] = 1U;
                ovl665_D_80119768.layout0C = 7U;
                func_8002AF8C(1, 1, 1);
            }
        }
        func_8003EC88(4);
        row = 0;
        break;
    case 4:
        effectsState = D_800F4D80[5].byte14;
        if (effectsState == 2) {
            D_800F4D80[5].cells[0] = effectsState;
            func_8002B330(1, -1, (char *)&D_800AD528.bytes[0x31], 0x11, 0);
            ovl665_D_80119758 = (s32) D_800AD528.bytes[0x31];
            if (D_800F3A60 != 0) {
                func_8004F24C(1);
                D_800F4D80[5].cells[0] = 1U;
                ovl665_D_80119768.layout0C = 7U;
                func_8002AF8C(1, 1, 1);
            }
        }
        func_8003EC88(5);
        row = 0;
        break;
    case 5:
        monoState = D_800F4D80[6].byte14;
        if (monoState == 2) {
            D_800F4D80[D_800AD528.audio.mono32+6].cells[0] = 0;
            func_8002B330(0, 2, (char *)&D_800AD528.audio.mono32, 2, 0);
            D_800F4D80[D_800AD528.audio.mono32+6].cells[0] = monoState;
            if (D_800F3A60 != 0) {
                func_8004F24C(1);
                D_800F4D80[D_800AD528.audio.mono32+6].cells[0] = 1;
                ovl665_D_80119768.layout0C = 7U;
                func_8002AF8C(1, 1, 1);
                func_8004FA98(D_800AD528.audio.mono32);
            }
        }
        func_8003EC88(6);
        func_8003EC88(7);
        row = 0;
        break;
    case 6:
        row = 0;
        closeIndex = 0;
        do {
            closingRow = (RrUiRow24 *)((unsigned long)(closeIndex*sizeof(RrUiRow24))+(unsigned long)D_800F4D80);
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
            detail = (RrUiRow24 *)((unsigned long)(detailIndex*sizeof(RrUiRow24))+(unsigned long)D_800F4D80);
            detailState = detail->byte14;
            if (detailState == 2) {
                detail->byte14 = 3U;
            } else if (detailState == 6) {
                ovl665_D_80119768.layout0C = 8;
            }
            if ((u8) D_800F4D80[detailIndex].byte14 >= 2U) {
                func_8003EC88(row);
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
            clearingRow = (RrUiRow24 *)((unsigned long)(clearIndex*sizeof(RrUiRow24))+(unsigned long)D_800F4D80);
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
        func_800207D4(&((RrFrameOrderingPrefix *)(void *)D_800ACC14)->ordering70[0], (0x40 - closingTimer) * 4, 0x45);
        row = 0;
        if (D_800AC5D0 <= 0) {
            ovl665_func_80114A24();
            ovl665_D_80119768.layout0C = 0;
            ovl665_D_80119768.scene00 = (s32) ((u32)ovl665_D_80119768.scene00 - 1U);
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
        func_8003EC88(row);
        row += 1;
    } while (row < 4);
    func_8002A344(0x1006A);
}
