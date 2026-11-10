#include "game/data/cardchk.h"
#include "game/menu/loadmenu.h"
#include "game/menu/savemenu.h"
#include "game/data/cardini.h"
#include "game/menu/menumode.h"
#include "game/data/menuvars.h"
#include "game/system/pad.h"
#include "rr/ui.h"
#include "rr/menu_choice.h"
#include "rr/string_tail_copy.h"

extern int StepMenuValue();
extern short CardMenuOperation;
extern short CardActivePort;
extern short MenuActiveControllerIndex;
extern short MenuStateIndex;
extern short GpSaveSlotCursor;
extern short D_800F3AF8;
extern char CardTargetFileName[];
extern char CardFileNameTemplate[];
extern char CardSaveFileName[];
extern unsigned char D_800A448C[][4];
extern unsigned char CardPortPollStatus[];
extern short D_80109478[];
extern short D_8010948E;
extern unsigned char MenuCardStepRows[][4];
extern unsigned char CardFileCounts[];

void StepCardFileSlotSelection(void);
void StepCardPortSelection();
void UpdateCardMenuRowStates();

void StepCardFileSlotSelection(void) {
    /* ASM_BARRIER: pins value to s3 as in the original */
    register int s3 asm("$19");
    int i;
    i = MenuStateTable[MenuStateIndex].tableIdx04;
    s3 = D_800A448C[i][1];
    switch (CardMenuOperation) {
    case 3:
    case 6:
    case 7:
        StepMenuValue((MenuActiveControllerIndex & 1) << 2, 2, &GpSaveSlotCursor, 3, 0);
        if (PlayerControllers[MenuActiveControllerIndex].action28 != 0) {
            PlaySfx(1);
            if ((unsigned short)(CardMenuOperation - 6) < 2) {
                D_800F3AF8 = 0x11;
                CopyStringClearTail(CardTargetFileName, CardTargetFileNames[GpSaveSlotCursor]);
            } else {
                CopyStringClearTail(CardTargetFileName, CardSaveFileName);
            }
        } else {
            if (PlayerControllers[MenuActiveControllerIndex].action2C != 0) {
                MenuStateIndex = 0;
            }
            return;
        }
        break;
    default:
        if (CardMenuOperation == 0) {
            if (MenuStateIndex == 0x3A) {
                D_801094A0 = 1;
                D_800F3AF8 = 0x20;
                CopyStringClearTail(CardTargetFileName, CardFileNameTemplate);
            } else {
                CopyStringClearTail(CardTargetFileName, CardSaveFileName);
            }
        } else {
            CopyStringClearTail(CardTargetFileName, CardSaveFileName);
        }
        break;
    }
    MenuStateIndex = s3;
    PollCardStatus(CardActivePort, 0, 1);
}

void StepCardPortSelection(void) {
    int a, s4, s3;
    /* ASM_BARRIER: pin q to a0 ($4) to reproduce original register allocation */
    register unsigned char *q asm("$4");
    unsigned char *r;
    ControllerState5C *r2;
    ControllerState5C *b3;
    unsigned char *b2;
    unsigned char (*tb)[4];
    int t, c;
    int ia;
    MenuStateEntry8 *base;
    MenuStateEntry8 *p;
    signed char sb;

    base = MenuStateTable;
    p = base + MenuStateIndex;
    q = (unsigned char *)p->tableIdx04;
    c = p->promptIdx06;
    D_8010948E = c;
    if (D_800F4D8A == 0) {
        sb = p->defaultChoice07;
        if (sb >= 0) {
            D_80109478[c] = sb;
        }
        D_800F4D8A = 1;
    }
    tb = D_800A44A8;
    ia = (int)q * 4;
    b2 = CardPortPollStatus;
    r = b2 + CardActivePort;
    q = (unsigned char *)tb + ia;
    a = q[0];
    s4 = q[1];
    s3 = q[2];
    if (*r < 0x10 || a == *r) {
        if (UiRows[0x26].byte14 == 2) {
            StepMenuValue(((unsigned short)MenuActiveControllerIndex & 1) << 2, 2, D_8010948E * 2 + (int)D_80109478, 2, 0);
            b3 = PlayerControllers;
            r2 = b3 + MenuActiveControllerIndex;
            if (r2->action28 != 0) {
                PlaySfx(1);
                if (D_80109478[D_8010948E] == 0) {
                    MenuStateIndex = s4;
                } else {
                    MenuStateIndex = s3;
                }
                D_800F4D8A = 0;
            } else if (r2->action2C != 0) {
                PlaySfx(2);
                MenuStateIndex = s3;
                D_800F4D8A = 0;
            }
        }
    } else {
        if (*r == 0x32) return;
        if (*r != 0x14) return;
        t = MenuStateIndex;
        if (t == 2 || t == 0x1A || t == 0x2D || t == 0x3C) {
            D_80109458 = MenuStateIndex;
            MenuStateIndex = 8;
        } else if (t == 0x10 || t == 0x23) {
            MenuStateIndex = 0x12;
        } else if (t == 0x34) {
            MenuStateIndex = 0x36;
        }
        D_800F4D8A = 0;
    }
}


void UpdateCardMenuRowStates(void) {
    unsigned char *e;
    int s2, s4, s0, s3;
    int a0;
    unsigned char (*tab)[4];
    int i = MenuStateTable[MenuStateIndex].tableIdx04;
    tab = MenuCardStepRows;
    e = tab[i];
    s2 = e[0];
    s4 = e[1];
    s0 = e[2];
    s3 = e[3];
    if (CardMenuOperation == 0) {
        if (MenuStateIndex == 0x3D) {
            if (CardHasFile(CardFileCounts[CardActivePort], CardSaveFileName) != 0) {
                D_80109490 = 1;
            } else {
                D_80109490 = 0;
            }
        }
    }
    if (CardHasFile(CardFileCounts[CardActivePort], CardTargetFileName) == 1) {
        MenuStateIndex = s2;
        if (s0 == 1) {
            a0 = 2;
            goto cf0;
        }
        CardLoadStep(2, 0);
        return;
    }
    if (s0 == 1) {
        if (CardPortPollStatus[CardActivePort] + s3 >= 0x10) {
            MenuStateIndex = 0x16;
            return;
        }
    }
    MenuStateIndex = s4;
    if (s0 != 1) return;
    a0 = 1;
cf0:
    CardSaveStep(a0, 0);
}
