#ifndef RR_GAME_UI_UITEXT_H
#define RR_GAME_UI_UITEXT_H

#include "types.h"

#include "rr/image_rows.h"

int DrawUiRow(s32 arg0);
void DrawUiBox(s32 x, s32 y, s32 width, s32 height, s32 red, s32 green, s32 blue);
int AllocImageRows(void *a);
void InitImageRow(int i, int a, int b, int j);
int UpdateImageRow(int a0);
void DrawImageRow(int a0, int a1, int a2, int a3);

#endif /* RR_GAME_UI_UITEXT_H */
