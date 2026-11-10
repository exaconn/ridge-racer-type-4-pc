#ifndef RR_GLYPH_LIST_H
#define RR_GLYPH_LIST_H
#include "rr/icon_sprites.h"
#include "rr/ui_bar.h"
typedef struct {u32 addr:24;u32 len:8;u32 command;} GlyphCommand8;
typedef struct {SpritePacketSignedClut sprite;GlyphCommand8 mode;} GlyphPacket28;
extern UiUvRect D_800A1090[50],HudGlyphUvRects[40];
#ifndef RR_GLYPH_SCRATCH
#define RR_GLYPH_SCRATCH ((GpuScratchPointers*)0x1F800000U)
#endif
void DrawGlyphList(s16*,s32,s32);
/* Readable signed16 x/y, then glyph/gap pairs and a negative terminator.
 * Bank0 selects primary glyph0..49; every nonzero bank selects0..39.
 * Glyph0x400..0x427 always selects the secondary40-entry font. Each glyph
 * needs28 writable aligned bytes in a live packet arena, disjoint from input
 * and font owners. Software cursors retain native width; only DMA wire tags
 * use low24 address bits. X arithmetic wraps32 before wire16 publication. */
RR_ASSERT_SIZE(GlyphCommand8,8);
RR_ASSERT_SIZE(GlyphPacket28,28);
RR_ASSERT_OFFSET(GlyphCommand8,command,4);
RR_ASSERT_OFFSET(GlyphPacket28,mode,20);
#endif
