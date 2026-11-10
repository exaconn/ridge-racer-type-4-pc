#ifndef RR_MENU_TEXT_STATE_H
#define RR_MENU_TEXT_STATE_H
#include "rr/layout.h"

/* One four-byte animation cell per emitted glyph. The glyph helper updates
 * the first signed halfword and preserves the second. */
typedef union { s32 word; s16 halves[2]; } DialogueGlyphAnimation4;
typedef struct {
    u16 *p;
    s8 b4,b5,st,b7;
    u8 b8;
    s8 b9;
    u8 bA,bB;
    s16 sC,sE,s10,s12,s14,s16;
    s32 i18;
    s8 b1C,b1D,b1E;
    u8 b1F;
    u8 *images20;
    s32 reserved24;
    DialogueGlyphAnimation4 animation28[200];
} MenuTextState;
extern MenuTextState *Dialogue;
extern MenuTextState *CardDialogueStatePtr;
void SkipDialoguePage(MenuTextState *state);
void UpdateDialogue(s32 x,s32 y,MenuTextState *state);
RR_ASSERT_SIZE(DialogueGlyphAnimation4,4);
#if defined(__mips__)
RR_ASSERT_SIZE(MenuTextState,0x348);
RR_ASSERT_OFFSET(MenuTextState,i18,0x18);
RR_ASSERT_OFFSET(MenuTextState,images20,0x20);
RR_ASSERT_OFFSET(MenuTextState,animation28,0x28);
#endif
#endif
