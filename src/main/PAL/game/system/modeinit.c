#include "game/system/modeslot.h"
/* RODATA 0x800104c4 0x2c */
#include "rr/ui.h"
#include "game/data/ldscn1.h"
#include "game/gfx/fadectl.h"
#include "game/gfx/fadein.h"
#include "game/gfx/fadesel.h"
#include "game/menu/menutran.h"
#include "game/race/racegrid.h"
#include "game/race/ranking.h"
#include "game/system/modepad.h"
#include "game/menu/menusave.h"
#include "game/system/modeinit.h"
#include "rr/audio_cd_controller.h"
#include "rr/driver_control.h"
#include "rr/frame_environment.h"
#include "game/data/ldscn1.h"
#include "game/gfx/gfxotag.h"
#include "game/system/modeinit.h"
#include "game/data/globals.h"
#include "game/data/selection.h"
#include "game/data/screenframe.h"

extern int func_80053CB0();
extern int CdDirectoryBuffer;
extern char D_800104A4[];
extern short D_800AC5D0;
extern int D_800AC5D4;
extern short D_800ACBD0[];
extern int D_800ACBF8;
extern int LoadCursor;
extern short D_800AD508;
extern short ReverseCourse;
extern int D_800F2EC8;
extern unsigned short D_800F3974;
extern short D_800F3A26;
extern short MenuScriptId;
extern short ModeGroup;
extern int D_800F4CA8;
extern int D_800F4D40;
extern unsigned short D_800F4D44;
extern short D_800F4D8A;
extern short LinkState;
extern int D_8010BFB8;
extern int InitBadgeRow();
extern char D_800104B0[];
extern char LinkEntryState[];
extern char *D_800AC590;
extern int VSyncThreshold;
extern short MenuCursor;
extern short D_800AD50A;
extern int ModeFrame;
extern short *BadgeRows;
extern int D_800F3A60;
extern void UpdateDialogue();
extern void ResetScreenCounters();
extern void UpdateBadgeRowAnim();
extern void PlaySfx();
extern void StartAttractDemo();

void TitleModeEnter();
void TitleMenuInit();
void TitleMenuUpdate(void);

void TitleModeEnter() {
    short *a0;
    char *a1;
    int a2;
    SetDispMask(0);
    func_80053CB0();
    a2 = 0;
    a1 = SelectionRows;
    a0 = D_800ACBD0;
    D_800AD508 = -1;
    LinkState = 0;
    D_800AC5D0 = 0;
    D_800AC5D4 = 0;
    D_8010BFB8 = 0;
    ReverseCourse = 0;
    do {
        a0[0] = 0;
        a0[1] = 0;
        a1[3] = 0;
        a1 += 4;
        a2++;
        a0 += 2;
    } while (a2 < 4);
    D_800F4CA8 = 2;
    MenuScriptId = 4;
    ModeGroup = 1;
    D_800F3A26 = 0x23;
    D_800F4D8A = 0;
    LoadCursor = CdDirectoryBuffer;
    D_800F4D40 = CdDirectoryBuffer;
    if ((unsigned)(D_800F4D44 - 2) < 3U) func_8001F254(CdDirectoryBuffer, a1, a2);
    D_800F4D44 = 0;
    _sio_control(1, 1, 0);
    D_800ACBF8 = 0;
    D_800F2EC8 = 0;
    D_800F4CD0 = (D_800F3974 & 1) + 7;
    func_8001E84C(D_800104A4);
}

void TitleMenuInit()
{
  int i;
  int y;
  int v;
  short c7;
  SetupDrawDispEnvs(0, 0, 0, 3);
  c7 = 7;
  i = 0;
  ModeFrame = 0;
  ScreenFrame = 0;
  VSyncThreshold = 0x80;
  D_800F4D2C = 0;
  MenuCursor = -1;
  LinkEntryState[3] = 0;
  LinkEntryState[2] = 0;
  LinkEntryState[1] = 0;
  LinkEntryState[0] = 0;
  D_800AD50A = 0x104;
  while (i < (D_800F4CD0 + 1))
  {
    v = i;
    if (D_800F4CD0 == c7)
    {
      if (i == D_800F4CD0)
      {
        v = 8;
      }
    }
    InitUiRow(i, 0x6C, (i * 0x22) + 0x40, 0x54, 0x20, 0, 0, 1, 0x7F, v);
    i++;
  }

  if (CountProgressFlags() == 0x140)
  {
    D_800F3970 = 1;
  }
  D_800AC590[7] = 2;
  InitBadgeRow(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
  BadgeRows[1] = 1;
  _sio_control(1, 1, 0);
  func_8001E84C(D_800104B0);
}

/* legacy typing of shared globals in this part of the unit */
#define ScreenFrame (*(unsigned (*))&ScreenFrame)
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)
#define BadgeRows ((W7C0_F *)BadgeRows)
#define D_800AC590 ((unsigned char *)D_800AC590)
#define UiRows ((unsigned char *)UiRows)
#define InitBadgeRow ((void (*)())InitBadgeRow)
#define _sio_control ((void (*)())_sio_control)

void TitleMenuUpdate(void)
{
    int s0;
    int v1;
    int a1;
    int s0b;
    int c3, c4;
    W7C0_F *a0;

    ModeFrame = ModeFrame + 1;
    ScreenFrame = ScreenFrame + 1;
    s0 = func_8002F0E0();
    switch (D_800F4D8A) {
    case 0:
    case 10:
        _sio_control(1, 1, 0);
        if (D_800F4BE0 > 0) {
            UpdateBadgeRowAnim(0);
            if (BadgeRows->s10 == 3) {
                if (D_800F3A60 == 0 && D_800F4D8A != 0 && (signed char)D_800AC590[6] != 4)
                    goto call;
                if (D_800F4D8A != 0xA)
                    goto els;
                PlaySfx(3);
                D_800AC590[4] = D_800AC590[4] + 1;
                goto c8;
            set:
                D_800AC590[4] = v1;
                goto call;
            els:
                D_800F4D8A = 0xA;
            c8:
                v1 = (signed char)D_800AC590[4];
                if (v1 < 4) {
                    c3 = 3;
                    c4 = 4;
                    a1 = D_800F4BE0;
                    a0 = BadgeRows;
                    do {
                        if ((a1 >> v1) & 1)
                            goto set;
                        if (v1 == c3)
                            a0->s10 = c4;
                        v1++;
                    } while (v1 < 4);
                }
            call:
                UpdateDialogue(BadgeRows->s8 + 0xA, BadgeRows->sA + 0x28, D_800AC590);
                return;
            } else if (BadgeRows->s10 == 0xB) {
                D_800F4D8A = 1;
                D_800F4BE0 = 0;
                ScreenFrame = 0;
                return;
            }
        } else {
            D_800F4D8A = 1;
            ScreenFrame = 0;
            return;
        }
        return;
    case 1:
        _sio_control(1, 1, 0);
        if (D_800F3A40 & 0x800) {
            if (s0 == 0)
                func_8005C7A4();
            PlaySfx(1);
            func_8005CA04();
            MenuCursor = 0;
            func_80034E14();
            D_800F4D8A = 2;
            InitBadgeRow(0, 0x10, 0x1B6, 0x40, 0x22, 0, 0xF00);
            BadgeRows->s2 = 1;
        }
        if (ScreenFrame >= 0x2EE)
            D_800F4D8A = 5;
        return;
    case 2:
        func_80034E14();
        s0b = MenuCursor;
        if (UiRows[0x14] == 2) {
            if (ModeSelectMenuStep() != 0) {
                D_800F4D8A = 3;
                D_800AC5D0 = 0x40;
                ResetScreenCounters();
                func_8005C8D8(D_800F4D2C);
                BadgeRows->s10 = 4;
            }
        }
        if (s0b != MenuCursor)
            ScreenFrame = 0;
        if (ScreenFrame >= 0xBB8)
            D_800F4D8A = 5;
        DrawModeSelectRows();
        UpdateBadgeRowAnim(0);
        return;
    case 3:
        if (D_800AC5D0 <= 0) {
            if (func_8003A9E8() == 1) {
                D_800F4D8A = 0;
                ModeGroup = PendingModeId >> 8;
                D_800F3A26 = PendingModeId & 0xFF;
            }
        } else {
            D_800AC5D0 = D_800AC5D0 - 1;
        }
        DrawModeSelectRows();
        UpdateBadgeRowAnim(0);
        return;
    case 5:
        _sio_control(1, 1, 0);
        if (D_8010BFBC & 1) {
            ModeGroup = 2;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        } else {
            StartAttractDemo(0);
        }
        D_8010BFBC = D_8010BFBC + 1;
        if (D_8010BFBC >= 0x64)
            D_8010BFBC = 0;
        return;
    }
}

#undef ScreenFrame
#undef D_800F4D8A
#undef BadgeRows
#undef D_800AC590
#undef UiRows
#undef InitBadgeRow
#undef _sio_control
