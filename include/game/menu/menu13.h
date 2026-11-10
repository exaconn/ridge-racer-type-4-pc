#ifndef RR_GAME_MENU_MENU13_H
#define RR_GAME_MENU_MENU13_H

#include "types.h"
#include "rr/menu_text_state.h"

extern unsigned char *DialoguePageCounts;
extern u16 *DialogueScriptPtrs[];
void SkipDialoguePage(MenuTextState *arg0);
s32 DecodeCourseIndex(s32 a0);
void EmitSpinningWheelIcon(int a0, int a1, int a2, int a3, int a4);


#endif /* RR_GAME_MENU_MENU13_H */
