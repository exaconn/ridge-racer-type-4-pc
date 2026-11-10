#ifndef RR_GAME_UI_WINDOW2_H
#define RR_GAME_UI_WINDOW2_H

#include "types.h"

void *EmitHudGlyph12Alt(s32 *arg0, void *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5);
void InitHudNumberTargets(int a0);
void DrawViewportBorderFrames(int a0);
void DrawHudRotatingQuad(s32 phase,s32 index);
void *EmitHudGlyph12(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void DrawHudNumber3(int a0, int a1);


#endif /* RR_GAME_UI_WINDOW2_H */
