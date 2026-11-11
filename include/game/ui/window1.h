#ifndef RR_GAME_UI_WINDOW1_H
#define RR_GAME_UI_WINDOW1_H

#include "rr/gpu.h"

/* One HUD sprite preset (BadgeRowPresetTableA/B, Rect16RowDefaultPresets,
 * UiRowDefaultPresets, GlyphRowDefaultPresets): position, size, UV, CLUT
 * and a semi-transparency flag. */
typedef struct {
    u16 x00;
    u16 y02;
    u16 w04;
    u16 h06;
    u8 u08;
    u8 pad09;
    u8 v0A;
    u8 pad0B;
    u16 clut0C;
    u16 pad0E;
    s32 semiTrans10;
} HudSpriteDef14;
RR_ASSERT_SIZE(HudSpriteDef14, 0x14);

/* Builds an untinted textured sprite from a preset (does not link it). */
void InitHudSpritePacket(SpritePacket *packet, const HudSpriteDef14 *preset);
/* Digit strings ('0'..'9' and the punctuation glyphs after them). */
void DrawDigitString12(s32 x, s32 y, const char *text, s32 clut);
void DrawDigitString8(GpuDmaTag *ordering, s32 x, s32 y, const char *text, s32 clut);
void DrawDigitString16(GpuDmaTag *ordering, s32 x, s32 y, const char *text);
/* Builds / links the persistent per-bank race HUD sprites
 * (FrameBuffer.hudSprites) for a game-mode layout. */
void InitRaceHudPackets(int layout);
void SubmitRaceHudPackets(int layout);

#endif /* RR_GAME_UI_WINDOW1_H */
