#ifndef RR_GAME_UI_WINDOW2_H
#define RR_GAME_UI_WINDOW2_H

#include "rr/gpu.h"

void *EmitHudGlyph12Alt(GpuDmaTag *ordering, void *cursor, s16 x, s16 y, s32 glyph, s32 clut);
void InitHudNumberTargets(int layout);
void DrawViewportBorderFrames(int layout);
void DrawHudRotatingQuad(s32 phase,s32 index);
void *EmitHudGlyph12(GpuDmaTag *ordering, void *cursor, s32 x, s32 y, s32 glyph, s32 clut);
void DrawHudNumber3(int value, int index);

#endif /* RR_GAME_UI_WINDOW2_H */
