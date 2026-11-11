#ifndef RR_GAME_UI_UIWIN_H
#define RR_GAME_UI_UIWIN_H

#include "types.h"

int AllocUiRows(void *a);
void InitUiRow(s32 index, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 count, s32 g, s32 h, ...);
void DrawUiWindowRowText(s32 arg0, s32 arg1, s32 arg2);
int UpdateGlyphRow(int index);


#endif /* RR_GAME_UI_UIWIN_H */
