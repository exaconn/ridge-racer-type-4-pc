#include "rr/overlay663_menu_handler.h"
#include "rr/tables.h"
extern RrMenuItemState5C D_800F3A38[2];
void func_80053AF4(s32, s32);
void func_80053CB0(void);
void ovl663_func_801157B8(void);
void ovl663_func_80114B98(void) {
    s32 cancelSound;
    s16 selected;
    s32 loaded;
    s16 closingTimer;
    s16 cancelTimer;
    s32 i;

    s32 closingMode;
    s32 highlight;
    u8 rowState;
    RrUiRow24 *row;

    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    D_800ABEB0 = (s32)((u32)D_800ABEB0 + 1U);
    switch (D_800F4D8A) {
    case 0:
        i = 0;
        if (D_800F4D80->byte14 == 2) {
            func_8002AF8C(1, 1, 1);
            D_800F4D80[D_800AC64A].cells[0] = 0;
            func_8002B330(0, 2, (char *)&D_800AC64A, 4, 0);
            highlight = 2;
            if (ovl663_D_801168CC == 0) {
                if (D_800AC64A == 0) {
                    highlight = 4;
                }
            }
            D_800F4D80[D_800AC64A].cells[0] = highlight;
            closingMode = 0;
            if (D_800F3A38[0].a != 0) {
                D_800F4D80[D_800AC64A].cells[0] = 1;
                selected = D_800AC64A;
                if (selected == 0) {
                    if (ovl663_D_801168CC > 0) {
                        func_8004F24C(1);
                        closingMode = 2;
                        func_8003A198();
                        D_800F4D8A = 3;
                        D_800ACC18 = D_800F4D40;
                    } else {
                        func_8004F24C(4);
                    }
                    goto chosen;
                } else if (selected == 1) {
                    func_8004F24C(1);
                    closingMode = 2;
                    ovl663_D_801168C8 = selected;
                    D_800F4D8A = 4;
                    goto chosen;
                } else if (selected == 2) {
                    func_8004F24C(1);
                    ovl663_D_801168C8 = selected;
                    closingMode = 2;
                    D_800F4D8A = 5;
                    goto chosen;
                } else {
                    if (selected != 3) goto chosen;
                    cancelSound = 1;
                    goto cancel;
                }
            } else {
                cancelSound = 2;
                if (D_800F3A38[0].b != 0) {
cancel:
                    closingMode = 1;
                    func_8004F24C(cancelSound);
                    D_800F4D8A = 8;
                    /* ASM_BARRIER: retain the initialized closing kind after cancellation. */
                    __asm__("" : "+r"(closingMode));
                }
            }
chosen:
            if (closingMode > 0) {
                func_8002AF8C(0, 1, 1);
                D_800AC5D0 = 0x40;
                i = 0;

                do {
                    row = (RrUiRow24 *)(i * sizeof(RrUiRow24) + (unsigned long)D_800F4D80);
                    rowState = row->byte14;
                    if (rowState == 2) {
                        if (closingMode == rowState) {
                            row->byte14 = 7U;
                        } else {
                            row->byte14 = 3U;
                            func_80053AF4(0, (s16) (D_800AC5D0 - 4));
                        }
                    }
                    i += 1;

                } while (i < 4);
                i = 0;
            }
        }
        break;
    case 3:
        if ((s16) D_800AC5D0 > 0) {
            D_800AC5D0 = (u16)D_800AC5D0 - 1;
        }
        func_800207D4(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70, (0x40 - (s16) D_800AC5D0) * 4, 0x45);
        i = 0;
        if ((s16) D_800AC5D0 <= 0) {
            loaded = func_8003BEA8(ovl663_D_801168A0[0]);
            if (loaded == 1) {
                ovl663_func_801157B8();
                D_800F4C6A = loaded;
                D_800F3A26 = 0x4C;
                D_800F4D8A = 0;
                return;
            }
            break;
        }
        break;
    case 4:
    case 5:
        closingTimer = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = (u16) closingTimer;
        func_800207D4(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70, (0x40 - closingTimer) * 4, 0x45);
        i = 0;
        if ((s16) D_800AC5D0 <= 0) {
            ovl663_func_80115028(ovl663_D_801168C8);
            D_800F4C6A = 1;
            D_800F3A26 = 75;
            D_800F4D8A = 0;
        }
        break;
    case 8:
        cancelTimer = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = (u16) cancelTimer;
        func_800207D4(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70, (0x40 - cancelTimer) * 4, 0x45);
        i = 0;
        if ((s16) D_800AC5D0 <= 0) {
            func_80053CB0();
            D_800F4C6A = 1;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        }
        break;
    default:
        break;
    }
    i = 0;
    do {
        func_8003EC88(i);
        i += 1;
    } while (i < 4);
    func_8002A344(0x10057);
}
