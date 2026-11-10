#include "rr/work_arena.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"
#include "types.h"
#include "scratchpad.h"
#include "game/menu/menu8.h"
#include "game/menu/menutran.h"
#include "game/menu/mnsel.h"
#include "rr/driver_control.h"
#include "rr/frame_environment.h"
#include "rr/menu_background.h"
#include "rr/selection.h"
#include "rr/bios_devices.h"
#include "game/ui/barsel0.h"
#include "game/data/screenframe.h"
#include "game/system/pad.h"
#include "rr/ui.h"
#include "rr/menu_grid.h"
#include "rr/menu_state_types.h"
#include "rr/course_choices.h"
#include "game/ui/uiwin.h"
#include "rr/screen_tiles.h"
#include "rr/badge_rows.h"
#include "game/menu/mnrows.h"

extern void ClearScreenOverlayState();
extern int SetMenuFrameOptions();
extern unsigned short D_800AC5D0;
extern int VSyncThreshold;
extern short MenuCursor;
extern int ModeFrame;
extern MenuSettings2C SaveSlotSelections[];
extern short D_800F3A26;
extern short ModeGroup;
extern unsigned short D_800F4D8A;
extern int DrawUiRow();
extern short CardMenuOperation;
extern int StepMenuValue();
extern void PlaySfx();
extern short MenuFrameState;
extern int MenuArenaBase;

void GpMenuModeEnter();
void GpMenuModeUpdate(void);
void DrawFirstThreeUiRows(void);
void StepGpMenuSelection();
void CarStandingsModeEnter(void);

void GpMenuModeEnter(void) {
    int a0, a1;
    short v;
    MenuSettings2C *p;
    SetDispMask(0);
    SetupDrawDispEnvs(0, 0, 0, 3);
    ModeFrame = 0;
    ScreenFrame = 0;
    D_800AC5D0 = 0;
    MenuCursor = 1;
    VSyncThreshold = 0x80;
    SelectionState[0x17] = -1;
    SelectionState[0x19] = 9;
    ResetRankEntries();
    ClearScreenOverlayState();
    a1 = 0;
    a0 = 0;
    p = SaveSlotSelections;
    do {
        a0++;
        if (p->phase17 > 0) goto T;
        p++;
    } while (a0 < 3);
    J:
    if (a1 == 0) {
        UpdateGrandPrixStageProgress();
        ModeGroup = 0;
        D_800F3A26 = 1;
        D_800F4D8A = 0;
        return;
    T:
        a1 = 1;
        goto J;
    }
    InitUiRow(0, 0x6C, 0xB8, 0x44, 0x20, 0, 0, 1, 0, 0x54);
    InitUiRow(1, 0x6C, 0xE0, 0x44, 0x20, 0, 0, 1, 0, 0x67);
    InitUiRow(2, 0x6C, 0x108, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    SetMenuFrameOptions(0, 1, 1);
    ModeGroup = 1;
    D_800F3A26 = 6;
    D_800F4D8A = 0;
}

void GpMenuModeUpdate(void) {
    StepGpMenuSelection();
    DrawFirstThreeUiRows();
    UpdateScreenFade(0x10086);
}

void DrawFirstThreeUiRows(void) {
    DrawUiRow(0);
    DrawUiRow(1);
    DrawUiRow(2);
}

void StepGpMenuSelection(void) {
    int s;
    int v, t;
    s = D_800F4D8A;
    if (s == 0) {
        UiRows[MenuCursor].cells[0] = 0;
        StepMenuValue(0, 2, &MenuCursor, 3, 0);
        UiRows[MenuCursor].cells[0] = 2;
        s = UiRows->byte14;
        if (s == 2) {
            SetMenuFrameOptions(1, 1, 1);
            if (PlayerControllers[0].action28 != 0) {
                PlaySfx(1);
                UiRows[MenuCursor].cells[0] = 1;
                v = 3;
                if (MenuCursor < 2) v = 7;
                UiRows->byte14 = v;
                UiRows[1].byte14 = v;
                UiRows[2].byte14 = v;
                D_800AC5D0 = 0x40;
                D_800F4D8A = 1;
            } else if (PlayerControllers[0].action2C != 0) {
                PlaySfx(2);
                v = 3;
                MenuCursor = s;
                UiRows->byte14 = 3;
                UiRows[1].byte14 = v;
                UiRows[2].byte14 = v;
                D_800AC5D0 = 0x40;
                D_800F4D8A = 1;
            }
        }
    } else if (s == 1) {
        SetMenuFrameOptions(0, 1, 1);
        t = D_800AC5D0 - 1;
        D_800AC5D0 = t;
        AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (0x40 - (short)t) * 4, 0x45);
        if ((short)D_800AC5D0 <= 0) {
            switch (MenuCursor) {
            case 0:
                UpdateGrandPrixStageProgress();
                ModeGroup = 0;
                D_800F3A26 = s;
                D_800F4D8A = 0;
                break;
            case 1:
                CardMenuOperation = 4;
                ModeGroup = s;
                D_800F3A26 = 0xD;
                D_800F4D8A = 0;
                break;
            case 2:
                ModeGroup = s;
                D_800F3A26 = 3;
                D_800F4D8A = 0;
                break;
            }
        }
    }
}

void CarStandingsModeEnter(void)
{
    int *sp = (int *)SCRATCHPAD;
    MenuGrid38 *p;
    AllocMenuGridRecords(D_800FFC48, 1);
    InitMenuGrid(0, 0, 3, 0xF0, 0xA0);
    PsyqBzero((SCRATCHPAD + 0x8), 0x20);
    p = MenuGridRecords;
    p->rot_z1C = 0x60;
    p->rot_y20 = -0x110;
    p->rot_x24 = -0x160;
    p->z14 = 0x1A;
    sp[6] = 0x400;
    UpdateGrandPrixStageProgress();
    ModeFrame = 0;
    ScreenFrame = 0;
    VSyncThreshold = 0x80;
    ResetScreenCounters();
    if (D_800AD4F2 == 6) {
        InitSelectionRows();
        ClearScreenOverlayState();
        InitMenuArenas();
        LoadCursor = MenuArenaBase;
    }
    MenuFrameState = 0;
    sp[0] = AddTileGrid2x2(&CurrentDrawBuf->buffer.ordering70.words[0], sp[0], 0, 0, 0x140, 0x200, 8, 8, 8);
    ModeGroup = 1;
    D_800F3A26 = 0x10;
    D_800F4D8A = 0;
}
