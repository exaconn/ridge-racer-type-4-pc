#include "types.h"
#include "scratchpad.h"
#include "m2c_macros.h"
#include "game/menu/menu8.h"
#include "game/menu/menutran.h"
#include "game/menu/mnsel.h"
#include "rr/driver_control.h"
#include "rr/frame_environment.h"
#include "rr/menu_background.h"
#include "rr/selection.h"
#include "rr/bios_devices.h"
#include "game/ui/barsel0.h"
#include "game/gfx/drawbuf.h"
#include "game/data/screenframe.h"

typedef struct { char p[0x17]; signed char b; char q[0x2C - 0x18]; } T;

typedef struct { char p0[0xC]; unsigned char a; char p1[7]; unsigned char b; char p2[0xF]; } E;

extern int ClearScreenOverlayState();
extern void InitUiRow(int,int,int,int,int,int,int,int,int,int,...);
extern int SetMenuFrameOptions();
extern short D_800AC5D0;
extern int VSyncThreshold;
extern short MenuCursor;
extern signed char SelectionState[];
extern int ModeFrame;
extern T D_800F2E30[];
extern short D_800F3A26;
extern short ModeGroup;
extern short D_800F4D8A;
extern int DrawUiRow();
extern E *UiRows;
extern short D_800FFC18;
extern struct { int p[10]; int a28; int a2c; } D_800F3A38;
extern int StepMenuValue();
extern int PlaySfx();
extern int AddScreenFadeTiles();
extern int AddTileGrid2x2();
extern int ResetScreenCounters();
extern int InitMenuArenas();
extern int InitMenuGrid();
extern int LoadCursor;
extern int *MenuGridRecords;
extern short MenuFrameState;
extern int D_800F4D40;
extern char D_800FFC48[];

void func_8002FCEC();
void func_8002FE98(void);
void func_8002FECC(void);
void func_8002FEFC();
void func_8003016C(void);

void func_8002FCEC() {
    int a0, a1;
    short v;
    T *p;
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
    p = D_800F2E30;
    L:
    a0++;
    if (p->b > 0) goto T;
    p++;
    if (a0 < 3) goto L;
    J:
    if (a1 != 0) goto B;
    func_8002F688();
    ModeGroup = 0;
    D_800F3A26 = 1;
    D_800F4D8A = 0;
    goto E2;
    T:
    a1 = 1;
    goto J;
    B:
    InitUiRow(0, 0x6C, 0xB8, 0x44, 0x20, 0, 0, 1, 0, 0x54);
    InitUiRow(1, 0x6C, 0xE0, 0x44, 0x20, 0, 0, 1, 0, 0x67);
    InitUiRow(2, 0x6C, 0x108, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    SetMenuFrameOptions(0, 1, 1);
    ModeGroup = 1;
    D_800F3A26 = 6;
    D_800F4D8A = 0;
    E2:
}

void func_8002FE98(void) {
    func_8002FEFC();
    func_8002FECC();
    UpdateScreenFade(0x10086);
}

void func_8002FECC(void) {
    DrawUiRow(0);
    DrawUiRow(1);
    DrawUiRow(2);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)
#define D_800AC5D0 (*(unsigned short (*))&D_800AC5D0)
#define D_800F3A26 (*(unsigned short (*))&D_800F3A26)
#define ModeGroup (*(unsigned short (*))&ModeGroup)

void func_8002FEFC() {
    int s;
    int v, t, w;
    s = D_800F4D8A;
    if (s != 0) goto C1;
    UiRows[MenuCursor].a = 0;
    StepMenuValue(0, 2, &MenuCursor, 3, 0);
    UiRows[MenuCursor].a = 2;
    s = UiRows->b;
    if (s != 2) goto END;
    SetMenuFrameOptions(1, 1, 1);
    if (D_800F3A38.a28 == 0) goto L0C;
    PlaySfx(1);
    UiRows[MenuCursor].a = 1;
    v = 3;
    if (MenuCursor < 2) v = 7;
    UiRows->b = v;
    UiRows[1].b = v;
    UiRows[2].b = v;
    D_800AC5D0 = 0x40;
    D_800F4D8A = 1;
    goto END;
L0C:
    if (D_800F3A38.a2c == 0) goto END;
    PlaySfx(2);
    v = 3;
    MenuCursor = s;
    UiRows->b = 3;
    UiRows[1].b = v;
    UiRows[2].b = v;
    D_800AC5D0 = 0x40;
    D_800F4D8A = 1;
    goto END;
C1:
    if (s != 1) goto END;
    SetMenuFrameOptions(0, 1, 1);
    t = D_800AC5D0 - 1;
    D_800AC5D0 = t;
    AddScreenFadeTiles(D_800ACC14 + 0x70, (0x40 - (short)t) * 4, 0x45);
    if ((short)D_800AC5D0 > 0) goto END;
    w = MenuCursor;
    if (w == s) goto L120;
    if (w < 2) {
        if (w == 0) goto L104;
        goto END;
    }
    if (w == 2) goto L13C;
    goto END;
L104:
    func_8002F688();
    ModeGroup = 0;
    D_800F3A26 = s;
    D_800F4D8A = 0;
    goto END;
L120:
    D_800FFC18 = 4;
    ModeGroup = s;
    D_800F3A26 = 0xD;
    D_800F4D8A = 0;
    goto END;
L13C:
    ModeGroup = s;
    D_800F3A26 = 3;
    D_800F4D8A = 0;
END:
    return;
}

#undef D_800F4D8A
#undef D_800AC5D0
#undef D_800F3A26
#undef ModeGroup

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 ((char *)D_800ACC14)

void func_8003016C(void)
{
    int *sp = (int *)SCRATCHPAD;
    int *p;
    AllocMenuGridRecords(D_800FFC48, 1);
    InitMenuGrid(0, 0, 3, 0xF0, 0xA0);
    func_80096434((SCRATCHPAD + 0x8), 0x20);
    p = MenuGridRecords;
    p[7] = 0x60;
    p[8] = -0x110;
    p[9] = -0x160;
    p[5] = 0x1A;
    sp[6] = 0x400;
    func_8002F688();
    ModeFrame = 0;
    ScreenFrame = 0;
    VSyncThreshold = 0x80;
    ResetScreenCounters();
    if (D_800AD4F2 == 6) {
        InitSelectionRows();
        ClearScreenOverlayState();
        InitMenuArenas();
        LoadCursor = D_800F4D40;
    }
    MenuFrameState = 0;
    sp[0] = AddTileGrid2x2(D_800ACC14 + 0x70, sp[0], 0, 0, 0x140, 0x200, 8, 8, 8);
    ModeGroup = 1;
    D_800F3A26 = 0x10;
    D_800F4D8A = 0;
}

#undef D_800ACC14
