/* RODATA 0x800105b0 0x18 */
#include "types.h"
#include "rr/menu_selection.h"
#include "rr/menu_text_state.h"
#include "rr/badge_row_types.h"
#include "game/gfx/fontdraw.h"
#include "game/menu/menu8.h"
#include "game/menu/menuslot.h"
#include "game/menu/menu13.h"
#include "rr/badge_rows.h"
#include "rr/menu_choice.h"

extern int MenuReturnSubmodeIndex;

void UpdateDialogue(int a0, int a1, MenuTextState *s);
int DecodeCourseIndex(s32 a0);
void EmitSpinningWheelIcon();

void UpdateDialogue(int a0, int a1, MenuTextState *s) {
    unsigned short *p;
    int i;
    int v;
    u8 withheld;
    int w;
    unsigned short *r;

    if (s->b4 != s->b5) {
        s->st = 0;
    }
    s->b5 = s->b4;
    if (s->b4 < 0) {
        return;
    }
    switch (s->st) {
    case 0:
        p = DialogueScriptPtrs[(signed char)(s->b4 % 40)];
        s->p = p;
        s->sC = p[0] & 0xFFF;
        s->p = p + 1;
        s->bA = *((unsigned char *)p + 2);
        s->p = p + 2;
        withheld = *((unsigned char *)p + 4);
        s->st = 5;
        s->b9 = 1;
        p += 3;
        s->p = p;
        s->bB = withheld;
        s->i18 = 0;
        v = DialoguePageCounts[s->b4];
        s->s12 = 0;
        s->sE = 1;
        s->s10 = 0;
        s->b1F = 0;
        s->s14 = 0;
        s->b8 = v;
        for (i = 199; i >= 0; i--) {
            s->animation28[i].word = 0;
        }
        break;
    case 1:
        if (s->i18 >= s->sC) {
            if (s->b7 == 1) {
                DrawDialogueContinueMark(a0 + 0x10D, a1 + 0x58);
                if (MenuReturnSubmodeIndex == 0) {
                    goto call1;
                }
                PlaySfx(3);
            }
            s->st = 3;
            s->s12 = s->bA - s->bB;
        } else {
            s->i18++;
            if (s->sE <= s->s10) {
                if (s->s12 < s->bA - s->bB) {
                    s->s12 = s->s12 + 1;
                } else if (s->s12 == s->bA - s->bB) {
                    if (s->b7 < 2) {
                        s->i18 = s->sC;
                    }
                }
                s->s10 = 0;
            } else {
                s->s10 = s->s10 + 1;
            }
        }
call1:
        DrawAnimatedText(a0, a1, s, 0, 0x3FFC);
        if ((signed char)s->b1C != (signed char)s->b1D) {
            s->b1F = 0;
            s->b1E = 0;
            s->b1D = s->b1C;
        }
        break;
    case 2:
        DialogueWaitEnterHook();
        DialogueWaitExitHook();
        DrawAnimatedText(a0, a1, s, 1, 0x3FFC);
        break;
    case 3:
        if ((signed char)s->b8 > s->b9) {
            r = (unsigned short *)((char *)s->p + s->bA * 2);
            s->p = r;
            s->sC = r[0] & 0xFFF;
            s->p = r + 1;
            s->bA = *((unsigned char *)r + 2);
            s->p = r + 2;
            w = *((unsigned char *)r + 4);
            r += 3;
            s->p = r;
            s->s12 = 0;
            s->bB = w;
            for (i = 199; i >= 0; i--) {
                s->animation28[i].word = 0;
            }
            s->i18 = 0;
            s->s10 = 0;
            s->b9 = s->b9 + 1;
            s->sE = 1;
            s->st = 1;
            if (s->b9 == (signed char)s->b8) {
                Dialogue->b7 = 0;
            }
            break;
        }
    case 4:
        DrawAnimatedText(a0, a1, s, 1, 0x3FFC);
        s->st = 4;
        break;
    case 5:
        if (BadgeRows[s->s16].state10 == 3) {
            s->st = 1;
        }
        break;
    }
}

void SkipDialoguePage(MenuTextState *arg0) {
    if (arg0->i18 < arg0->sC) {
        if (MenuReturnSubmodeIndex != 0) {
            if (arg0->st != 4) {
                PlaySfx(3);
                if (arg0->s12 < (arg0->bA - arg0->bB)) {
                    arg0->s12 = (s16) (arg0->bA - arg0->bB);
                    arg0->i18 = (arg0->sC - 1);
                    return;
                }
                arg0->i18 = (arg0->sC - 1);
            }
        }
    }
}

s32 DecodeCourseIndex(s32 a0) {
    ReverseCourse = a0 >= 8;
    return CoursePreviewSceneIds[a0 & 7];
}

void EmitSpinningWheelIcon(int a0, int a1, int a2, int a3, int a4) {
    int m = (a4 << 5) % 256;
    unsigned short s = (a4 << 6) + 0x3828;
    AddSpriteWithDrawMode(a0, a1, a2, a3, 0x20, 0x40, m, 0xC0, 0x9A, s, 0, 0x80);
}
