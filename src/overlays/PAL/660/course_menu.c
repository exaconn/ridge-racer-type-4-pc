#include "rr/overlay660.h"

void ovl660_func_80114910(void) { D_800AD50A = 0x131; }

void ovl660_func_80114920(void) {
    RrProjectionScratchPrefix *scratch;
    s32 selection;
    s16 *result;
    D_800F2DE4=0;
    D_800ABEB0=0;
    D_800AC64A=0;
    D_800AC5E0=3;
    selection=func_80032E9C(D_800AC648);
    result=&D_800F3914;
    *result=selection;
    func_8003A198();
    D_800F4D70.second=D_800102F0+0x2D000;
    func_8003A1B4();
    D_800ACC18=D_800F4D70.second;
    func_8003E928(0,12,64,68,32,0,0,1,0,83);
    func_8003E928(1,12,104,68,32,0,0,1,0,102);
    func_8003E928(2,12,144,68,32,0,0,1,0,85);
    func_8003E928(3,12,368,208,32,0,0,8,4,D_800AC648&7);
    func_8003E928(4,24,88,68,32,0,0,1,0,84);
    func_8003E928(5,24,128,68,32,0,0,1,0,54);
    func_8003E928(6,24,168,68,32,0,0,1,0,85);
    func_8003E928(7,24,88,68,32,0,0,1,0,84);
    func_8003E928(8,24,128,68,32,0,0,1,0,53);
    func_8003E928(9,24,168,68,32,0,0,1,0,85);
    func_8003E928(10,116,88,28,32,0,0,1,0,55);
    func_8003E928(11,116,128,28,32,0,0,1,0,56);
    scratch=RR_PROJECTION_SCRATCH;
    scratch->word18=512;
    scratch->word0C=-416;
    scratch->word1C=0;
    scratch->word20=0;
    scratch->word08=0;
    scratch->word10=0;
    D_800ABEF8[0].pos20[0]=0;
    D_800ABEF8[0].pos20[1]=0;
    D_800ABEF8[0].pos20[2]=0;
}

void ovl660_func_80114C40(void)
{
    s32 val;

    SetDispMask(0);
    func_8002A604();
    ovl660_preview_bank = 0;
    ovl660_preview_selection = 0;
    ovl660_preview_confirmation = 0;
    func_8002B75C(ovl660_record_name, D_800F3920.prefix.progress.prefix00.fields.name);
    func_8001ECE8(0, 0, 0, 2);
    val = D_800F3920.prefix.progress.prefix00.fields.configuration0C;
    D_800AC648 = 0;
    ovl660_available_course_count = 4;
    if (val >= 16) {
        ovl660_available_course_count = 8;
    }
    if (val & 0x100) {
        ovl660_available_course_count = 16;
    }
    ovl660_func_80114920();
    func_80037040((s8*)&D_800F4D98,D_800ABE08[0][0].glyph);
    D_800F4C6A = 1;
    D_800F3A26 = 50;
    D_800F4D8A = 0;
    func_8001E84C(ovl660_title);
    func_80053BB4(0);
}

void ovl660_func_80114D38(void) {
    s32 loopCount;
    s16 cancelTimer;
    s16 settingsTimer;
    s32 row;
    s32 previewIndex;
    s32 settingsIndex;
    s32 cancelIndex;
    s32 backIndex;
    s32 cell;
    u16 previousCourse;
    u8 activeState;
    RrUiRow24 *preview;
    RrUiRow24 *settings;
    RrUiRow24 *cancel;
    RrUiRow24 *back;

    D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
    D_800ABEB0 = (s32)((u32)D_800ABEB0 + 1U);
    if (func_8003BAB0() == 1) {
        switch (D_800F4D8A) {
        case 0:
            func_8002AF8C(1, 1, 1);
            previousCourse = D_800AC648;
            func_8002B330(3, 2, (char *)&D_800AC648, ovl660_available_course_count, 0);
            if ((s16) previousCourse != (s16) D_800AC648) {
                func_8003A198();
            } else {
                func_80033368((s16) previousCourse, 0);
            }
            D_800F3914 = func_80032E9C((s16) D_800AC648);
            D_800F4D80[D_800AC64A].cells[0] = 0;
            func_8002B330(0, 2, (char *)&D_800AC64A, (s32) D_800AC5E0, 0);
            D_800F4D80[D_800AC64A].cells[0] = 2;
            cell = 0;
            if (D_800F4D80[3].byte02 != 0) {
                do {
                    if (cell == (D_800AC648 & 7)) {
                        D_800F4D80[3].cells[(s32)cell] = (s8) (((u8 *)&D_800AD522)[0] + 2);
                        D_800F4D80[3].byte16 = cell;
                    } else if (cell >= ovl660_available_course_count) {
                        D_800F4D80[3].cells[(s32)cell] = 1;
                    } else {
                        D_800F4D80[3].cells[(s32)cell] = (u8) ((u8 *)&D_800AD522)[0];
                    }
                    cell += 1;
                } while (cell < (s32) D_800F4D80[3].byte02);
            }
            if (D_800F3A60 != 0) {
                activeState = D_800F4D80[0].byte14;
                if (activeState == 2) {
                    func_8004F24C(1);
                    func_8002AF8C(0, 1, 1);
                    if ((s16) D_800AC64A == 0) {
                        row = 0;
                        previewIndex = 0;
                        D_800F4D80[0].cells[0] = 1;
                        do {
                            preview = (RrUiRow24 *)((unsigned long)(previewIndex*sizeof(RrUiRow24))+(unsigned long)D_800F4D80);
                            if (preview->byte14 == 2) {
                                preview->byte14 = 3U;
                            }
                            row += 1;
                            previewIndex++;
                        } while (row < 4);
                        D_800F4D8A = 3;
                    } else if ((s16) D_800AC64A == 1) {
                        row = 0;
                        if (D_800F4BEC > 0) {
                            settingsIndex = 0;
                            do {
                                settings = (RrUiRow24 *)((unsigned long)(settingsIndex*sizeof(RrUiRow24))+(unsigned long)D_800F4D80);
                                if (settings->byte14 == 2) {
                                    settings->byte14 = 3U;
                                }
                                loopCount=D_800F4BEC;
                                row += 1;
                                settingsIndex++;
                            } while (row < loopCount);
                        }
                        D_800AC5D0 = 0x40;
                        D_800F4D8A = 0xA;
                    } else if ((s16) D_800AC64A == activeState) {
                        row = 0;
                        if (D_800F4BEC > 0) {
                            cancelIndex = 0;
                            do {
                                cancel = (RrUiRow24 *)((unsigned long)(cancelIndex*sizeof(RrUiRow24))+(unsigned long)D_800F4D80);
                                if (cancel->byte14 == 2) {
                                    cancel->byte14 = 3U;
                                }
                                loopCount=D_800F4BEC;
                                row += 1;
                                cancelIndex++;
                            } while (row < loopCount);
                        }
                        D_800AC5D0 = 0x40;
                        D_800F4D8A = 8;
                        func_80053AF4(0, 0x3C);
                    }
                } else {
                    goto check_back;
                }
            } else {
check_back:
                if ((D_800F3A64 != 0) && (D_800F4D80[0].byte14 == 2)) {
                    func_8004F24C(2);
                    func_8002AF8C(0, 1, 1);
                    row = 0;
                    if (D_800F4BEC > 0) {
                        backIndex = 0;
                        do {
                            back = (RrUiRow24 *)((unsigned long)(backIndex*sizeof(RrUiRow24))+(unsigned long)D_800F4D80);
                            if (back->byte14 == 2) {
                                back->byte14 = 3U;
                            }
                                loopCount=D_800F4BEC;
                                row += 1;
                            backIndex++;
                        } while (row < loopCount);
                    }
                    D_800AC5D0 = 0x40;
                    D_800F4D8A = 8;
                }
            default:;
            }
            goto draw_course;
        case 2:
            if (D_800F3A60 != 0) {
                if (D_800F4D80[0].byte14 == 5) {
                    D_800F4D8A = 4;
                    func_8003E928(3, 0x78, 0xDC, 0x44, 0x20, 0, 0, 1, 0, 0, 0, 0);
                    func_8003E928(4, 0x78, 0x104, 0x44, 0x20, 0, 0, 1, 0, 0, 0, 0);
                }
            }
            goto draw_course;
        case 4:
            if (D_800F3A60 != 0) {
                if (D_800F4D80[3].byte14 == 2) {
                    D_800F4D8A = 5;
                    D_800F2AB0[1] = 3;
                    D_800F4D80[3].byte14 = 3U;
                    D_800F4D80[4].byte14 = 3;
                }
            } else {
            case 5:;
            }
            if (D_800F4D80[3].byte14 == 5) {
                D_800F4D80[0].byte14 = 0U;
                D_800F4D80[1].byte14 = 0;
                D_800F4D80[2].byte14 = 0;
                D_800F4D8A = 0;
            }
            goto draw_course;
        case 3:
            if (D_800F4D80[0].byte14 == 5) {
                ovl660_func_801163C4();
                D_800F4C6A = 1;
                D_800F3A26 = 0x33;
                D_800F4D8A=0;
                goto draw_rows;
            }
            break;
        case 8:
            cancelTimer = (u16)D_800AC5D0 - 1;
            D_800AC5D0 = (u16) cancelTimer;
            func_800207D4(((RrFrameOrderingPrefix *)(void *)D_800ACC14)->ordering70, (0x40 - cancelTimer) * 4, 0x45);
            if ((s16) D_800AC5D0 <= 0) {
                func_80053CB0();
                D_800F4C6A = 1;
                D_800F3A26 = 3;
                D_800F4D8A=0;
                goto draw_rows;
            }
            break;
        case 10:
            settingsTimer = (u16)D_800AC5D0 - 1;
            D_800AC5D0 = (u16) settingsTimer;
            func_800207D4(((RrFrameOrderingPrefix *)(void *)D_800ACC14)->ordering70, (0x40 - settingsTimer) * 4, 0x45);
            if ((s16) D_800AC5D0 <= 0) {
                D_800FFC18 = 1;
                D_800F3A1E = 0x132;
                D_800F4C6A = 1;
                D_800F3A26 = 0x47;
                D_800F4D8A=0;
                goto draw_rows;
            }
            break;
        }
    }
    else goto draw_course;
    goto draw_rows;

draw_course:
    func_80032F44((s16)D_800AC648,0);
draw_rows:
    func_8003EC88(0);
    func_8003EC88(1);
    func_8003EC88(2);
    func_8003EC88(3);
    if (D_800F4D80[0].byte14 == 2) {
        func_8002B12C(0xCE, 0xD0, 0x5E, 0, (s32) (s16) D_800AC648, -1, (s32)((u32)ovl660_available_course_count + 1U));
    }
    func_8002A344(0x10026);
}
