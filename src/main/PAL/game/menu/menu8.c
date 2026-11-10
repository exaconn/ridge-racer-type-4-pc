/* RODATA 0x80010550 0x60 */
#include "types.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"
#include "scratchpad.h"
#include "rr/menu_state_types.h"
#include "rr/selection_types.h"
#include "game/menu/mnrows.h"
extern MenuSettings2C SelectionState;
extern SelectionRows32 SelectionRows;
extern u8 MenuSelectionLockedFlag;
#include "rr/ui.h"
#include "rr/badge_row_types.h"
#include "rr/menu_text_state.h"
#include "game/data/ldscn1.h"
#include "game/gfx/gfxotag.h"
#include "game/menu/menutran.h"
#include "game/menu/menu8.h"
#include "game/system/pad.h"
#include "game/ui/barsel0.h"
#include "game/data/globals.h"
#include "rr/menu_background.h"
#include "game/ui/badge1.h"
#include "rr/badge_rows.h"
#include "game/course/trkintp2.h"
#include "rr/screen_tiles.h"
#include "game/gfx/sprefct.h"
#include "game/ui/uidraw.h"
#include "game/ui/uibar.h"


typedef MenuSettings2C SaveSlotSelection;

extern unsigned char MenuSelectedMakerId;
extern short CardMenuOperation;
#include "rr/sprite_effect.h"
extern SpriteEffectSource MenuJitterSpritePresetA;
extern SpriteEffectSource MenuJitterSpritePresetB;
extern int SetMenuFrameOptions();
extern short NumCars;
extern void ClearScreenOverlayState(void);
extern s8 ScreenFadeState;
extern s16 ModeIndex;
extern s16 ModeGroup;
extern s16 GpSaveSlotCursor;
extern u16 SubmodeState;
extern int StepMenuValue();
extern void PlaySfx();
extern short FadeCountdown;
extern short MenuScriptId;
extern int MenuSharedScratchPtr;
extern SaveSlotSelection SaveSlotSelections[];
extern BadgeRow1C *BadgeRows;
extern unsigned short MenuSelectedCarSlot;
extern int MenuAutoAdvanceTimer;
extern s16 MenuCursor;
extern s32 ModeFrame;
extern s16 CardActivePort;

void SelectDialogueScript(void);
void GpSaveSlotMenuEnter(void);
void GpSaveSlotMenuUpdate(void);
void StepReplaySavePrompt(void);
void YesNoPromptModeEnter(void);

void SelectDialogueScript(void) {
    int sw;
    int idx, a1, a3, t;
    const MenuSettings2C *settings;
    MenuTextState *P;
    int u;
    int i;

    sw = MenuActiveDialogId;
    Dialogue->b7 = 1;
    Dialogue->s16 = 0;
    Dialogue->b1E = -2;
    switch (sw) {
    case 0:
        Dialogue->b7 = 0;
    case 1:
        Dialogue->b4 = 0;
        PendingModeId = 0x109;
        MenuActiveDialogId = 1;
        break;
    case 2:
        Dialogue->b4 = MenuSelectedMakerId + 0x18;
        PendingModeId = 0x10F;
        break;
    case 4:
        idx = SelectionState.phase17;
        if (idx < 4) {
            a3 = SelectionState.results00[idx];
        } else {
            a3 = 3 - SelectionState.retries08[idx];
            if (idx == 7) a3 = 0;
        }
        switch (idx) {
        case 0:
            a1 = SelectionState.retries08[idx] < 3;
            break;
        case 1:
        case 3:
            a1 = 4;
            if (SelectionState.retries08[idx] >= 3) a1 = SelectionState.results00[idx - 1];
            break;
        case 2:
            a1 = 4;
            if (SelectionState.retries08[idx] >= 3) a1 = SelectionState.reserved10[4];
            break;
        case 4:
            a1 = MenuSelectionLockedFlag;
            break;
        case 5:
            a1 = 3 - SelectionState.retries08[idx - 1];
            break;
        case 6:
        case 7:
            a1 = 0;
            i = 0;
            settings = &SelectionState;
            do {
                a1 += settings->retries08[i + 4];
                i++;
            } while (i < 4);
            if (a1 == 0xC) {
                a1 = 0;
            } else if (idx == 7) {
                a1 = 1;
            } else {
                a1 = 4 - SelectionState.retries08[idx - 1];
            }
            break;
        }
        P = Dialogue;
        u = a3 * 5;
        t = a1 + 4;
        P->b4 = u + t;
        switch (MenuSubmodePendingId) {
        case 1:
        case 3:
        case 6:
            MenuActiveDialogId = 6;
            UpdateSelectionRank();
            PendingModeId = 0x12A;
            break;
        default:
            PendingModeId = 0x10F;
            break;
        }
        if (MenuSubmodePendingId == 7) PendingModeId = 0x205;
        break;
    case 6:
        Dialogue->b4 = 0x25 - MenuSelectionLockedFlag;
        PendingModeId = 0x111;
        CardMenuOperation = 3;
        break;
    }
}

void UpdateDialoguePanel(void) {
    void *var_a1;
    s32 temp_v0;
    s32 temp_s2;

    temp_v0 = Dialogue->b1E;
    temp_s2 = Dialogue->b1F;
    switch (temp_v0) {
    case -2:
        Dialogue->b1F = 0;
        Dialogue->b1E = -4;
        Dialogue->b1D = -1;
        Dialogue->b1C = -1;
        break;
    case -4:
        InitRect16Row(0, 0x78, 0x60, 0x50, 0xB4, 0, 0);
        Dialogue->b1E = -5;
        break;
    case -5:
        if (UpdateRect16Row(0) != 0) {
            Dialogue->b1E = -1;
        }
        break;
    case -3:
        break;
    case -1:
        {
            void *a1 = *(void **)SCRATCHPAD;
            *(void **)SCRATCHPAD = AddDrawModePrim(
                &CurrentDrawBuf->buffer.ordering70.words[1],
                AddTileSemiTransPrim(&CurrentDrawBuf->buffer.ordering70.words[1], a1, 0x78, 0x60, 0x50, 0xB4, (u16)temp_s2, (u16)temp_s2, (u16)temp_s2),
                0x45);
            if ((u16)temp_s2 >= 0x7C) {
                Dialogue->b1E = 2;
                Dialogue->b1F = 0;
                BadgeRows[Dialogue->s16].half02 = 1;
            } else {
                Dialogue->b1F += 8;
            }
            UpdateRect16Row(0);
        }
        break;
    case 0:
        var_a1 = *(void **)SCRATCHPAD;
        if ((u16)temp_s2 >= 3) {
            var_a1 = AddJitterSprites(&CurrentDrawBuf->buffer.ordering70.words[0], var_a1, &MenuJitterSpritePresetA, 1, 0xFF, 2);
        }
        *(void **)SCRATCHPAD = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[1], var_a1, 0x78, 0x60, 0x50, 0xB4, 8, 8, 8);
        if ((u16)temp_s2 >= 0x15) {
            Dialogue->b1E = 1;
            Dialogue->b1F = 0;
        } else {
            Dialogue->b1F += 1;
        }
        if (temp_s2 == 1) {
            u8 temp_a0 = Dialogue->b1C;
            if ((temp_a0 - 1) < 5U) {
                LoadTimImage((int *)(Dialogue->images20 + (((s8)temp_a0 - 1) << 14)));
            }
        }
        break;
    case 1:
        {
            s32 two = 2;
            *(void **)SCRATCHPAD = AddTilePrim(
                &CurrentDrawBuf->buffer.ordering70.words[3],
                AddJitterSprites(
                    &CurrentDrawBuf->buffer.ordering70.words[0],
                    AddJitterSprites(&CurrentDrawBuf->buffer.ordering70.words[0], *(void **)SCRATCHPAD, &MenuJitterSpritePresetB, 1, temp_s2, 1),
                    &MenuJitterSpritePresetA, 1, (0x80 - temp_s2) * 2, two),
                0x78, 0x60, 0x50, 0xB4, 8, 8, 8);
            if (temp_s2 >= 0x80U) {
                Dialogue->b1F = 0;
                Dialogue->b1E = two;
            } else {
                Dialogue->b1F += 8;
            }
        }
        break;
    case 2:
    default:
        var_a1 = *(void **)SCRATCHPAD;
        if (Dialogue->b1C >= 0) {
            var_a1 = AddJitterSprites(&CurrentDrawBuf->buffer.ordering70.words[0], var_a1, &MenuJitterSpritePresetB, 0, 0x80, 1);
        }
        *(void **)SCRATCHPAD = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[1], var_a1, 0x78, 0x60, 0x50, 0xB4, 8, 8, 8);
        break;
    }
}

void UpdateDialogueBox(void) {
    BadgeRow1C *temp_v0;

    if (((BadgeRow1C *)((Dialogue->s16 * 0x1C) + (unsigned long)BadgeRows))->state10 == 3) {
        SetMenuFrameOptions(1, 1, 0);
        SkipDialoguePage(Dialogue);
        temp_v0 = (BadgeRow1C *)((Dialogue->s16 * 0x1C) + (unsigned long)BadgeRows);
        UpdateDialogue(temp_v0->x08 + 0xA, temp_v0->y0A + 0x28, (MenuTextState *)Dialogue);
    }
    UpdateBadgeRowAnim(Dialogue->s16);
}

void DialogueWaitEnterHook(void) {
}

void DialogueWaitExitHook(void) {
}

void ResetRankEntries(void) {
    int i = 0;
    RankEntry8 **q = RankTable;
    RankEntry8 *p = RankEntries;
    do {
        p->score00 = 0;
        p->carIndex04 = i;
        *q++ = p;
        i++;
        p++;
    } while (i < 8);
}

void ResetRankEntriesForCars(void) {
    int i = 0;
    RankEntry8 *e;
    RankEntry8 **t;
    int n;
    if (NumCars > 0) {
        n = NumCars;
        t = RankTable;
        e = RankEntries;
        do {
            e->score00 = 0;
            e->carIndex04 = i;
            *t = e;
            t++;
            i++;
            e++;
        } while (i < n);
    }
}

void InitSelectionRows(void) {
    s32 a2 = 0;
    s32 a1;
    SelectionRow *a0;
    SelectionRow *v1;
    SelectionRows32 *b;
    unsigned char *page;
    const u8 *choices, *variants;
    const MenuSettings2C *config;
    u8 t;
    s32 x;
    unsigned char *stage;
    u32 idx;
    u32 offset;
    u32 attribute;
    page=(unsigned char *)0x800F0000U;
    /* ASM_BARRIER: Materialize the state page after the initial row index. */
    __asm__("" : "+r"(page) : "r"(a2));
    b=(SelectionRows32 *)(page+0x4D98);
    /* ASM_BARRIER: Retain the full frame pointer separately from its page. */
    __asm__("" : "+r"(b));
    stage=(unsigned char *)0x800A0000U;
    /* ASM_BARRIER: Expose the selection-table page before adding its signed offset. */
    __asm__("" : "+r"(stage));
    choices=stage-0xBDC;
    stage=(unsigned char *)0x800A0000U;
    /* ASM_BARRIER: Keep the separate variant-table page materialization. */
    __asm__("" : "+r"(stage));
    variants=stage-0xBE0;
    stage=(unsigned char *)0x800B0000U;
    /* ASM_BARRIER: Expose the configuration page before adding its signed offset. */
    __asm__("" : "+r"(stage));
    config=(const MenuSettings2C *)(stage-0x2B28);
    /* ASM_BARRIER: Form lookup bases before initializing the byte offset. */
    __asm__("" : : "r"(choices), "r"(variants), "r"(config) : "$5");
    a1=RR_FIELD_OFFSET(SelectionRows32,rows);
    do {
        x = page[0x4D98];
        idx=x*8;
        offset=a2+idx;
        t=*(unsigned char *)(offset+(unsigned long)choices);
        a0 = (SelectionRow *)(a1+(unsigned long)b);
        a0->selection = t;
        if (t == page[0x4D98]) {
            a0->variant = b->variant;
        } else {
            a0->variant = *(unsigned char *)(a0->selection+(unsigned long)variants);
            /* ASM_BARRIER: Retain the original row-pointer recomputation after the variant store. */
            __asm__("" : "+r"(a0));
            a0 = (SelectionRow *)(a1+(unsigned long)b);
        }
        if (a0->selection != page[0x4D98]) {
            if (a0->variant == b->variant) {
                a0->variant = *(unsigned char *)(page[0x4D98]+(unsigned long)variants);
            }
        }
        v1 = (SelectionRow *)(a1+(unsigned long)b);
        /* ASM_BARRIER: Keep the current row and index before publishing its attribute. */
        __asm__("" : : "r"(v1), "r"(a2));
        attribute=config->copies[0].bytes[2];
        /* ASM_BARRIER: Read the row attribute before advancing the loop index. */
        __asm__("" : "+r"(attribute) : "r"(a2));
        a2 += 1;
        v1->parameter = attribute;
        a1 += sizeof(SelectionRow);
    } while (a2 < 7);
}

void GpSaveSlotMenuEnter(void) {
    ClearScreenOverlayState();
    ScreenFadeState = 2;
    GpSaveSlotCursor = 0;
    InitBadgeRow(1, 0x80, 0x58, 0xA8, 0x80, 0, 0x701);
    BadgeRows[1].half02 = 1;
    InitBadgeRow(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    BadgeRows->half02 = 1;
    SetMenuFrameOptions(0, 1, 1);
    ModeGroup = 1;
    ModeIndex = 0xE;
    SubmodeState = 0;
}

void GpSaveSlotMenuUpdate(void) {
    int s0;
    switch (SubmodeState) {
    case 0:
        UiRows[GpSaveSlotCursor].cells[0] = 0;
        StepMenuValue(0, 2, &GpSaveSlotCursor, 3, 0);
        UiRows[GpSaveSlotCursor].cells[0] = 2;
        if (BadgeRows->state10 == 3) {
            s0 = CardDialogueStatePtr->st;
            if (s0 == 4) {
                SetMenuFrameOptions(1, 1, 1);
                if (PlayerControllers[0].action28 != 0) {
                    if (SaveSlotSelections[GpSaveSlotCursor].phase17 > 0) {
                        PlaySfx(1);
                        SubmodeState = 1;
                        FadeCountdown = 0x40;
                        ResetScreenCounters();
                        LoadCursor = MenuSharedScratchPtr;
                        BadgeRows->state10 = s0;
                        BadgeRows[1].state10 = s0;
                    } else {
                        PlaySfx(4);
                    }
                } else if (PlayerControllers[0].action2C != 0) {
                    PlaySfx(2);
                    SubmodeState = 2;
                    FadeCountdown = 0x40;
                    ResetScreenCounters();
                    LoadCursor = MenuSharedScratchPtr;
                    LoadTimListStep(0);
                    BadgeRows->state10 = s0;
                    BadgeRows[1].state10 = s0;
                }
            }
        }
        break;
    case 1:
        SetMenuFrameOptions(0, 1, 1);
        if (FadeCountdown > 0) FadeCountdown = FadeCountdown - 1;
        AddScreenFadeTiles((u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]), (0x40 - FadeCountdown) * 4, 0x45);
        if (FadeCountdown <= 0) {
            if (LoadTimListStep(1) == 1) {
                SelectionState = SaveSlotSelections[GpSaveSlotCursor];
                ModeGroup = 1;
                ModeIndex = 0xF;
                SubmodeState = 0;
                return;
            }
        }
        break;
    case 2:
        SetMenuFrameOptions(0, 1, 1);
        if (FadeCountdown > 0) FadeCountdown = FadeCountdown - 1;
        AddScreenFadeTiles((u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]), (0x40 - FadeCountdown) * 4, 0x45);
        if (FadeCountdown <= 0) {
            UpdateGrandPrixStageProgress();
            MenuScriptId = 1;
            ModeGroup = 1;
            ModeIndex = 0x23;
            SubmodeState = 0;
            return;
        }
    default:
        break;
    }
    UpdateBadgeRowAnim(0);
    UpdateBadgeRowAnim(1);
    if (BadgeRows->state10 == 3) {
        CardDialogueStatePtr->b4 = 0x1C;
        UpdateDialogue(BadgeRows->x08 + 0xA, BadgeRows->y0A + 0x28, CardDialogueStatePtr);
    }
    UpdateScreenFade(0x10086);
}

void StepReplaySavePrompt(void) {
    if (MenuSelectedCarSlot & 0x1000) {
        MenuAutoAdvanceTimer = MenuAutoAdvanceTimer - 1;
    } else if (MenuSelectedCarSlot & 0x4000) {
        MenuAutoAdvanceTimer = MenuAutoAdvanceTimer + 1;
    }
    if (MenuAutoAdvanceTimer < 0) {
        MenuAutoAdvanceTimer = 1;
    } else if (MenuAutoAdvanceTimer >= 2) {
        MenuAutoAdvanceTimer = 0;
    }
    if (Pad0Buttons & 0x800) {
        ModeGroup = 1;
        ModeIndex = 0xF;
        SubmodeState = 0;
    }
}

void YesNoPromptModeEnter(void) {
    ModeFrame = 0;
    CardActivePort = 0;
    CardMenuOperation = 3;
    MenuCursor = 0;
    InitUiRow(0, 0x8C, 0x88, 0x1C, 0x20, 0, 0, 1, 0, 0x68);
    InitUiRow(1, 0x8C, 0xB0, 0x1C, 0x20, 0, 0, 1, 0, 0x69);
    InitBadgeRow(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    ModeGroup = 1;
    ModeIndex = 0x2B;
    SubmodeState = 0;
    BadgeRows->half02 = 1;
    UpdateScreenFade(0x10086);
}
