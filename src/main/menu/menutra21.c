#include "game/ui/uipart1.h"
#include "game/sound/sndvab.h"
#include "game/data/savetype.h"
#include "rr/save_slots.h"
#include "game/system/pad.h"
/* RODATA 0x8001160c 0x74 */
/* Candidate module: menu transitions, 8007BD18..8007BFF4.
 * Contiguous functions indexed by the same menu state and entry table. */
#include "rr/tables.h"
#include "rr/frame_storage_types.h"
#include "game/data/cardchk.h"
#include "game/data/cardini.h"
#include "game/menu/loadmenu.h"
#include "game/menu/menumode.h"
#include "game/menu/menutra21.h"
#include "game/menu/menutra22.h"
#include "rr/course_choices.h"
#include "rr/menu_selection.h"
#include "game/data/menuvars.h"
#include "rr/ui.h"
#include "rr/badge_row_types.h"
#include "rr/menu_text_state.h"
#include "rr/screen_tiles.h"
#include "rr/string_tail_copy.h"
#include "game/sound/cdmain.h"
#include "game/data/ldscn1.h"
#include "game/sound/fxplay.h"

extern BadgeRow1C *BadgeRows;


typedef struct { s16 a, b; } P2;



extern MenuChoice TeamSelectBannerUvs[];
extern s32 MenuCursorBlinkColors[];
extern s16 MenuPanelBorderColors[];
extern MenuChoice MenuDividerLineColors[];
extern s16 MenuActiveControllerIndex;
extern s32 ModeFrame;
extern s16 MenuStateIndex;
extern u8 CardPortPollStatus[];
extern s16 CardActivePort;
extern s16 CardReplayLoadedFlag;
extern unsigned char MenuHighlightBarColors[][4];
extern unsigned short CardTransferMode;
extern unsigned short SubmodeState;
extern short CardMenuOperation;
extern short CardPromptChoices[];
extern short CardPromptIndex;
extern int ovl660_init_menu();
extern int ovl661_init_preview();
extern int ovl667_init_car_selection();
extern int ovl662_init_team_car_layout();
extern short FadeCountdown;
extern short ModeIndex;
extern short LinkState;
extern unsigned short CardMenuReturnModeId;
extern unsigned short GameModeType;
extern int DialogueSceneMode;
extern short GpSaveSlotCursor;
extern int GarageExtraDefaultTimes[];
extern s16 MenuSlideStepVectors[][2];
#include "rr/save_buffer.h"
extern int MenuSaveBannerTimer;
extern short MenuSubpageCursor;
extern short MenuSubpageScroll;
extern unsigned char CardFileCounts;
extern char CardSaveFileName[];
extern char CardTargetFileName[];
extern int MenuConfirmDialogState;
extern int MenuTransitionTimer;
extern unsigned char GaragePreviewCamPresets[];

void RunCardSaveOrLoadStep(void);
void StepCardConfirmPrompt(void);
void WaitCardDialogueComplete(void);
void RunCardFormatStep(void);
void InitCardMenuSession();
void CloseCardMenuUiRows(void);
void StepCardSlotHeaderScan();
void CheckCardPollStatusTransition(void);
void CheckCardFileExistsTransition(void);
void StepCardDataTransfer(void);
int ScanBothCardPortsStep();
int ReadCardSlotHeaderStep();

void RunCardSaveOrLoadStep(void) {
    s32 i = MenuStateTable[MenuStateIndex].tableIdx04;
    s32 a = TeamSelectBannerUvs[i].a;
    s32 c = TeamSelectBannerUvs[i].c;
    s32 b = TeamSelectBannerUvs[i].b;
    s32 r;
    if (c == 1) r = CardSaveStep(0, CardActivePort);
    else r = CardLoadStep(0, CardActivePort);
    if (r == 4) {
        MenuStateIndex = a;
    } else if (r == 5) {
        MenuStateIndex = b;
    } else if (r == 6) {
        MenuStateIndex = b;
    }
    if (c == 0xD) {
        if (r == 4) CardReplayLoadedFlag = 1;
    }
}

void StepCardConfirmPrompt(void)
{
  s32 a1 = *(MenuCursorBlinkColors + MenuStateTable[MenuStateIndex].tableIdx04);
  if ((&PlayerControllers[MenuActiveControllerIndex])->action28 != 0)
  {
    if (a1 == 0)
    {
      PlaySfx(1);
      if (MenuStateIndex == 0xC)
      {
        MenuStateIndex = SavedMenuStateIndex;
        CardPortPollStatus[CardActivePort] = 0x5A;
        PollCardStatus(CardActivePort, 0, 1);
        return;
      }
      MenuStateIndex = 0;
    }
  }
  else
    if ((&PlayerControllers[MenuActiveControllerIndex])->action2C != 0)
  {
    if (a1 == 0)
    {
      PlaySfx(2);
      MenuStateIndex = 0;
    }
  }
}

void WaitCardDialogueComplete(void) {
    s32 i = MenuStateTable[MenuStateIndex].tableIdx04;
    s32 v = MenuPanelBorderColors[i * 2];
    if (CardDialogueStatePtr->st == 4) {
        MenuStateIndex = v;
        ModeFrame -= 1;
    }
}

void RunCardFormatStep(void) {
    s32 k;
    s32 i;
    s32 a, b;
    for (k = 0x63; k >= 0; k--) ;
    i = MenuStateTable[MenuStateIndex].tableIdx04;
    a = MenuDividerLineColors[i].a;
    b = MenuDividerLineColors[i].c;
    if (CardFormatSlot(CardActivePort, 0) == 1) {
        MenuStateIndex = a;
        return;
    }
    MenuStateIndex = b;
}

void InitCardMenuSession(void) {
    int arr[4];
    int i, c, flag, n;
    MenuStateEntry8 *base, *p;
    unsigned char *q;
    ControllerState5C *r, *b3;
    signed char sb;

    CardPdaModeFlag = 0;
    CardTransferMode = *(unsigned short *)&CardMenuOperation;
    switch ((short)CardTransferMode) {
    case 0: MenuStateIndex = 0; break;
    case 1: case 5: MenuStateIndex = 0x20; break;
    case 2: MenuStateIndex = 0x21; break;
    case 3: MenuStateIndex = 0x18; break;
    case 6: MenuStateIndex = 0x2A; CardTransferMode = 0x10; break;
    case 7: MenuStateIndex = 0x2B; CardTransferMode = 0x10; break;
    }
    base = MenuStateTable;
    p = base + MenuStateIndex;
    i = p->tableIdx04;
    c = p->promptIdx06;
    CardPromptIndex = c;
    if (SubmodeState == 0) {
        sb = p->defaultChoice07;
        if (sb >= 0) {
            CardPromptChoices[c] = sb;
        }
        SubmodeState = 1;
    }
    flag = 0;
    q = MenuHighlightBarColors[i];
    i = 0;
    arr[0] = q[0];
    arr[1] = q[1];
    arr[2] = q[2];
    arr[3] = q[3];
    n = 4;
    for (; i < 4; i++) {
        if (arr[i] == 0xFF) { n = i; break; }
        if (i == 1 && (arr[1] & 0x80)) {
            flag = 1;
            arr[1] ^= 0x80;
        }
    }
    if (flag) {
        StepMenuValue(((unsigned short)MenuActiveControllerIndex & 1) << 2, 2, CardPromptChoices + CardPromptIndex, n, 0);
    } else {
        StepMenuValue((((unsigned short)MenuActiveControllerIndex & 1) << 2) | 1, 2, CardPromptChoices + CardPromptIndex, n, 0);
    }
    b3 = PlayerControllers;
    r = b3 + MenuActiveControllerIndex;
    if (r->action28 != 0) {
        PlaySfx(1);
        SubmodeState = 0;
        MenuStateIndex = arr[CardPromptChoices[CardPromptIndex]];
        if (MenuStateIndex != 0x43) {
            CardPortPollStatus[CardActivePort] = 0x5A;
            PollCardStatus(CardActivePort, 0, 1);
        }
    } else if (r->action2C != 0) {
        PlaySfx(2);
        SubmodeState = 0;
        MenuStateIndex = arr[n - 1];
    }
}


void CloseCardMenuUiRows(void) {
    int i;
    char *q;
    if (UiRows[0x23].byte14 == 2) {
        for (i = 0; i < 4; i++) {
            q = (char *)(i * 0x24 + (int)UiRows) + 0x4C8;
            if (q[0x14] == 2) {
                q[0x14] = 3;
            }
        }
        BadgeRows->state10 = 4;
        if (CardMenuOperation == 3 || CardMenuOperation == 6 || CardMenuOperation == 7) {
            BadgeRows[1].state10 = 4;
        }
        FadeCountdown = 0x40;
        if (CardMenuOperation == 0) {
            SeqFadeVolume(0, 0x3C);
        }
        ResetScreenCounters();
        DialogueSceneMode = 2;
        return;
    }
    if (FadeCountdown > 0) {
        FadeCountdown = FadeCountdown - 1;
    }
    AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (0x40 - FadeCountdown) << 2, 0x45);
    if (LoadMenuCarModelsStep() != 1) return;
    if (FadeCountdown > 0) return;
    if (UiRows[0x23].byte14 != 5) return;
    if (BadgeRows->state10 != 0xB) return;
    for (i = 0; i < 3; i++) {
        SaveSlotSelections[i] = SaveSlotSnapshots[i];
    }
    ClearScreenOverlayState();
    switch (CardMenuOperation) {
    case 0:
    default:
    case 4:
        SeqMuteVolume();
        ModeGroup = 1;
        ModeIndex = 3;
        SubmodeState = 0;
        break;
    case 1:
        if (CardMenuReturnModeId == 0x132) {
            ovl660_init_menu();
            SubmodeState = 0;
            ModeGroup = CardMenuReturnModeId >> 8;
            ModeIndex = CardMenuReturnModeId & 0xFF;
        }
        break;
    case 2:
        if ((GameModeType - 2) < 2U) {
            ovl661_init_preview(ActivePlayerIndex);
            ModeGroup = 1;
            ModeIndex = 0x38;
            SubmodeState = 0;
        } else if ((short)GameModeType == 4) {
            if (LinkState != -1) {
                ovl667_init_car_selection(ActivePlayerIndex);
                ModeGroup = 1;
                ModeIndex = 0x3C;
            } else {
                SeqMuteVolume();
                ModeGroup = 1;
                ModeIndex = 0x2D;
            }
            SubmodeState = 0;
        }
        break;
    case 3:
        ModeGroup = 1;
        ModeIndex = 0xF;
        SubmodeState = 0;
        break;
    case 5:
        ovl662_init_team_car_layout();
        ModeGroup = 1;
        ModeIndex = 0x40;
        SubmodeState = 0;
        break;
    case 6:
        if (CardReplayLoadedFlag != 0) {
            ModeGroup = 3;
            ModeIndex = 3;
        } else {
            ModeGroup = 3;
            ModeIndex = 2;
        }
        SubmodeState = 0;
        break;
    case 7:
        ModeGroup = 1;
        ModeIndex = 0x27;
        SubmodeState = 0;
        break;
    }
}

void StepCardSlotHeaderScan(void) {
    short t;
    if (SubmodeState == 0) {
        GarageExtraDefaultTimes[0] = -1;
        GarageExtraDefaultTimes[1] = -1;
        GarageExtraDefaultTimes[2] = -1;
        GpSaveSlotCursor = 0;
        ReadCardSlotHeaderStep(0, 0);
        SubmodeState = 1;
        return;
    }
    if (ReadCardSlotHeaderStep(1, GpSaveSlotCursor) != 0) {
        t = (unsigned short)GpSaveSlotCursor + 1;
        GpSaveSlotCursor = t;
        if (t == 3) {
            if ((short)CardMenuOperation == 7) {
                MenuStateIndex = 0x2B;
            } else {
                MenuStateIndex = 0x2A;
            }
            GpSaveSlotCursor = 0;
            SubmodeState = 0;
            return;
        }
        ReadCardSlotHeaderStep(0, t);
    }
}

void CheckCardPollStatusTransition(void) {
    int i = MenuStateTable[MenuStateIndex].tableIdx04;
    unsigned char *q = CardPollTransitionTable + i * 4;
    int x = q[0], y = q[1];
    if (CardPortPollStatus[CardActivePort] < 0x10 || x == CardPortPollStatus[CardActivePort]) {
        MenuStateIndex = y;
        return;
    }
    if (CardPortPollStatus[CardActivePort] == 0x14 && MenuStateIndex == 2) {
        MenuStateIndex = 8;
    }
}


void CheckCardFileExistsTransition(void) {
    s32 i = MenuStateTable[MenuStateIndex].tableIdx04;
    s32 a = MenuSlideStepVectors[i][0];
    s32 b = MenuSlideStepVectors[i][1];
    if (MenuStateIndex == 0x3B) {
        if (CardPortUnformatted[CardActivePort] == 1) {
            MenuStateIndex = a;
            return;
        }
        MenuStateIndex = b;
        return;
    }
    MenuStateIndex = a;
}

void StepCardDataTransfer(void)
{
  int r;
  int t0;
  int t1;
  SavedMenuSettings *t2;
  short a1;
  unsigned v1;
  char *a0;
  int new_var;
  char *a2;
  SavedMenuSettings *a3;
  signed char *e;
  char sp[0x38];
  if (SubmodeState == 0)
  {
    ScanBothCardPortsStep(0);
    MenuSaveBannerTimer = 0;
    SubmodeState = 1;
    return;
  }
  r = ScanBothCardPortsStep(1);
  t1 = 0;
  if (r > 0)
  {
    new_var = 8;
    do
    {
      a1 = 0;
      v1 = 0;
      a0 = ((char *) (t1 * 0x2C)) + ((int) SaveBuffer);
      do
      {
        if (v1 != new_var)
        {
          a1 += ((unsigned short *) a0)[0x1F60 / 2];
        }
        v1++;
        a0 = a0 + 2;
      }
      while (v1 < 0x16U);
      a2 = ((char *) (t1 * 0x2C)) + ((int) SaveBuffer);
      a3 = (SavedMenuSettings *) (a2 + 0x1F60);
      if (a1 == a3->signedHalves[RR_SAVE_SLOT_CHECKSUM_HALFWORD])
      {
        *(t2 = SaveSlotSelections + t1) = *((SavedMenuSettings *) a3);
      }
      else
      {
        e = ((signed char *) SaveSlotSelections) + (t1 * 0x2C);
        e[0x17] = -1;
      }
      t1++;
    }
    while (t1 < 3);
    MenuStateIndex = 0x18;
    SubmodeState = 0;
  }
  else
    if (r < 0)
  {
    for (t1 = 2; t1 >= 0; t1--)
    {
      e = ((signed char *) SaveSlotSelections) + (t1 * 0x2C);
      e[0x17] = -1;
    }

    MenuStateIndex = 0x18;
    SubmodeState = 0;
  }
}


int ScanBothCardPortsStep(int a0) {
    int sp[14];
    int ret;
    int i;
    int r;
    int q;
    int v;
    if (a0 == 0) {
        MenuSubpageCursor = 0;
    }
    ret = 0;
    switch (MenuSubpageCursor) {
    case 0:
        CardPortPollStatus[1] = 0x5A;
        CardPortPollStatus[0] = 0x5A;
        PollCardStatus(CardActivePort, 0, 1);
        CopyStringClearTail(CardTargetFileName, CardSaveFileName);
        MenuSubpageCursor = 1;
        break;
    case 1:
        PollCardStatus(CardActivePort, 0, 0);
        if (CardPortPollStatus[0] < 0x10U) {
            if (CardHasFile(CardFileCounts, CardTargetFileName) == 1) {
                MenuSubpageScroll = 0;
                MenuSubpageCursor = 2;
            } else {
                MenuSubpageCursor = 7;
            }
            break;
        }
        v = CardPortPollStatus[0];
        switch (v) {
        case 0x14:
        case 0x1E:
        case 0x28:
        case 0x3C:
            MenuSubpageCursor = 7;
            break;
        case 0x32:
        case 0x5A:
            ret = 0;
            break;
        }
        break;
    case 2:
        MenuSubpageCursor = 3;
        break;
    case 3:
        for (i = 0x63; i >= 0; i--) {
        }
        r = CardReadFileAt(CardActivePort, 0, CardTargetFileName, SaveBuffer + RR_SAVE_SLOT_SCAN_BUFFER_OFFSET, sizeof(CardSlotScanWindow100), RR_SAVE_SLOT_SCAN_FILE_OFFSET);
        if (r == -1) {
            if (MenuSubpageScroll < 3) {
                MenuSubpageScroll = (unsigned short)MenuSubpageScroll + 1;
            } else {
                MenuSubpageCursor = 7;
            }
        } else {
            MenuSubpageCursor = 4;
        }
        break;
    case 4:
        for (i = 0x63; i >= 0; i--) {
        }
        q = GetHwCardEvent();
        if (q != 0) {
            if (q == 1) {
                MenuSubpageCursor = 5;
            } else if (MenuSubpageScroll >= 3) {
                MenuSubpageCursor = 7;
            } else {
                MenuSubpageCursor = 2;
                MenuSubpageScroll = (unsigned short)MenuSubpageScroll + 1;
            }
        }
        break;
    case 5:
        ret = 1;
        break;
    case 6:
    case 7:
    default:
        ret = -1;
        break;
    }
    return ret;
}


int LoadSaveFromCardStep(int a0) {
    int ret;
    int i;
    int v;
    char *p;
    if (a0 == 0) {
        MenuConfirmDialogState = 0;
    }
    ret = 0;
    switch (MenuConfirmDialogState) {
    case 0:
        CardPortPollStatus[1] = 0x5A;
        CardPortPollStatus[0] = 0x5A;
        PollCardStatus(0, 0, 1);
        i = 0x17;
        p = CardTargetFileName + i;
        do {
            *p = 0;
            i--;
            p--;
        } while (i >= 0);
        CopyStringClearTail(CardTargetFileName, CardSaveFileName);
        CardTransferMode = 0x30;
        MenuConfirmDialogState = 1;
        break;
    case 1:
        PollCardStatus(0, 0, 0);
        if (CardPortPollStatus[0] < 0x10U) {
            if (CardHasFile(CardFileCounts, CardTargetFileName) == 1) {
                CardLoadStep(2, 0);
                MenuConfirmDialogState = 2;
            } else {
                goto set1;
            }
            break;
        }
        v = CardPortPollStatus[0];
        switch (v) {
        case 0x14:
        case 0x1E:
        case 0x28:
        case 0x3C:
            MenuConfirmDialogState = 3;
            goto set1;
        case 0x32:
        case 0x5A:
            ret = 0;
            break;
        }
        break;
    case 2:
        ret = CardLoadStep(0, 0);
        if (ret == 4) {
            goto set1;
        } else if (ret == 5) {
            goto set1;
        } else if (ret == 6) {
            goto set1;
        } else {
            ret = 0;
        }
        break;
    case 3:
set1:
        ret = 1;
        break;
    default:
        return ret;
    }
    return ret;
}


int ReadCardSlotHeaderStep(int a0, int a1) {
    int ret;
    int v;
    if (a0 == 0) {
        MenuTransitionTimer = 0;
    }
    ret = 0;
    switch (MenuTransitionTimer) {
    default:
    case 0:
        CardPortPollStatus[1] = 0x5A;
        CardPortPollStatus[0] = 0x5A;
        PollCardStatus(0, 0, 1);
        MenuTransitionTimer = 1;
        CopyStringClearTail(CardTargetFileName, CardTargetFileNames[a1]);
        CardTransferMode = 0x10;
        break;
    case 1:
        PollCardStatus(0, 0, 0);
        if (CardPortPollStatus[0] < 0x10U) {
            if (CardHasFile(CardFileCounts, CardTargetFileName) == 1) {
                CardLoadStep(2, 0);
                MenuTransitionTimer = 2;
            } else {
                ret = -1;
            }
            break;
        }
        v = CardPortPollStatus[0];
        switch (v) {
        case 0x14:
        case 0x1E:
        case 0x28:
        case 0x3C:
            MenuTransitionTimer = 3;
            ret = -2;
            break;
        case 0x32:
        case 0x5A:
            ret = 0;
            break;
        }
        break;
    case 2:
        ret = CardLoadStep(0, 0);
        if (ret == 4) {
            MenuTransitionTimer = ret;
            ret = 2;
        } else if (ret == 5) {
            ret = -1;
        } else if (ret == 6) {
            ret = -1;
        } else {
            ret = 0;
        }
        break;
    case 3:
        ret = -1;
        break;
    case 4:
        ret = 3;
        break;
    }
    return ret;
}


void DrawCardMenuStatusText(int a0, int a1, int a2, int a3, int a4, int a5) {
    unsigned char *v1;
    int s0 = a1;
    if (a2 != 0) {
        int c14 = 0x14, c16 = 0x16, c4 = 4, c1 = 1;
        v1 = GaragePreviewCamPresets;
        a3 = v1 + c14;
        do {
            if (a5 == v1[1]) {
                if (a5 != c14) {
                    if (a0 != c16 || s0 != c4 || a2 != c1) s0 = 0;
                    a0 = v1[0];
                }
                break;
            }
            v1 += 4;
        } while ((int)v1 < a3);
        if (a0 == 5) {
            if (CardPdaModeFlag == 1) {
                a0 = 0x1A;
            } else if (CardMenuOperation == 2 && CardActivePort == 1) {
                a0 = 0x21;
            }
        }
    }
    if (((short *)BadgeRows)[8] == 3) {
        if (a0 == -1) {
            if (CardMenuOperation == 3 || CardMenuOperation == 6 || CardMenuOperation == 7) a0 = 0x19;
        } else if (a0 == -128) {
            a0 = CardDialogueStatePtr->b4;
        } else {
            if (a0 == 0x12 && CardPdaModeFlag == 1 && CardPortUnformatted[CardActivePort] == 1) {
                a0 = 0x1C;
            } else if (a0 == 0x15 && CardActivePort == 1) {
                a0 = 0x22;
            } else if (a0 == 0x25 && CardActivePort == 1) {
                a0 = 0x26;
            } else if (a0 == 0x1F && CardMainSaveExists == 0) {
                a0 = 0x20;
            }
        }
        if (a0 >= 0) {
            CardDialogueStatePtr->b4 = a0;
            UpdateDialogue(((short *)BadgeRows)[4] + 10, ((short *)BadgeRows)[5] + 0x28, CardDialogueStatePtr);
        }
    }
    UpdateCardSlotUiRows(a4, ((s0 & 2) > 0) & (CardDialogueStatePtr->s12 >= 6));
    DrawCardMenuCommandRows(a4, (s0 & 4) > 0);
    if (s0 & 8) DrawSaveSlotTableHeader(GpSaveSlotCursor);
}
